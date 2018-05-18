#include "common.h"
#include <termios.h>

/*
   int tcgetattr(int fd, struct termios *termios_p);

   int tcsetattr(int fd, int optional_actions,
   const struct termios *termios_p);
   void cfmakeraw(struct termios *termios_p);
   TCSANOW
 */

int main()
{
	system("clear");
	struct termios oldter,newter;
	tcgetattr(0,&oldter);

	char buf[3];

	while(1)
	{
		cfmakeraw(&newter);
		tcsetattr(0,TCSANOW,&newter);
		bzero(buf,3);
		read(0,buf,3);
		tcsetattr(0,TCSANOW,&oldter);

		int i, j;
		char *a[] = {"esc:", "le:", "ri:"};
		for(j=0;j<3;j++)
		{
			printf("%s\n", a[j]);
			for(i=0;i<3;i++)
			{
				printf("%d ",buf[i]);
			}
		}
		printf("\n");
	}
}	
