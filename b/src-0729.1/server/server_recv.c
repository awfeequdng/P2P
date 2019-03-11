

#include "all.h"
#include "server.h"



/**************************************
������: recv_pthread
����:		�����߳�
***************************************/
void *server_recv_pthread(void *pdata)
{	
	int i;
	
	struct sockaddr_in clientaddr;
//	  struct message recvbuf;
	char recvbuf[UDP_PACK_MAX_SIZE];
	struct check_head *head;
	int ret;
	int clilen;

	/* ��ȡ sockert ��ǩ�� */
	int sockfd = server_info.sockfd;			/* ��ȡfd */

	app_printf("recv_pthread %d... \n", sockfd);
	while(1)
	{
		
		clilen = sizeof(clientaddr);
		ret = p2p_recvfrom(sockfd, (char *)recvbuf, sizeof(recvbuf), 0, (struct sockaddr *)&clientaddr, (socklen_t *)&clilen);


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

			/* Ȩ�޼�� */

//			dbg_printf("addr %p", val);

			for(i = 0; aff_table[i].aff_fun != NULL; i++)
			{
				if(aff_table[i].aff == head->affairs)
				{
					aff_table[i].aff_fun(sockfd, recvbuf, ret, &clientaddr);
				}
			}
		}
	}
}


/*******************************************
������:	__init_socket
����:		��ʼ��������socket
*********************************************/

int __init_socket(unsigned int port)
{
    int sockfd;
    struct sockaddr_in serveraddr;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1)
    {
        perror("bind");
        exit(1);
    }

    return sockfd;
}





/*******************************************
������:	check_Active_pthread
����:		��Ҫ�����ڼ���½�û��Ļ�Ծ״̬��
			���ڲ���Ծ�ߣ�T ��
*********************************************/
void *check_Active_pthread(void *pdata)
{
	struct value *v, *found;
	struct key *kk;
	struct hashtable_itr *itr;
	
	while(1){

//		pthread_mutex_lock(&hashtable_lock);
		if (hashtable_count(h_table) > 0)
		{
			itr = hashtable_iterator(h_table);
			do {
				v = hashtable_iterator_value(itr);
				kk = hashtable_iterator_key(itr);

				if(v->cli_info.login_cnt > 0){
					v->cli_info.login_cnt -= 30;
				}else{
					app_printf("remove %s\r\n", kk->name);

					/* �����ݿ��б��Ϊ������ */
					sql_table_update_string("device", "name", kk->name, "is_online", "offline");
					sql_table_update_string("system", "name", kk->name, "is_online", "offline");
					sql_table_update_string("network", "name", kk->name, "is_online", "offline");
					sql_table_update_string("other", "name", kk->name, "is_online", "offline");
					
					if (NULL == (found = __remove_some(h_table,kk))) {
            			app_printf("BUG: key not found for removal\n");
        			}
				}
			} while (hashtable_iterator_advance(itr));
			free(itr);
		}
//		pthread_mutex_unlock(&hashtable_lock);
		
		sleep(SYNC_TIME);
	}
}


/***********************************************
������: server_sock_init
����:		��ʼ����������socket �߳� 
***********************************************/
void init_server_sock_pthread(void)
{
	pthread_t recv_thread;
    pthread_attr_t recv_thread_attr;

	server_info.sockfd = __init_socket(SERVER_SYNC_PORT);
	/* �����ӽ��� */
	pthread_attr_init(&recv_thread_attr);		//��ʼ����������
	pthread_attr_setdetachstate(&recv_thread_attr, PTHREAD_CREATE_DETACHED);
	if (pthread_create(&recv_thread, &recv_thread_attr, server_recv_pthread, NULL) < 0)
	{
		perror("pthread_create");
	}

#if 1
	/* ���ͻ����Ƿ��Ծ */
	pthread_attr_init(&recv_thread_attr);		//��ʼ����������
	pthread_attr_setdetachstate(&recv_thread_attr, PTHREAD_CREATE_DETACHED);
	if (pthread_create(&recv_thread, &recv_thread_attr, check_Active_pthread, NULL) < 0)
	{
		perror("pthread_create");
	}
#endif
}


