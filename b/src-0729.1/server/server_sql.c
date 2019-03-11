
/***********************************************************
文件名	:	server_sql.c
作者		:	Faker
版本号	:	1.0
日期		:	2015.05.13

说明:
		server 端数据库接口

***********************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include "mysql.h"  //我的机器上该文件在/usr/include/mysql下


#include "all.h"
#include "server.h"



#define SELECT_QUERY "select * from mytable where name like '%s'"
//#define ALL_QUERY "select * from mytable;"

MYSQL mysql, *sock; //定义数据连接句柄


/*******************************************
函数名:	service_sql_init
功能:		服务器数据库初始化
*********************************************/

int service_sql_init(void)
{
	mysql_init(&mysql);
	/* 连接上数据库 */
//	if(!(sock = mysql_real_connect(&mysql, "localhost", "root", "my-name-is?lza1205", "rt5350_user_list", 0, NULL, 0))){
	if(!(sock = mysql_real_connect(&mysql, "localhost", "root", "123456", "p2p_user_list", 0, NULL, 0))){
		fprintf(stderr, "Couldn't connect to engine!\n%s\n\n", mysql_error(&mysql));
		perror("");
		exit(1);
	}

	sql_table_update_all("device", "is_online", "offline");
	sql_table_update_all("network", "is_online", "offline");
	sql_table_update_all("system", "is_online", "offline");
	sql_table_update_all("other", "is_online", "offline");

	app_printf("mysql init is ok \r\n");
	return 0;
}


/*******************************************
函数名:	service_sql_select
功能:		根据name 从服务器中找出passwd
*********************************************/

int service_sql_select(char *name, char *passwd)
{
	MYSQL_RES	*res;	//查询结果集
//	MYSQL_FIELD	*fd;	//包含字段信息
	MYSQL_ROW	row;	//存放一行查询结果的字符串数组
	char qbuf[160];		//存放查询 sql 语句字符串

	sprintf(qbuf, SELECT_QUERY, name);
	printf("qbuf is %s\n", qbuf);
	/* 查询数据库 */
	if(mysql_query(sock, qbuf)){
		fprintf(stderr, "Query failed (%s)\n", mysql_error(sock));
		return -1;
	}

	if(!(res = mysql_store_result(sock))){
		fprintf(stderr, "Couldn't get result from %s\n", mysql_error(sock));
		return -1;
	}

	printf("number of fields returned : %d\n", mysql_num_fields(res));
	while((row = mysql_fetch_row(res))){
		printf("Ther name #%s 's passwd is: %s\n", (((row[0]==NULL)&&(!strlen(row[0]))) ? "NULL" : row[0]),
													(((row[1]==NULL)&&(!strlen(row[1]))) ? "NULL" : row[1])); 
        puts( "query ok !\n" ) ; 
		strcpy(passwd, (((row[1]==NULL)&&(!strlen(row[1]))) ? "NULL" : row[1]));
		mysql_free_result(res);
		return 0;
	}

	mysql_free_result(res);
	return -1;
//    mysql_close(sock);
}


/* 释放结果集 */
void __mysql_free_result(void)
{
	MYSQL_RES	*res;	//查询结果集
	do
	{
	  if (!(res= mysql_store_result(sock)))
	  {
			return;
	  }
	  mysql_free_result(res);
	
	} while (!mysql_next_result(sock));

}


#define UPDATE_STRING_QUERY "update mytable set %s=\'%s\' where name=\'%s\'"
#define UPDATE_INT_QUERY "update mytable set %s=%d where name=\'%s\'"

/* 根据名字修改数据库中的某项 */
int server_sql_update_string(char *name, char *key, char *val)
{
	char qbuf[160];		//存放查询 sql 语句字符串

	sprintf(qbuf, UPDATE_STRING_QUERY, key, val, name);
	printf("qbuf is %s\n", qbuf);
	/* 查询数据库 */
	if(mysql_query(sock, qbuf)){
		fprintf(stderr, "Query failed (%s)\n", mysql_error(sock));
		return -1;
	}
	return 0;
}

int server_sql_update_int(char *name, char *key, unsigned int val)
{
	char qbuf[160];		//存放查询 sql 语句字符串

	sprintf(qbuf, UPDATE_INT_QUERY, key, val, name);
	printf("qbuf is %s\n", qbuf);
	/* 查询数据库 */
	if(mysql_query(sock, qbuf)){
		fprintf(stderr, "Query failed (%s)\n", mysql_error(sock));
		return -1;
	}
	return 0;
}











/*****************************************************************************/

/*
	下面的函数可以指定某个表格
*/



#define UPDATE_TABLE_STRING_QUERY "update %s set %s=\'%s\' where %s=\'%s\'"
#define UPDATE_TABLE_INT_QUERY "update %s set %s=%d where %s='%s'"

#define UPDATE_TABLE_ALL_QUERY "update %s set %s=\'%s\'"


/*
	table :表名
	name: 索引
	key:	修改的项
	val:	修改值
*/
/* 根据名字修改数据库中的某项 */
int sql_table_update_string(char *table, char *name_key, char *name_val, char *key, char *val)
{
	char qbuf[160];		//存放查询 sql 语句字符串

	sprintf(qbuf, UPDATE_TABLE_STRING_QUERY, table, key, val, name_key, name_val);
	
	/* 查询数据库 */
	if(mysql_query(sock, qbuf)){
		printf("qbuf is %s\n", qbuf);
		fprintf(stderr, "Query failed (%s)\n", mysql_error(sock));
		return -1;
	}
	return 0;
}

