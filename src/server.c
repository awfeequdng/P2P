#define __SERVER_C_

#include "all.h"
#include "server.h"

void server_command_init(void);

int main(int argc, char *argv[])
{
	
	char command[1024];
	char *str;

	/* ������ӡ��Ϣ */
	set_app_printf(1);
	set_dbg_printf(1);
	set_sync_printf(1);
	
	/* ������ϣ�� */
	h_table = create_hashtable(16, hashfromkey, equalkeys);
	if(NULL == h_table){
		app_printf("create_hashtable err \n");
		return -1;
	}

	/* �����������ļ���ʼ�� */
	server_conf_init();

	/* �������ڵ���Ϣ */
	server_list_init();

	/* ��ʱ�������ʼ�� */
	my_timer_init();

	/* AES256 ���� */
//	aes256_key_init();
	set_aes256(P2P_DEF_AES256);			//���� AES 256 ���ܷ�ʽ

	/* ���ݿ��ʼ�� */
	service_sql_init();
//	__mysql_debug();	/* ���� */

	/* ������socket ��ʼ�� */
	init_server_sock_pthread();

	/* ��ʼ�������� */
	server_command_init();

	/* TCP ���߳� */
	server_updata_init();

//	__mysql_debug();

	while(1)
	{
        str = fgets(command, 1024, stdin);
		if(str != NULL)
		{
	        command_format(command);
			printf("server @: $ ");
		}

	}
	
	return 0;
}


