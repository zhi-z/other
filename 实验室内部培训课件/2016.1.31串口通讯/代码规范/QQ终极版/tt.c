#include "common.h"

int main()
{
	int QQ;
	char p[32];
	srand((unsigned int)time(NULL));//设置随机数种子

	QQ = rand() % 999999; //产生6位数的QQ号码
	sprintf(p, "%d", QQ);
	printf("p=%s\n", p);


	return;

}
