/***********************************************************
�ļ���	:	command_client.c
����		:	Faker
�汾��	:	1.0
����		:	2015.05.15

˵��:	
	�ͻ���֧�ֵ�������

***********************************************************/



#include "all.h"
#include "client.h"
#include "command.h"

extern char *gp_comman_tag[10];		//�����в���
extern int g_comman_num;		//�����в�������



/********************************************
������: printf_info
����:		��ӡ�������Ϣ
*********************************************/

void printf_info(void)
{
	app_printf("version          %s\r\n", sys_cfg.now_version);
	app_printf("app name         %s\r\n", sys_cfg.myname);
	app_printf("passwd           %d\r\n", sys_cfg.passwd);
	
	app_printf("wan ip           %s\r\n", sys_cfg.wan_ip);
	app_printf("wan port         %d\r\n", sys_cfg.wan_port);
	
	app_printf("lan ip           %s\r\n", sys_cfg.lan_ip);
	app_printf("lan port         %d\r\n", sys_cfg.lan_port);
	
	app_printf("server ip        %s\r\n", sys_cfg.serverip);
	app_printf("server port      %d\r\n", sys_cfg.ser_port);
	
}


/********************************************
������: __help
����:		help
*********************************************/

static void __help(void)
{
	int i;
	app_printf("/------------------- Welcome to Faks -------------------/\r\n");
	app_printf("You can enter the following command : \r\n");

	for(i = 0; gt_comman[i].name != NULL; i++)	//һ�����ȶ�
	{
		app_printf("[%d]%s       :   %s \r\n", i, gt_comman[i].name, gt_comman[i].help);
	}

	app_printf("/------------------- --------------- -------------------/\r\n");
}



/********************************************
������: __get_info
����:		call ĳ�ͻ���
*********************************************/

static void __get_info(void)
{
	char buf[8];
	__str_to_buf(buf, gp_comman_tag[1]);
	get_client_info(buf);
}


void __send(void)
{
	char buf[8];
	__str_to_buf(buf, gp_comman_tag[1]);
	sendto_node_unreliable(buf, _proto_c_msg_, gp_comman_tag[2], strlen(gp_comman_tag[2]) + 1);
}

/* �������� */
void ___send(void)
{
	char buf[8];
	__str_to_buf(buf, gp_comman_tag[1]);
	sendto_node_reliable(buf, _proto_c_msg_, gp_comman_tag[2], strlen(gp_comman_tag[2]) + 1, 10);
}


/********************************************
������:	__test
����:		����
********************************************/
void __test(void)
{
	int ret;
	char buf[1024];
	struct sockaddr_in recv_addr;
	while(1)
	{
		char buf[8];
		__str_to_buf(buf, "1234567890abcdef");
		if(sendto_node_unreliable(buf, _proto_c_msg_, "qwerdf", 7) < 0)
		{
			printf("____!!!! sendto_node_unreliable qwer err \r\n");
		}else{
			printf("____**** sendto_node_unreliable qwer ok \r\n");
		}
		if(sendto_node_reliable(buf, _proto_c_msg_, "qwerdf", 7, 10) < 0)
		{
			printf("____!!!! sendto_node_reliable qwer err \r\n");
		}else{
			printf("____**** sendto_node_reliable qwer ok \r\n");
		}

		__str_to_buf(buf, gp_comman_tag[1]);
		if(sendto_node_unreliable(buf, _proto_c_msg_, "qwerdf", 7) < 0)
		{
			printf("____!!!! sendto_node_unreliable %s err \r\n", gp_comman_tag[1]);
		}else{
			printf("____**** sendto_node_unreliable %s ok \r\n", gp_comman_tag[1]);
		}
		if(sendto_node_reliable(buf, _proto_c_msg_, "qwerdf", 7, 10) < 0)
		{
			printf("____!!!! sendto_node_reliable %s err \r\n", gp_comman_tag[1]);
		}else{
			printf("____**** sendto_node_reliable %s ok \r\n", gp_comman_tag[1]);
		}
#if 0
		ret = recv_node_data(buf, 1024, &recv_addr, 5);
		if(ret < 0)
		{
			printf("____!!!! my recv_node_data is err \r\n");
		}else{
			printf("____**** my_thread_recv is ok \r\n");
		}
#endif
	}
}


extern void printf_client_list(void);
extern void printf_p2p_test_node_list(void);
extern void printf_p2p_sync_node_list(void);
extern void __printf_net_data_list(void);

/********************************************
�ͻ���������
********************************************/
struct command_t gt_comman_client[] = {
	{
		.name 		= "info",
		.com_fun 	= printf_info,
		.tag_num	= 0,
		.tag_p		= NULL,
		.help 		= "show my info",
	},

	{
		.name		= "help",
		.com_fun	= __help,
		.tag_num	= 0,
		.tag_p		= NULL,
		.help 		= "show help",
	},

	{
		.name		= "client_list",
		.com_fun	= printf_client_list,
		.tag_num	= 0,
		.tag_p		= NULL,
		.help		= "show client listt",

	},
	
	{
		.name		= "p2p_test_list",
		.com_fun	= printf_p2p_test_node_list,
		.tag_num	= 0,
		.tag_p		= NULL,
		.help		= "show p2p test list",

	},	
	{
		.name		= "p2p_sync_list",
		.com_fun	= printf_p2p_sync_node_list,
		.tag_num	= 0,
		.tag_p		= NULL,
		.help		= "show p2p sync list",

	},	
	{
		.name		= "?",
		.com_fun	= __help,
		.tag_num	= 0,
		.tag_p		= NULL,
		.help 		= "show help",
	},

	{
		.name		= "call",
		.com_fun	= __get_info,
		.tag_num	= 0,
		.tag_p		= NULL,
		.help		= "you can call client",
	},

	{
		.name		= "send",
		.com_fun	= __send,
		.tag_num	= 0,
		.tag_p		= NULL,
		.help		= "you can send msg to client",
	},

	{
		.name		= "_send",
		.com_fun	= ___send,
		.tag_num	= 0,
		.tag_p		= NULL,
		.help		= "you can send msg to client",
	},

	{
		.name		= "net_data_list",
		.com_fun	= __printf_net_data_list,
		.tag_num	= 0,
		.tag_p		= NULL,
		.help		= "show network data list",
	},

	{
		.name		= "test",
		.com_fun	= __test,
		.tag_num	= 0,
		.tag_p		= NULL,
		.help		= "test",
	},

	{
		.name		= NULL,
		.com_fun 	= NULL,
		.tag_num	= 0,
		.tag_p		= NULL,
		.help 		= NULL,
	},
};


void client_command_init(void)
{
	gt_comman = gt_comman_client;
}


