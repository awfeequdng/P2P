
#define __CLIENT_C_

#include "all.h"

#include "client.h"




void client_command_init(void);

int main(int argc, char *argv[])
{
	char command[1024];
	char *str;

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

//	__debug_aff_net();
	
	while(1)
	{
        str = fgets(command, 1024, stdin);
		if(str != NULL)
		{
	        command_format(command);
			printf("client @: $ ");
		}

	}
	return 0;
}


