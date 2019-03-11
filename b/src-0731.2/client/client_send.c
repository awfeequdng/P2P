

#include "all.h"
#include "client.h"

int send_to_server(char *sendbuf, int send_len)
{
	int ret;
	struct sockaddr_in serveraddr;
	
	serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(sys_cfg.serverip);
    serveraddr.sin_port = htons(sys_cfg.ser_port);

	ret = p2p_sendto(sys_cfg.sockfd, (char *)sendbuf, send_len, 0, (struct sockaddr *)&serveraddr,
		   sizeof(serveraddr));

	/* ��������ӵ�������ȥ */
	add_net_data_list(sendbuf, send_len, &serveraddr);
	return ret;
}


/*
��ȡĳ���ͻ��˵���Ϣ
*/
int get_client_info(char *name)
{
	struct sockaddr_in serveraddr;
	
	struct check_head head;
	int ret;
	char sendbuf[1024];
	int send_len;

	
	serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(sys_cfg.serverip);
    serveraddr.sin_port = htons(sys_cfg.ser_port);

	/* �����ͷ */
	compages_head(&(head), _aff_client_get_client_);

	struct proto_c_get_lient proto;
	__strcpy((proto.name), name, USER_NAME_LEN);

	memcpy(sendbuf, &head, sizeof(head));
	memcpy(sendbuf + sizeof(head), &proto, sizeof(proto));
	send_len = sizeof(head) + sizeof(proto);

	ret = p2p_sendto(sys_cfg.sockfd, (char *)sendbuf, send_len, 0, (struct sockaddr *)&serveraddr,
		   sizeof(serveraddr));

	/* ��������ӵ�������ȥ */
	add_net_data_list(sendbuf, send_len, &serveraddr);

	return ret;
}


#if 0

/* ͨ����������ת�ķ�ʽ��ĳ���ͻ��˷�����Ϣ */
int send_client_data(char *name, char *data, int len)
{
	struct proto_c_send_data proto;
	struct check_head head;

	char sendbuf[1204];
	int send_len;
	int ret;
	
	struct sockaddr_in serveraddr;

	serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(sys_cfg.serverip);
    serveraddr.sin_port = htons(sys_cfg.ser_port);

	/* �����ͷ */
	compages_head(&(head), _aff_client_send_data_);

	__strcpy((proto.dest_name), name, USER_NAME_LEN);
	__strcpy((proto.src_name), sys_cfg.myname, USER_NAME_LEN);

	memcpy(sendbuf, &head, sizeof(head));
	memcpy(sendbuf + sizeof(head), &proto, sizeof(proto));
	send_len = sizeof(head) + sizeof(proto);
	memcpy(sendbuf + send_len, data, len);
	send_len += len;

	ret = p2p_sendto(sys_cfg.sockfd, (char *)sendbuf, send_len, 0, (struct sockaddr *)&serveraddr,
		   sizeof(serveraddr));

	/* ��������ӵ�������ȥ */
	add_net_data_list(sendbuf, send_len, &serveraddr);

	return ret;
}

#endif



/*----------------------------------------------------------------------*/
 pthread_mutex_t send_mtx = PTHREAD_MUTEX_INITIALIZER;
 pthread_cond_t send_cond = PTHREAD_COND_INITIALIZER;


int send_ack_flg = 0;	//�Ƿ���Ӧ��
struct check_head ack_head;		//���յ���Ӧ��ͷ��

/*
��������
wait_flg  = 1  ��������
		= 0 ������
*/
int __send_client_data(char *name, char *data, int len, int wait_flg, int delay_time)
{
	int ret;

	struct proto_c_send_data proto;
	struct check_head head;

	char sendbuf[1204];
	int send_len;
	
	struct sockaddr_in serveraddr;

	serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(sys_cfg.serverip);
    serveraddr.sin_port = htons(sys_cfg.ser_port);

	/* �����ͷ */
	compages_head(&(head), _aff_client_send_data_);

	__strcpy((proto.dest_name), name, USER_NAME_LEN);
	__strcpy((proto.src_name), sys_cfg.myname, USER_NAME_LEN);

	memcpy(sendbuf, &head, sizeof(head));
	memcpy(sendbuf + sizeof(head), &proto, sizeof(proto));
	send_len = sizeof(head) + sizeof(proto);
	memcpy(sendbuf + send_len, data, len);
	send_len += len;

	ret = p2p_sendto(sys_cfg.sockfd, (char *)sendbuf, send_len, 0, (struct sockaddr *)&serveraddr,
		   sizeof(serveraddr));

	/* ��������ӵ�������ȥ */
	add_net_data_list(sendbuf, send_len, &serveraddr);

	if(wait_flg == 0)
		return ret;

	if(wait_ack_head(&head, delay_time) == 0)
	{
		return ret;
	}else{
		return -1;
	}
}

/* �ȴ���Ӧ��� */
int wait_ack_head(struct check_head *head, int delay_time)
{	
	int ret;
	
	pthread_mutex_lock(&send_mtx);

	struct timespec to;
	to.tv_sec = time(NULL) + delay_time;
	to.tv_nsec = 0;
		
loop:
	send_ack_flg = 0;
	while(send_ack_flg == 0){

		ret = pthread_cond_timedwait(&send_cond,&send_mtx, &to);	//�ȴ�����.��ʱ�˳�
		if(ret == ETIMEDOUT)
		{				
			pthread_mutex_unlock(&send_mtx);//�ٽ������ݲ�����ϣ��ͷŻ�����
			return -1;		//��ʱ�˳�
		}
	}

	/* �Ƚ���Ӧ����Ƿ���һ���� */
	if(ack_head.key != head->key){
		/* ����� �����ȴ� */
//		goto loop;	
		pthread_mutex_unlock(&send_mtx);//�ٽ������ݲ�����ϣ��ͷŻ�����
		return -1;
	}

	pthread_mutex_unlock(&send_mtx);//�ٽ������ݲ�����ϣ��ͷŻ�����

	return 0;
}


void recv_ack_wakeup(struct check_head *head)
{
#if 1
	pthread_mutex_lock(&send_mtx);//��Ҫ����head����ٽ���Դ���ȼ�����

	memcpy(&ack_head, head, sizeof(struct check_head));
	send_ack_flg = 1;
	
	pthread_cond_signal(&send_cond);		//��������

	pthread_mutex_unlock(&send_mtx);//����
#endif
}

















/* ͨ����������ת�ķ�ʽ��ĳ���ͻ��˷�����Ϣ */
int send_client_data(char *name, char *data, int len)
{
	__send_client_data(name, data, len, 0, 0);
}


/* �������� */
int send_client_data_2(char *name, char *data, int len, int delay_time)
{
	__send_client_data(name, data, len, 1, delay_time);
}

