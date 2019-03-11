/************************************************
	agent ���Э��Լ��
************************************************/

#ifndef __AGENT_PROTO_H_
#define __AGENT_PROTO_H_

/* �ͻ����������͵� */
#define _aff_client_sync_		0x0	/* �������� */
#define _aff_client_login_		0x1	/* ��¼���� */
#define _aff_client_updata_ack_		0x2	/* �ͻ�������Ӧ������ */
#define _aff_client_get_client_		0x3	/* ��ȡĳ���ͻ��˵���Ϣ */
#define _aff_client_send_data_		0x4	/* ���������ת�� */
#define _aff_client_send_data_ack_	0x5	/* ���������ת������Ӧ�� */


/* �������������͵� */
#define _aff_server_sync_		0x100
#define _aff_server_updata_		0x101	/* ������ϣ���ͻ������� */
#define _aff_server_relogin_	0x102	/* ������Ҫ��ͻ������µ�¼ */
#define _aff_server_login_ack_	0x103	/* ������Ӧ���½��Ϣ */
#define _aff_server_get_client_ack_	0x104	/* ������Ӧ��ͻ�����Ϣ */


#define _aff_client_device_		0x1001	/* ����ʱ�ϴ� */
#define _aff_client_system_		0x1002	/* �豸�Լ� 30 ���� */
#define _aff_client_on_off_		0x1003	/* ���ػ���¼ */
#define _aff_client_download_	0x1004	/* ������־ */
#define _aff_client_net_flow_	0x1005	/* ������¼ */
#define _aff_client_ap_drop_	0x1006	/* AP ά�� */







struct proto_c_login{
	char version[20];		//�汾��
};

struct proto_s_login_ack{
	char ack;				//Ӧ��״̬
};


/* ��ȡĳ���ͻ�����Ϣ */
struct proto_c_get_lient{
	char name[USER_NAME_LEN];
};

/* ������Ӧ��ͻ�����Ϣ */
struct proto_s_client_info{
	char status;		//����״̬
	char name[USER_NAME_LEN];
	char ip[IP_STRING_MAX];
	char port[IP_STRING_MAX];
};

/* �ͻ�������ת������ */
struct proto_c_send_data{
	char src_name[USER_NAME_LEN];
	char dest_name[USER_NAME_LEN];
};

#endif

