

#ifndef __SERVER_LIST_H__
#define __SERVER_LIST_H__

#include "all.h"
//#include "server.h"
//#include "my_list.h"

#include "my_list.h"

struct server_node{
	char name[100];		//������������
	int port;			//�������Ķ˿ں�
	char ip[20];		//��������IP
	
	struct list_head list;	/* ���� */
};

#endif


