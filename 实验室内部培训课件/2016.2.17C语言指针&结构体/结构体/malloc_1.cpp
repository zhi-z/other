/*
	时间：2016年2月17日08:48:01
	作用：malloc（）动态内存的分配。
*/
# include <stdio.h>
# include <malloc.h>

int main(void)
{
	int a[5] = {4, 10, 2, 8, 6};
	
	int len;
	printf("请输入你需要分配的数组的长度: len = ");
	scanf("%d", &len);
	int * pArr = (int *)malloc(sizeof(int) * len);  //第一：malloc函数只能返回第一个字节的地址，这是规定   第二：请求操作系统分配相应的地址  
	//然后把前四个字节的地址赋给了PArr
//	*pArr = 4;  //类似于 a[0] = 4;
//	pArr[1] = 10; //类似于a[1] = 10;
//	printf("%d %d\n", *pArr, pArr[1]);

	//我们可以把pArr当做一个普通数组来使用
	for (int i=0; i<len; ++i)
		scanf("%d", &pArr[i]);

	for (i=0; i<len; ++i)
		printf("%d\n", *(pArr+i));

	
	free(pArr);  //把pArr所代表的动态分配的20个字节的内存释放

	return 0;
}