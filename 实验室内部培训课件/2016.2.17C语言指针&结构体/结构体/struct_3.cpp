/*
时间：2016年2月16日22:45:40
作用：结构体中指针的运用
描述：普通变量与结构体指针变量作为函数传参的问题：指针变量用->;普通变量用.的方式
*/

# include <stdio.h>
# include <string.h>

struct Student
{	
	int sid;
	char name[200];
	int age;
}; //分号不能省

void f(struct Student * pst);   //赋值
void g(struct Student st);      //输出
void g2(struct Student *pst);   //输出

int main(void)
{
	struct Student st;  //已经为st分配好了内存

	f(&st);
//	g(st);   //这种方式耗内存 耗时间 不推荐，发送了208个字节  因为把整个把整体的变量都发送过去了
	g2(&st);

	//printf("%d %s %d\n", st.sid, st.name, st.age);

	return 0;
}

//这种方式耗内存 耗时间 不推荐，发送了208个字节   因为st至少占208个字节
void g(struct Student st)
{
	printf("%d %s %d\n", st.sid, st.name, st.age);	
}

//显示了指针的强大
void g2(struct Student *pst)
{
	printf("%d %s %d\n", pst->sid, pst->name, pst->age);	//当pst是指针变量的时候就用->的方式
}


//赋值函数，发送了4个字节
void f(struct Student * pst)
{
	(*pst).sid = 99;
	strcpy(pst->name, "zhangsan");
	pst->age = 22;
}