int sql_table_update_int(char *table, char *name_key, char *name_val, char *key, int val)
{
	char qbuf[160];		//存放查询 sql 语句字符串

	sprintf(qbuf, UPDATE_TABLE_INT_QUERY, table, key, val, name_key, name_val);
	
	/* 查询数据库 */
	if(mysql_query(sock, qbuf)){
		printf("qbuf is %s\n", qbuf);
		fprintf(stderr, "Query failed (%s)\n", mysql_error(sock));
		return -1;
	}
	return 0;
}


/* 修改所有行的某项 */
int sql_table_update_all(char *table, char *key, char *val)
{
	char qbuf[160];		//存放查询 sql 语句字符串

	sprintf(qbuf, UPDATE_TABLE_ALL_QUERY, table, key, val);
	
	/* 查询数据库 */
	if(mysql_query(sock, qbuf)){
		printf("qbuf is %s\n", qbuf);
		fprintf(stderr, "Query failed (%s)\n", mysql_error(sock));
		return -1;
	}
	return 0;
}


/* 查询数据库中是否有这个元素 */
#define SELECT_STRING_QUERY	"select * from %s where %s like \'%s\'"
int sql_table_select_string(char *table, char *key, char *val)
{
	char qbuf[160];		//存放查询 sql 语句字符串
	MYSQL_RES	*res;	//查询结果集
//	MYSQL_FIELD *fd;	//包含字段信息
	MYSQL_ROW	row;	//存放一行查询结果的字符串数组

	sprintf(qbuf, SELECT_STRING_QUERY, table, key, val);
	//printf("qbuf is %s\n", qbuf);
	/* 查询数据库 */
	if(mysql_query(sock, qbuf)){
		fprintf(stderr, "Query failed (%s)\n", mysql_error(sock));
		return -1;
	}
	
	if(!(res = mysql_store_result(sock))){
		fprintf(stderr, "Couldn't get result from %s\n", mysql_error(sock));
		return -1;
	}

	//printf("number of fields returned : %d\n", mysql_num_fields(res));
	while((row = mysql_fetch_row(res))){

		mysql_free_result(res);
		return 0;
	}

	mysql_free_result(res);
	return -1;
}


/* 往数据库中插入元素 */
#define INSERT_STRING_QUERY	"insert into %s (%s) values (\'%s\')"
int sql_table_insert(char *table, char *key, char *val)
{
	char qbuf[160];		//存放查询 sql 语句字符串

	sprintf(qbuf, INSERT_STRING_QUERY, table, key, val);
	
	/* 查询数据库 */
	if(mysql_query(sock, qbuf)){
		printf("qbuf is %s\n", qbuf);
		fprintf(stderr, "Query failed (%s)\n", mysql_error(sock));
		return -1;
	}
	return 0;
}

#define SQL_SELECT_QUERY "select %s from %s where name like '%s'"

/* 根据名字找到某个值找到某个值 */
int sql_table_select(char *table, char *name, char *key, char *my_buf)
{
	MYSQL_RES	*res;	//查询结果集
//	MYSQL_FIELD	*fd;	//包含字段信息
	MYSQL_ROW	row;	//存放一行查询结果的字符串数组
	char qbuf[160];		//存放查询 sql 语句字符串

	sprintf(qbuf, SQL_SELECT_QUERY, key, table, name);
	printf("qbuf is %s\n", qbuf);
	/* 查询数据库 */
	if(mysql_query(sock, qbuf)){
		fprintf(stderr, "Query failed (%s)\n", mysql_error(sock));
		return -1;
	}

	if(!(res = mysql_store_result(sock))){
		fprintf(stderr, "Couldn't get result from %s\n", mysql_error(sock));
		return -1;
	}

	printf("number of fields returned : %d\n", mysql_num_fields(res));
	while((row = mysql_fetch_row(res))){		
		int n;
		strcpy(my_buf, (((row[0]==NULL)&&(!strlen(row[0]))) ? "NULL" : row[0]));
		/* 最后面有个空格?????? */
		n = strlen(my_buf);
//		my_buf[n - 2] = 0;
//		strcpy(val, (((row[1]==NULL)&&(!strlen(row[1]))) ? "NULL" : row[1]));
		mysql_free_result(res);
		 puts( "query ok !\n" ) ; 
		return 0;
	}

	mysql_free_result(res);
	return -1;
//    mysql_close(sock);
}


/*******************************************
函数名:	login_passwd
功能:		对登录密码进行校验
*********************************************/

int login_passwd(char *name, char *passwd)
{
	char passq[USER_NAME_LEN];
//	int ret;
	
	if(service_sql_select(name, passq)!= 0)
		return -1;
//	app_printf("passq is %s",passq);
	if (strcmp(passwd, passq) == 0)
		return 0;
	else
		return -1;
}





void __mysql_debug(void)
{
	int ret;
	char tmp_buf[100];

	
	sql_table_select("device", "YTVY-00:16:3E:00:19:B4", "version", tmp_buf);
	printf("[sql_table_select is %s]\r\n", tmp_buf);

	server_sql_update_string("lza3", "passwd", "123456");

	server_sql_update_string("lza1", "passwd", "654321");

#if 1
	ret = sql_table_select_string("device", "code", "qwer");
	if(ret != 0)
		printf("sql_table_select_string err \r\n");
#endif

	ret = sql_table_insert("device", "code", "faker");
	if(ret != 0)
		printf("sql_table_insert err \r\n");

	ret = sql_table_select_string("device", "code", "faker");
	if(ret != 0)
		printf("sql_table_select_string err \r\n");

	sql_table_update_string("device", "name", "faker", "mac", "aabbccdd");
	if(ret != 0)
		printf("sql_table_update_string err \r\n");

	ret = sql_table_select_string("device", "code", "faker");
	if(ret != 0)
		printf("sql_table_select_string err \r\n");


}


