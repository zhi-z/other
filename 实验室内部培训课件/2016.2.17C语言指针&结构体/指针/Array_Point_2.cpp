/*

作用：指针和数组的作用
时间：2016年2月16日17:05:30
描述：把数组输出（输出封装成一个函数）

如何通过被调函数修改主调函数中一维数组的内容
	两个参数
		存放数组首元素的指针变量
		存放数组元素长度的整型变量
*/
# include <stdio.h>

void Show_Array(int * p, int len)
{
	int i = 0;

	for (i=0; i<len; ++i)
		printf("%d\n", p[i]);

	//p[2] = -1;  //p[0] == *p   p[2] == *(p+2) == *(a+2) == a[2]
	//p[i]就是主函数的a[i]
}

int main(void)
{
	int a[5] = {1,2,3,4,5};
	
	Show_Array(a, 5);  //a等价于&a[0], &a[0]本身就是int *类型

	//printf("%d\n", a[2]);

	return 0;
}