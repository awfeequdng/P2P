
/*******************************************************
音频文件传输
*******************************************************/
#include "all.h"
#include "client.h"


void sound_file(listen_t *listen)
{	
	char buf[4096];
	sound_head_t *s_head;

	int ret;

	printf("fopen \r\n");

	FILE *fp = fopen("sound_file", "w+");
	if(fp == NULL)
	{
		perror("fopen ");
		return ;
	}
	
	printf("fopen \r\n");
	while(1)
	{
		ret = recv_sound_file(listen, buf, sizeof(buf));
		if(ret == -1)
		{
			printf("%p recv is err \r\n", listen);
			return ;		//时间到了退出
		}

		s_head = (sound_head_t *)(buf + sizeof(struct check_head) + sizeof(struct proto_c_send_data));

		/* 对方是否准备好了 */
		switch(s_head->type)
		{
			case _sound_file_data_:
			{
				fwrite(buf + sizeof(struct check_head) + sizeof(struct proto_c_send_data) + sizeof(sound_head_t), 
						ret - (sizeof(struct check_head) + sizeof(struct proto_c_send_data) + sizeof(sound_head_t)), 1, fp);
				break;
			}
			case _sound_file_end_:
			{
				printf("_sound_file_end_ \r\n");
				fclose(fp);
				return ;
			}
			printf("??????????????????? \r\n");
		}
	}
}


/* 启动一个线程来接收音频文件 */
void *sound_file_phread(listen_t *listen)
{
	int ret;
	char buf[1204];
	struct sockaddr_in clientaddr;

	ret = 0;
	/* 调用回调函数 */
	if(client_inface.listen_audio != NULL)
		client_inface.listen_audio(listen);

	printf("send_sound_file_ack \r\n");
	/* 回复对方拒绝通话 */
	if(ret != 0)
	{
		send_sound_file_ack(listen->name, 1);
		listen_close(listen);
		return 0;
	}

	/* 回复对方。可以开始通话了 */
	send_sound_file_ack(listen->name, 0);

	printf("sound_file \r\n");

	/* 正式接收音频文件 */
	sound_file(listen);

	listen_close(listen);
	return 0;
}



/*---------------------------------------------------------*/
/*
主动发起音频文件传输请求
*/


int sound_file_end_flg;
/*
从lasa 中获取音频数据并发送出去
*/
void sound_file_send(listen_t *listen)
{
	char buff[3760];

	sound_file_end_flg = 0;
	/* 录音初始化 */
	alsa_capture_init();

	printf("sound_file_send \r\n");
	while(1)
	{
		if(alsa_capture(buff) > 0)
			send_sound_file(listen->name, buff, sizeof(buff));

		/* 发送结束 */
		if(sound_file_end_flg == 1)
		{
			send_sound_file_end(listen->name);
			printf("send_sound_file_end \r\n");
			break;
		}
	}

	alsa_capture_close();
}

/*
启动一个线程来处理此次通话
*/
void *sound_file_to_phread(void *pdata)
{
	int ret;
	char buf[1204];
	struct sockaddr_in clientaddr;
	listen_t *listen;
	listen = (struct listen *)pdata;

	/* 开启这个listen  */
	listen_start(listen);

	printf("sound_file_request \r\n");

	/* 发送会话请求 */
	sound_file_request(listen->name);

	/* 等待对方确认 */
	if(recv_sound_file_ack(listen) != 0)
	{
		printf("recv_sound_file_noack \r\n");
		goto out1;
	}

	printf("sound_file_send \r\n");
	
	/* 正式发送音频文件 */
	sound_file_send(listen);

out1:
	listen_close(listen);
	return 0;
}



void sound_file_to(char *name)
{
	listen_t *listen;
	listen = create_listen(name, _proto_c_audio_);
	if(listen == NULL)
		printf("NULL !\r\n");
	listen_pthread(listen, sound_file_to_phread);
}



