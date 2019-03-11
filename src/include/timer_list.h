
#ifndef __TIMER_LIST_H__
#define __TIMER_LIST_H__

#include "my_list.h"

struct my_timer{
	int sec;		//����
	void *arg;		//����
	void (*handle)(void *pdata);	//������

	struct list_head list;	/* ���� */
};


void my_timer_init(void);


#endif

