/*
作用：证明a[0] 到a[1] 所占的字节数
*/

# include <stdio.h>

int main(void)
{
	double * p;
	double x = 66.6;

	p = &x;  //x占8个字节 1个字节是8位, 1个字节一个地址

	double arr[3] = {1.1, 2.2, 3.3};
	double * q;

	q = &arr[0];
	printf("%p\n", q);  //%p实际就是以十六进制输出
	q = &arr[1];
	printf("%p\n", q);  

	return 0;
}