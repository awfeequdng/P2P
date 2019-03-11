
#ifndef __CLIENT_INTERFACE_H__
#define __CLIENT_INTERFACE_H__

#ifdef __CLIENT_INTERFACE_C__
#define __CLIENT_INTERFACE_EXT__
#else
#define __CLIENT_INTERFACE_EXT__
#endif

#include "all.h"
#include "client.h"

struct client_interface_ops{
	/* ���յ���������ת������ */
	void (*recv_stun)(int sockfd, char *buf, int len, struct sockaddr_in *clientaddr);	
	/* ���յ���������ת��Ӧ��� */
	void (*recv_stun_ack)(int sockfd, char *buf, int len, struct sockaddr_in *clientaddr); 
	/* ���������ؽڵ���Ϣ */
	void (*recv_node_info)(int sockfd, char *buf, int len, struct sockaddr_in *clientaddr);

	/* ����ʧ�� */	
	void (*data_send_err)(struct net_data *net_dat);

	
	/* ���յ�P2P������ */
	void (*recv_turn)(int sockfd, char *buf, int len, struct sockaddr_in *clientaddr);	
	/* ���յ�P2P��Ӧ��� */
	void (*recv_turn_ack)(int sockfd, char *buf, int len, struct sockaddr_in *clientaddr); 
};


__CLIENT_INTERFACE_EXT__ struct client_interface_ops client_inface;


int p2p_client_init(void);

void client_command_init(void);

void set_data_send_err(
	void (*data_send_err)(struct net_data *net_dat)
);
void set_recv_node_info(
	void (*recv_node_info)(int sockfd, char *buf, int len, struct sockaddr_in *clientaddr)
);
void set_recv_stun_ack(
	void (*recv_stun_ack)(int sockfd, char *buf, int len, struct sockaddr_in *clientaddr)
);
void set_recv_stun(
	void (*recv_stun)(int sockfd, char *buf, int len, struct sockaddr_in *clientaddr)
);


/*
sendto_node_unreliable 
���ɿ������ݴ��䡣����ֵ����0  ��ʾ���ͳɹ�
���ǲ���֤�Է�һ�����յ�������UDP
��������
*/
int sendto_node_unreliable(char *name, char *buf, int len);

/*
sendto_node_reliable 
��Կɿ������ݴ��䡣����ֵ����0  ��ʾ���ͳɹ�
���ҶԷ�һ�����յ���
�ú�����ȴ��Է���Ӧ���źš����������ֵ����0
����ζ�ŶԷ�һ���յ���
����Է���Ӧ������10��󽫻᷵��-1
*/
int sendto_node_reliable(char *name, char *buf, int len, int delay_time);

/* 
recv_node_data
���������ݶ�����ȡ��һ�����ݰ�
delay_time	Ϊ -1 ���ʾ������һֱ����
			����0 ��ʾ������������һ��ʱ�䡣Ȼ�󷵻�
*/
int recv_node_data(char *buf, int len, struct sockaddr_in *addr, int delay_time);


#endif


