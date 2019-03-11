
#ifndef __CLIENT_AFF_H_
#define __CLIENT_AFF_H_

#ifdef __CLIENT_AFF_C_
#define __CLIENT_AFF_EXT_
#else
#define __CLIENT_AFF_EXT_	extern
#endif

#include "all.h"


/* ���յ����������� */
struct client_aff_info{
	int aff;		//��������
	void (*aff_fun)(int sockfd, char *buf, int len, struct sockaddr_in *clientaddr);
};

__CLIENT_AFF_EXT_ struct client_aff_info client_aff_table[];


#endif

