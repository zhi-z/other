#include "common.h"



//自定义错误处理函数
void my_err(const char *err_string, int line)
{
	fprintf(stderr, "line:%d ", line);
	perror(err_string);
}


/* 创建空表 */
Table *create_empty_table(int size)
{
	Table *HeadTable = NULL;
	HeadTable = (Table *)malloc(sizeof(Table));
	if (HeadTable == NULL)
	{
		ERRP("HeadTable malloc failed in func table_create()!\n", ERR1);
	}

	HeadTable->num = 0;
	HeadTable->size = size;
	HeadTable->head.data = NULL;
	HeadTable->head.next = &(HeadTable->head);
	HeadTable->head.prev = &(HeadTable->head);

	return HeadTable;

ERR1:
	return NULL;
}


/* 在尾部插入一个用户 */
void insert_to_tail(void *data, Table *tb)
{
	Node    *newNode = NULL;
	void    *newdata = NULL;

	if(data == NULL || tb == NULL)
	{
		my_err("insert_to_tail", __LINE__);
	}

	newNode = (Node *)malloc(sizeof(Node));
	if (newNode ==  NULL)
	{
		ERRP("newNode malloc failed in func insert_to_tail()!\n", ERR1);
	}


	newdata = malloc(tb->size);
	if (newdata == NULL)
	{
		ERRP("newNode malloc failed in func insert_to_tail()!\n", ERR2);
	}

	memmove(newdata, data, tb->size);
	newNode->data = newdata;

	newNode->next = &(tb->head);
	newNode->prev = tb->head.prev;
	tb->head.prev->next = newNode;
	tb->head.prev = newNode;
	tb->num++;

	return;
ERR2:
	free(newNode);
ERR1:
	return;
}













