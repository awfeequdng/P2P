
/*******************************************************
����ͨ��
********************************************************/

#include "all.h"
#include "client.h"


/*
ͨ��
*/
void sound_talk(listen_t *listen)
{
	/* ����һ��¼�����ͺ��� */
	
	
	while(1)
	{
		
	}
}



/*----------------------------------------------------------------*/
/*
����һ���߳�������˴�ͨ��
*/
void *sound_talk_phread(listen_t *listen)
{
	int ret;
	char buf[1204];
	struct sockaddr_in clientaddr;

	ret = 0;
	/* ���ûص����� */
	if(client_inface.listen_audio != NULL)
		client_inface.listen_audio(listen);

	/* �ظ��Է��ܾ�ͨ�� */
	if(ret != 0)
	{
		
		listen_close(listen);
		return 0;
	}

	/* �ظ��Է������Կ�ʼͨ���� */

	/* ��ʽͨ�� */
	sound_talk(listen);

	listen_close(listen);
	return 0;
}




/*---------------------------------------------------------*/
/*
������������ͨ������
*/



/*
����һ���߳�������˴�ͨ��
*/
void *sound_talk_to_phread(void *pdata)
{
	int ret;
	char buf[1204];
	struct sockaddr_in clientaddr;
	listen_t *listen;
	listen = (struct listen *)pdata;

	/* �������listen  */
	listen_start(listen);

	/* ���ͻỰ���� */
	

	/* �ȴ��Է�ȷ�� */

	/* ��ʽͨ�� */
	sound_talk(listen);

	listen_close(listen);
	return 0;
}



void sound_talk_to(char *name)
{
	listen_t *listen;
	listen = create_listen(name, _proto_c_audio_);
	listen_pthread(listen, sound_talk_to_phread);
}



