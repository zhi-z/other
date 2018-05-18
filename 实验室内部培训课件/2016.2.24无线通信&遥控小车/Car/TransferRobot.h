
/*******************************************************************************
* 文件名 : TransferRobot.H

* 版本     作者            日期            说明
* V1.0                  2015/10/15       初始版本

* 描述   : MCU:      晶振:    MHz
           
*******************************************************************************/

#ifndef __TransferRobot_H__
#define __TransferRobot_H__

 //-------------------------------转角定义
 #define ANGLE_LEFT_30   30  
 #define ANGLE_LEFT_45   45 
 #define ANGLE_LEFT_90   90
 #define ANGLE_LEFT_135   135
 #define ANGLE_LEFT_180   180 
 #define ANGLE_RIGHT_180   -180 
 #define ANGLE_RIGHT_30   -30  
 #define ANGLE_RIGHT_45   -45 
 #define ANGLE_RIGHT_90   -90
 #define ANGLE_RIGHT_135   -135

 //-------------------------------颜色返回值
 #define COLOUR_RED   2		   //红色
 #define COLOUR_WHITE   3	   //白色
 #define COLOUR_BLUE   4	   //蓝色
 #define COLOUR_BLACK   5	   //黑色
 #define COLOUR_YELLOW   6	   //黄色

 unsigned char COLOUR;

//----------------------------------------判断颜色部分
#define uchar unsigned char
#define uint unsigned int 
//#define	LED P1_3 
sbit LED   =   P2^1;
//out 接p32
sbit  s0   =   P1^4;     
sbit  s1   =   P1^5;     
sbit  s2   =   P1^2;    
sbit  s3   =   P3^7;
long int  green=0,red=0,blue=0;	//RGB颜色初始化
long int pulses=0;
long int time=0;
//long int t[3]={18,9,12};
long int t[3]={35,26,37};
int f;
void T0_init();
//void Time2_init();
void Outside_Init() ;
void whitebalance();
void test_led();

 unsigned char Counter = 0;	 //记录小车走过了哪个点
 unsigned char Counter_FGHI = 0;
 unsigned char Counter_Wedges = 0;	 //记录取到多少个物块，并放到出发区的黑线上
 unsigned char Counter_BlackLineWedges = 0;
 unsigned int Go_Forward_Counter = 0;  //前进记数
 unsigned char Flag_Ultrasonic = 0,Flag_Rotate = 0,flag = 0,flag_1 = 1;
 

 void tracking(void);//红外线循迹
 void RotationAngle(int flag);  //旋转角度
 void JudgeWedges(void);   //判断是否有物块如果有去取物块，然后旋转180度返回
 void Return_A(void);
 void Return_B(void);
 void Return_C(void);
 void Return_D(void);
 void Return_E(void);
// void Return_F(void);
// void Return_I(void);
 int JudgeColour();
 void GetBodyChoice(unsigned char counter);
 void GoRedAddress(void);
 void WedgesAddressChoice(unsigned char x);
 void GoYellowAddress(void);
 void GoBlackAddress(void);
 void GoBlueAddress(void);
 void GoWhiteAddress(void);
 void GetBlackLineWedges(void);
 void RightGetBlackLine(void);
 void LeftGetBlackLine(void);
 void ArrangeWedges(unsigned char y);

 void GetFGHIWedges_Right(void);
 void GetFGHIWedges_Left(void);
 void GoHome(void);
 void PlaceWedges(void);
 void JudgeWedges_1(void);


#endif /*__ABCD_H__*/


