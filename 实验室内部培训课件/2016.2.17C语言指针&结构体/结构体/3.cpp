/*
时间：2016年2月16日21:25:22
作用：指针类型的变量如何跨函数修改变量
*/
# include <stdio.h>

void f(int ** q);

int main(void)
{
	int i = 9;
	int * p = &i;// int  *p;  p = &i;

	printf("%p\n", p);
	f(&p);
	printf("%p\n", p);

	return 0;
}

void f(int ** q)
{
	*q = (int *)0xFFFFFFFF;  //向下转型
}