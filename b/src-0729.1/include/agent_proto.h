/************************************************
	agent 相关协议约定
************************************************/

#ifndef __AGENT_PROTO_H_
#define __AGENT_PROTO_H_

/* 客户端主动发送的 */
#define _aff_client_sync_		0x0	/* 心跳事务 */
#define _aff_client_login_		0x1	/* 登录事务 */
#define _aff_client_updata_ack_		0x2	/* 客户端升级应答事务 */
#define _aff_client_get_client_		0x3	/* 获取某个客户端的信息 */
#define _aff_client_send_data_		0x4	/* 请求服务器转发 */
#define _aff_client_send_data_ack_	0x5	/* 请求服务器转发数据应答 */


/* 服务器主动发送的 */
#define _aff_server_sync_		0x100
#define _aff_server_updata_		0x101	/* 服务器希望客户端升级 */
#define _aff_server_relogin_	0x102	/* 服务器要求客户端重新登录 */
#define _aff_server_login_ack_	0x103	/* 服务器应答登陆信息 */
#define _aff_server_get_client_ack_	0x104	/* 服务器应答客户端信息 */


#define _aff_client_device_		0x1001	/* 开机时上传 */
#define _aff_client_system_		0x1002	/* 设备自检 30 分钟 */
#define _aff_client_on_off_		0x1003	/* 开关机记录 */
#define _aff_client_download_	0x1004	/* 升级日志 */
#define _aff_client_net_flow_	0x1005	/* 流量记录 */
#define _aff_client_ap_drop_	0x1006	/* AP 维护 */







struct proto_c_login{
	char version[20];		//版本号
};

struct proto_s_login_ack{
	char ack;				//应答状态
};


/* 获取某个客户端信息 */
struct proto_c_get_lient{
	char name[USER_NAME_LEN];
};

/* 服务器应答客户端信息 */
struct proto_s_client_info{
	char status;		//在线状态
	char name[USER_NAME_LEN];
	char ip[IP_STRING_MAX];
	char port[IP_STRING_MAX];
};

/* 客户端请求转发数据 */
struct proto_c_send_data{
	char src_name[USER_NAME_LEN];
	char dest_name[USER_NAME_LEN];
};

#endif

