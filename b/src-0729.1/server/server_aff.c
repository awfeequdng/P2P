
#define __SERVER_AFF_C_

#include "all.h"
#include "server.h"


void aff_clientt_login(int sockfd, char *buf, int len, struct sockaddr_in *clientaddr)
{
	struct value *currentuser = NULL;
	char version[50];

	struct check_head *head = (struct check_head *)buf;

	dbg_printf("name %s  \npasswd %s \n",
				head->name, head->passwd);
	dbg_printf("recv data : [%s]\n",buf + sizeof(struct check_head));

	/* 从哈希表中找到内容 */
	currentuser = insert_hashtable_value(head->name);

	if(NULL == currentuser)
	{
		goto server_err;
	}

	/* 获取登陆者信息 */
	sql_table_insert("device", "name", head->name);
	sql_table_update_string("device", "name", head->name, "passwd", head->name);
	sql_table_update_string("device", "name", head->name, "ip", inet_ntoa(clientaddr->sin_addr));
	sql_table_update_int("device", "name", head->name, "port", ntohs(clientaddr->sin_port));
	sql_table_update_int("device", "name", head->name,"login_cnt", LOGIN_CNT_INIT);

	/* 向客户端应答数据 */
	char send_buf[1204];
	int send_len;
	struct proto_s_login_ack ack;
	ack.ack = 0;

	head->affairs = _aff_server_login_ack_;
	memcpy(send_buf, head, sizeof(struct check_head));
	send_len = sizeof(struct check_head);
	memcpy(send_buf + send_len, &ack, sizeof(struct proto_s_login_ack));
	send_len += sizeof(struct proto_s_login_ack);

	p2p_sendto(sockfd, (char *)send_buf, send_len, 0, (struct sockaddr *)clientaddr,
		   sizeof(struct sockaddr));
	
server_err:
	return ;
}


void aff_client_sync(int sockfd, char *buf, int len, struct sockaddr_in *clientaddr)
{
	struct value *currentuser = NULL;

	struct check_head *head = (struct check_head *)buf;

	/* 从哈希表中找到内容 */
	currentuser = fine_value(head->name);

	/* 找不到。请求重新登录 */
	if(NULL == currentuser)
	{
		compages_head(head, _aff_server_relogin_);
		goto server_err;
	}

	/* 获取登陆者信息 */
//	currentuser->cli_info.type = head->type;
	__strcpy(currentuser->cli_info.name, head->name, USER_NAME_LEN);
	__strcpy(currentuser->cli_info.passwd, head->passwd, USER_NAME_LEN);
	__strcpy(currentuser->cli_info.ip, inet_ntoa(clientaddr->sin_addr), IP_STRING_MAX);
	currentuser->cli_info.port = ntohs(clientaddr->sin_port);
	currentuser->cli_info.login_cnt = LOGIN_CNT_INIT;

	return ;
server_err:
	p2p_sendto(sockfd, head, sizeof(struct check_head), 0, (struct sockaddr *)clientaddr,
			   sizeof(struct sockaddr));
	return ;

}


void aff_client_get_client(int sockfd, char *buf, int len, struct sockaddr_in *clientaddr)
{	
	int ret;
	char send_buf[1024];
	int send_len;
	struct proto_c_get_lient *proto;
	struct proto_s_client_info proto_ack;
	struct check_head *head = (struct check_head *)buf;

	printf("aff_client_get_client \r\n");

	proto = buf + sizeof(struct check_head);

	ret = sql_table_select_string("device", "name", proto->name);
	if(ret != 0)
	{
		printf("sql_table_select_string err %s \r\n", proto->name);
		proto_ack.status = 1;	/* 不在线 */
	}else{
		proto_ack.status = 0;	/* 在线 */
		sql_table_select("device", proto->name, "ip", (proto_ack.ip));
		sql_table_select("device", proto->name, "port", (proto_ack.port));
		strcpy((proto_ack.name), proto->name);
	}

	head->affairs = _aff_server_get_client_ack_;
	memcpy(send_buf, head, sizeof(struct check_head));
	memcpy(send_buf + sizeof(struct check_head), &proto_ack, sizeof(proto_ack));
	send_len = sizeof(proto_ack) + sizeof(struct check_head);

	
	p2p_sendto(sockfd, (char *)send_buf, send_len, 0, (struct sockaddr *)clientaddr,
		   sizeof(struct sockaddr));
	
}


/* 客户端请求服务器转发 */
void aff_client_send_data(int sockfd, char *buf, int len, struct sockaddr_in *clientaddr)
{
	int ret;
	char send_buf[1024];
	int send_len;
	char ip[IP_STRING_MAX], port[20];
	struct proto_c_get_lient *proto;
	struct proto_s_client_info proto_ack;
	struct check_head *head = (struct check_head *)buf;

	printf("aff_client_get_client \r\n");

	proto = buf + sizeof(struct check_head);

	ret = sql_table_select_string("device", "name", proto->name);
	if(ret != 0)
	{
		printf("sql_table_select_string err %s \r\n", proto->name);
		proto_ack.status = 1;	/* 不在线 */
		strcpy((proto_ack.name), proto->name);
		goto ack_loop;
	}

	sql_table_select("device", proto->name, "ip", (ip));
	sql_table_select("device", proto->name, "port", (port));
	
	struct sockaddr_in addr;

	addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(atoi(port));

	/* 转发出去 */
	p2p_sendto(sockfd, (char *)buf, len, 0, (struct sockaddr *)&addr,
		   sizeof(struct sockaddr));

	return ;
ack_loop:

	head->affairs = _aff_server_get_client_ack_;
	memcpy(send_buf, head, sizeof(struct check_head));
	memcpy(send_buf + sizeof(struct check_head), &proto_ack, sizeof(proto_ack));
	send_len = sizeof(proto_ack) + sizeof(struct check_head);

	
	p2p_sendto(sockfd, (char *)send_buf, send_len, 0, (struct sockaddr *)clientaddr,
		   sizeof(struct sockaddr));

	return ;
}

struct aff_info aff_table[] = {
	{
		_aff_client_login_,	/* 登录信号 */
		aff_clientt_login,
	},
	{
		_aff_client_sync_,	/* 同步信号 */
		aff_client_sync,
	},
	{
		_aff_client_get_client_,	/* 获取某个客户端*/
		aff_client_get_client,
	},
	{
		_aff_client_send_data_,
		aff_client_send_data,
	},
	{
		_aff_client_send_data_ack_,
		aff_client_send_data,
	},
	{
		0,
		NULL,
	},
};

