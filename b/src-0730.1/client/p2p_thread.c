
/*
P2P��صĺ���
*/

/********************************************************
������:	p2p_thread_init
����:		��ʼ��ͬ���߳�
********************************************************/

void p2p_thread_init(void)
{
    pthread_t p2p_thread;
    pthread_attr_t p2p_thread_attr;

	/* ����һ���߳�ȥ����P2P ���� */
	pthread_attr_init(&p2p_thread_attr);		//��ʼ����������
	pthread_attr_setdetachstate(&p2p_thread_attr, PTHREAD_CREATE_DETACHED);
	if (pthread_create(&p2p_thread, &p2p_thread_attr, p2p_thread_sync, NULL) < 0)
	{
		perror("pthread_create");
	}
}


