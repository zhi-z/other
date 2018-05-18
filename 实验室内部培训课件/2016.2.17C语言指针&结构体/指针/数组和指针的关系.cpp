# include <stdio.h>

int main (void)
{
    int a[5];

	printf ("%#X\n",&a[0]);
	printf ("%#X\n", a);

	return 0;
}

/*
    输出的结果：
				0X18FF34
				0X18FF34
				Press any key to continue

  总结：一维数组名是个指常量
       它存放的是一维数组第一个元素的地址


*/