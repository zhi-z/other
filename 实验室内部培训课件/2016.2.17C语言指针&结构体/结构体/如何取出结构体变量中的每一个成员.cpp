#include <stdio.h>


struct Student
{
    int age;
	float score;
	char sex;
};


int main (void)
{
	struct Student st = {80, 66.7, 'B'};  //初始化  定义的同时赋初值
	struct Student *pst = &st;
	pst->age = 78;  //第二种方式    
	st.score = 66.7f;  //第一种方式   66.7在C语言中默认是double类型，如果希望实现一个实数是float类型，则必须在末尾加f或F

	printf ("%d %f\n",st.age, pst->score );

    return 0;
}

