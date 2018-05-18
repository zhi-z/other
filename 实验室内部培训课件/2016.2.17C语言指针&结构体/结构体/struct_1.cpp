/*
时间：2016年2月16日22:02:19
作用：结构体的使用
*/
# include <stdio.h>
# include <string.h>  //strcpy(st.name, "lisi");需要

struct Student
{	
	int sid;      //有三个成员
	char name[200];
	int age;
}; //分号不能省

int main(void)
{
	struct Student st = {1000, "zhangsan", 20};
	printf("%d  %s  %d\n", st.sid, st.name, st.age);

	st.sid = 99;
	//st.name = "lisi";  //error
	strcpy(st.name, "lisi");//注意区别
	st.age = 22;
	printf("%d  %s  %d\n", st.sid, st.name, st.age);  //要使用结构体的成员，格式：变量名.成员名

	//printf("%d %s %d\n", st);  //error
	return 0;
}