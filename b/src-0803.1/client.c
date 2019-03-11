
#define __CLIENT_C_

#include "all.h"

#include "client.h"


/* 同步接收 */
void *my_thread_recv(void *pdata)
{
	int ret;
	char buf[1024];
	struct sockaddr_in recv_addr;
	while(1)
	{
		ret = recv_node_data(buf, 1024, &recv_addr, -1);
		if(ret < 0)
		{
			printf("my recv is err \r\n");
		}else{
			char *recv_data;
			struct check_head *head = (struct check_head *)buf;
			struct proto_c_send_data *recv_proto = (struct proto_c_send_data *)(buf + sizeof(struct check_head));
			recv_data = (char *)(buf + sizeof(struct check_head) + sizeof(struct proto_c_send_data));

			printf("recv from %s 's %s data : [%s] \r\n", recv_proto->src_name, 
												((head->affairs == _aff_client_p2p_data_)?"stun":"turn"),
												recv_data);
		}
	}
}

void my_thread_init(void)
{
    pthread_t p2p_thread;
    pthread_attr_t p2p_thread_attr;

	/* 启动一个线程去尝试P2P 呼叫 */
	pthread_attr_init(&p2p_thread_attr);		//初始化进程属性
	pthread_attr_setdetachstate(&p2p_thread_attr, PTHREAD_CREATE_DETACHED);
	if (pthread_create(&p2p_thread, &p2p_thread_attr, my_thread_recv, NULL) < 0)
	{
		perror("pthread_create");
	}
}

int main(int argc, char *argv[])
{
	char command[1024];
	char *str;

	p2p_client_init();

	set_recv_list(1);
	my_thread_init();

	while(1)
	{
        str = fgets(command, 1024, stdin);
		if(str != NULL)
		{
	        command_format(command);
			printf("client @%s: $ ", sys_cfg.myname);
		}

	}
	return 0;
}


