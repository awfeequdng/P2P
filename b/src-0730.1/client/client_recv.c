#define __CLIENT_RECV_C_

#include "all.h"
#include "client.h"





/********************************************
������: client_recv_handle
����:		�ͻ���udp���յ����ݺ�ĵ�һ��������
*********************************************/

void client_recv_handle(void)
{
	int i;
	struct sockaddr_in clientaddr;
	char recvbuf[UDP_PACK_MAX_SIZE];
	struct check_head *head;
	int ret;
	int clilen;

	while(1)
	{
		
		clilen = sizeof(clientaddr);
		ret = p2p_recvfrom(sys_cfg.sockfd, (char *)recvbuf, sizeof(recvbuf), 0, (struct sockaddr *)&clientaddr, (socklen_t *)&clilen);


		if (ret < 0)
		{
			perror("my_recvfrom_handle");
			continue;
		}
		else
		{
			head = (struct check_head *)recvbuf;

			/* ��鳤�� */
			if(ret < sizeof(struct check_head))
				continue;

//			printf_client(&clientaddr);
//			printf("aff is %d \r\n", head->affairs);
			
			for(i = 0; client_aff_table[i].aff_fun != NULL; i++)
			{
				if(client_aff_table[i].aff == head->affairs)
				{
					client_aff_table[i].aff_fun(sys_cfg.sockfd, recvbuf, ret, &clientaddr);
					break ;
				}
			}
		}
	}

}




/********************************************
������: connet_to_server
����:		�����������UDP ��������
*********************************************/
int connet_to_server(char *serverip, int port)
{
	char sendbuf[1024];
	int send_len;
	struct sockaddr_in servaddr;
	struct sockaddr_in recvaddr;
	int nread;
	int ret;
	int fromlen;
	char recvbuf[1024];
    fd_set readfds;
    fd_set writefds;
	struct timeval tv;

	struct check_head head, *recv_head;;
	char *str;

	struct proto_s_login_ack *ack;


	dbg_printf("connet to %s:%d\r\n", serverip, port);

	/* ����������socket ��ǩ�� */
	memset(&servaddr, 0, sizeof(struct sockaddr_in));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(serverip);
    servaddr.sin_port = htons(port);

	/* �����ͷ */
	compages_head(&head, _aff_client_login_);

	/* �ϲ����� */	
	memcpy(sendbuf, &head, sizeof(head));
	send_len = sizeof(head);
	
    ret = p2p_sendto(sys_cfg.sockfd, sendbuf, send_len, 0, (struct sockaddr *)&servaddr, sizeof(struct sockaddr));
	if(ret == -1){
		return -1;
	}

   while(1)
   {
	   
	   int maxfd = sys_cfg.sockfd;
	   
	   FD_ZERO(&readfds);
	   FD_ZERO(&writefds);
	   FD_SET(sys_cfg.sockfd, &readfds);

	   /* �ȴ�5 �룬���������û�з��أ���ôʧ�� */
	   tv.tv_sec = 5;
	   tv.tv_usec = 0;
	   ret = select(maxfd + 1, &readfds, &writefds, NULL, &tv);
	   
	   if (ret > 0)
	   {
		   if (FD_ISSET(sys_cfg.sockfd, &readfds)) 
		   {
			   fromlen = sizeof(servaddr);
			   nread = p2p_recvfrom(sys_cfg.sockfd,(char *)&recvbuf,sizeof(recvbuf),0,(struct sockaddr *)&recvaddr, (socklen_t *)&fromlen);
			   if(nread <= 0)
				   return -1;
			   
			   printf_client(&recvaddr);

			   if(recvaddr.sin_addr.s_addr != inet_addr(sys_cfg.serverip))
				   continue;

			   recv_head = (struct check_head *)recvbuf;
			   if((recv_head->affairs == _aff_server_login_ack_))
				   goto login_ack;
		   }
	   }
	   ret = p2p_sendto(sys_cfg.sockfd, sendbuf, send_len, 0, (struct sockaddr *)&servaddr, sizeof(struct sockaddr));
	   if(ret == -1){
		   perror("sendto ");
	   }
   }

login_ack:
	
	ack = recvbuf + sizeof(struct check_head);
	return ack->ack;		//���ص�½���

}



/********************************************
������: sync_service
����:		������ͬ�����ӽ���
*********************************************/

void *sync_thread_fun(void *pdata)
{
	while(1)
	{
		sync_service(); 			//ͬ��������
		sleep(SYNC_TIME);
	}
}



/************************************************
������: client_recv_pthread
����:		�ͻ��˽��մ����߳�
************************************************/
void *client_recv_pthread(void *pdata)
{
	
	pthread_t sync_thread;
    pthread_attr_t sync_thread_attr;

	/* ���ӵ������� */
	connet_to_server(sys_cfg.serverip, sys_cfg.ser_port);

	/* ����һ��ͬ�����߳� */
	pthread_attr_init(&sync_thread_attr);		//��ʼ����������
    pthread_attr_setdetachstate(&sync_thread_attr, PTHREAD_CREATE_DETACHED);
    if (pthread_create(&sync_thread, &sync_thread_attr, sync_thread_fun, NULL) < 0)
    {
        perror("pthread_create");
    }


	client_recv_handle();

	while(1);
}




/********************************************
������: sync_service
����:		������������������
*********************************************/

void sync_service(void)
{
	int ret;
	struct sockaddr_in servaddr;
	struct check_head head;
	char sendbuf[1024];
	int send_len;

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(sys_cfg.serverip);
    servaddr.sin_port = htons(sys_cfg.ser_port);

	compages_head(&(head), _aff_client_sync_);

	memcpy(sendbuf, &head, sizeof(head));
	send_len = sizeof(head);

	ret = p2p_sendto(sys_cfg.sockfd, sendbuf, send_len, 0, (struct sockaddr *)&servaddr, sizeof(struct sockaddr));
	if(ret == -1){
		perror("sendto ");
	}
}



void init_network_pthread(void)
{
	pthread_t thread;
    pthread_attr_t thread_attr;
	
	pthread_t aff_thread;
    pthread_attr_t aff_thread_attr;

	/* �����ӽ��� */
	pthread_attr_init(&thread_attr);		//��ʼ����������
	pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
	if(pthread_create(&thread, &thread_attr, client_recv_pthread, NULL) < 0)
	{
		perror("pthread_create");
	}
}

