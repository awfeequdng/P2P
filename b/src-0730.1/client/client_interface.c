
#define __CLIENT_INTERFACE_C__


#include "all.h"
#include "client.h"



void default_recv_stun(int sockfd, char *buf, int len, struct sockaddr_in *clientaddr)
{
	struct proto_c_send_data *recv_proto;
	recv_proto = buf + sizeof(struct check_head);
	
	printf("recv data : \r\n");
	printf("%s send to %s data is [%s] \r\n", recv_proto->src_name, recv_proto->dest_name,
							buf + sizeof(struct check_head) + sizeof(struct proto_c_send_data));
}

void default_recv_node_info(int sockfd, char *buf, int len, struct sockaddr_in *clientaddr)
{
	//printf("get client's info is ok \r\n");
	struct proto_s_client_info *recv_proto;
	recv_proto = buf + sizeof(struct check_head);
	
	if(recv_proto->status != 0)
	{
		printf("%s is offline \r\n", recv_proto->name);
	}else{
		printf("%s is online \r\n", recv_proto->name);
	}
}

void default_recv_stun_ack(int sockfd, char *buf, int len, struct sockaddr_in *clientaddr)
{
	printf("recv ack, send is ok \r\n");
	
	struct proto_c_send_data *recv_proto;
	recv_proto = buf + sizeof(struct check_head);
	
	printf("%s send to %s ack \r\n", recv_proto->src_name, recv_proto->dest_name);
}


void default_data_send_err(struct net_data *net_dat)
{
	printf("send failed! \r\n");
}

struct client_interface_ops client_inface = {
	.recv_stun = default_recv_stun,
	.recv_node_info = default_recv_node_info,
	.recv_stun_ack = default_recv_stun_ack,
	.data_send_err = default_data_send_err,
};





/* �����Լ��Ĵ����� */
void set_recv_stun(
	void (*recv_stun)(int sockfd, char *buf, int len, struct sockaddr_in *clientaddr)
)
{
	client_inface.recv_stun = recv_stun;
}


/* �����Լ��յ���תack �ķ��� */
void set_recv_stun_ack(
	void (*recv_stun_ack)(int sockfd, char *buf, int len, struct sockaddr_in *clientaddr)
)
{
	client_inface.recv_stun_ack = recv_stun_ack;
}

/* �����Լ���ȡ�ڵ���Ϣ�Ĵ����� */
void set_recv_node_info(
	void (*recv_node_info)(int sockfd, char *buf, int len, struct sockaddr_in *clientaddr)
)
{
	client_inface.recv_node_info = recv_node_info;
}

/* �����Լ��ķ���ʧ�ܴ����� */
void set_data_send_err(
	void (*data_send_err)(struct net_data *net_dat)
)
{
	client_inface.data_send_err = data_send_err;
}





/* 
ȫ�ֳ�ʼ�� 
*/
int p2p_client_init(void)
{
	/* ������ӡ��Ϣ */
	set_app_printf(1);
	set_dbg_printf(1);
	set_sync_printf(1);

	/* AES256 ���ܳ�ʼ�� */
	aes256_key_init();
	set_aes256(1);			//���� AES 256 ���ܷ�ʽ
	
	/* ����������ļ� */
	get_config_for_json();

	/* ��ʼ��socket */
    init_client_socket((unsigned short)sys_cfg.my_port);

	memcpy(sys_cfg.now_version, CLIENT_VERSION, sizeof(sys_cfg.now_version));
	printf("\nclient version :         %s\n\n", sys_cfg.now_version);
	
	/* ��ʼ���������ݶ��� */
	init_net_data_list();

	/* ����udp ͨ���߳� */
	init_network_pthread();

	/* ��ʼ�������� */
	client_command_init();
}


