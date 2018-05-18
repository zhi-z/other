/***************************************************
* 时间：2015.2.8
* 名称：
* 作者：    
* 目的：
* 作用：超声波模块测距离
***************************************************/
#include"ultrasonic.h"


/***************************************************
* 时间：2015.2.8
* 函数名称：Delay15us
* 作用：延时函数
***************************************************/

void Delay15us()		//@11.0592MHz
{
	unsigned char i;

	i = 4;
	while (--i);
}

/***************************************************
* 时间：2015.2.8
* 函数名称：启动超声波
***************************************************/


int ultrasonic(void)
{
   float t;
   unsigned int i = 0;
   unsigned int k,s;

   TMOD |=0x10;

   Trig = 1;		//给一个10us以上的高电平,发出启动信号
   Delay15us();
   Trig = 0; 
   // 5 = （ n*t*340 ）/2;  n为定时器的记数次数，                                                                                                                                                                            
   while(!Echo&&i<50000)     //等待是否有反应，如果有反应则Echo=1,i<50000防止一直在这里循环
   {
      i++;
   }
   i = 0;
   TR1 = 1;	         //开启定时器开始计时
   while(Echo&&i<50000) //等待是否接受到信号,如果接受到信号原来Echo=1变Echo=0 跳出循环，i<5000防止卡死
   {
      i++;
   }
   i = 0;
   TR1 = 0;

   k = (TH1*256+TL1);			   //低八位TL0记满溢出，给高八位TH0加1，没低八位加256次溢出一次给高八位加1
   t =k*(12/22118400.0);

   s = t*340*100/2;
   TH1 = 0;
   TL1 = 0;
   
//   s = t*340*100/2;
//   write_com (0x80 + 0x40);          //+48转换成ASCII码，因为数码管显示只能对应的ASCII码
//
//   write_data (s/100+48);
//
//   write_data (s/10%10+48);
//
//   write_data (s%10+48);

   return s;
  

}
