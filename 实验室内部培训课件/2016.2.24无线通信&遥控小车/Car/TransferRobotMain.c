/*******************************************************************************
* 文件名 : main.c
* 实现功能:12864测试程序
* 版本     作者            日期            说明
* V1.0                      2015/08/10       初始版本

* 描述   : MCU: STC89C52     晶振: 11.0592   MHz

* 		 这个程序在c点方向没有设定，并且最后一个物块在右边
           
*******************************************************************************/

 #include <reg52.h>
 #include "car.h"
 #include "ultrasonic.h"
 #include <BoeBot.h>
 #include <intrins.h>
 #include "uart.h"
 #include "TransferRobot.h"
	
 
 void main(void)
 {
	test_led();
	s0= 1 ;//---------------------------------比例1
	s1= 1 ;	
//	UartInit();	
	T0_init();//----------------------------定时器初始化
	Outside_Init();//-----------------------外部中断初始化
	
	go_forward(100);
		
 
	while(1) //-----------------------------从出发区出发转向A点物块
	{
		if(Left_2_led != 1) //--------------0检测到
		{
			tracking();
		}
		else
		{
//			calibration(10);
			go_forward(25);		 
			calibration(10);
			break;
		}
	}
	RotationAngle(90);machine(2,132,132);//-左转90度，检测A点是否有物块
	calibration(50);

//-------------------判断A B C D 是否有物块，如果有就把它放回出发区的前面的黑线上。	
//                 当取到第四个物块的时候退出。
	flag_1 = 0;
	while(1)
	{  			 
		JudgeWedges_1(); //--------------------判断是否有物块，当有物块取取到物块后返回
		if(Counter<=4)
		GetBodyChoice(Counter);	//-----------判断从哪个点开始判断
		if(Counter>4)
			break;
	}


	while(Counter_Wedges>0)
	{

	   GetBlackLineWedges();//----------------从黑线交叉地方去取，被转移的物块

		COLOUR = JudgeColour();
		while(COLOUR == 0)
			COLOUR = JudgeColour();

		WedgesAddressChoice(COLOUR);//-------把物块送到指定的区域

		Counter_Wedges--;
	}

//	RotationAngle(-90);	//----------旋转90度后
	GetFGHIWedges_Right();   //------------获取第五个物块

	COLOUR = JudgeColour();//--------------判断第五个物块的颜色
	while(COLOUR == 0)
		COLOUR = JudgeColour();

	WedgesAddressChoice(COLOUR);//-------把物块送到指定的区域
	GoHome();

	while(1)
	calibration(1000);
	return;		 //完成退出


//while(1)
//{
//	calibration(50);
//	RotationAngle(-180);
//COLOUR = 	   
//	JudgeColour();// Delay1ms(1000);
//	if(COLOUR==COLOUR_BLACK)
//		go_forward(24);		
   Return_E();
//LeftGetBlackLine();
//	PlaceWedges();
//GetFGHIWedges_Left(); 

//  GetBlackLineWedges();
//tracking();
//}	
 }

 /***************************************************************************************************
 *
 *	 函数名称：	    	tracking
 *   作用：             红外线循迹
 *        
 ***************************************************************************************************/

 void tracking(void)
 {
	if(Left_1_led==0&&Centre_1_led==0&&Right_1_led==0)	 //1
	{
		Go_Forward_Counter++;
		go_forward(1);	
	}	
	if(Left_1_led==0&&Centre_1_led==0&&Right_1_led==1)	 //2
		machine(1,132,132);   //左转
	if(Left_1_led==0&&Centre_1_led==1&&Right_1_led==0)	 //3
		machine(1,172,172);     //右转
	if(Left_1_led==0&&Centre_1_led==1&&Right_1_led==1)	 //4
		machine(1,132,132);     //左转
	if(Left_1_led==1&&Centre_1_led==0&&Right_1_led==0)   //5
		machine(1,172,172); 	 //右转
	if(Left_1_led==1&&Centre_1_led==0&&Right_1_led==1)   //6
	{
		Go_Forward_Counter++;
		go_forward(1);
	}
	if(Left_1_led==1&&Centre_1_led==1&&Right_1_led==0)   //7
		machine(1,172,172); 	 //右转
	if(Left_1_led==1&&Centre_1_led==1&&Right_1_led==1)   //8
	{
		Go_Forward_Counter++;
		go_forward(1); 
	}
 }

  /***************************************************************************************************
 -
 -	 函数名称：	    	RotationAngle
 -   作用：             旋转角度的选择
 -   描述：     		 30，45,90,180
 ***************************************************************************************************/
  void RotationAngle(int flag)
  {
		switch(flag)
		{	
			case  ANGLE_LEFT_30:  machine(3,132,132); break;
			
			case  ANGLE_LEFT_45:  machine(14,132,132); break;
			
			case  ANGLE_LEFT_90:  machine(26,132,132); break;
			                          	
			case  ANGLE_LEFT_135: machine(40,132,132); break;
	
			case  ANGLE_LEFT_180: machine(53,132,132); break;	
	
			case  ANGLE_RIGHT_30:  machine(5,172,172); break;
			
			case  ANGLE_RIGHT_45:  machine(14,172,172); break;
			
			case  ANGLE_RIGHT_90:  machine(26,172,172); break;
			                          	
			case  ANGLE_RIGHT_135: machine(40,172,172); break;	
										
			case  ANGLE_RIGHT_180: machine(53,172,172); break;
	
			default :  break;
		}	
  }

 /***************************************************************************************************
 *
 *	 函数名称：	    	JudgeWedges
 *   作用：             判断会否有物块，如果有进前进去拿物块，拿到后旋转180度返回
 *        
 ***************************************************************************************************/
 
 void JudgeWedges_1(void)
 {
    Go_Forward_Counter = 0;
 	while(1)
	{	
		Delay1ms(10);
		if(Counter == 2)		//判断是否有物块
		{
			if(Counter >= 4)
			{
				Counter++;
				RotationAngle(-90);
				calibration(10);
				go_forward(10);
				calibration(10);
				break;
			}
			else{

			  	RotationAngle(-45); //右转45
				Counter++;		  //Counter为0时为A,1为B，2为C，3为D，4为E
				calibration(20);			
				
			}

		}	 
		else if(Counter == 0||Counter == 1||Counter == 3||Counter == 4)
		{
			if(flag_1 == 0)
			{
				go_forward(12);
				flag_1++;
//				if(Counter == 0)
				 RightGetBlackLine();
//				else{
//
//					 LeftGetBlackLine();
//				}
				
			}
			while(1)
			{
				tracking();	  //循迹
				if(ultrasonic()<3/*&&ultrasonic()>=0*/)
				{
					if(Counter != 0/*||Counter != 3*/)	//当不是A点和E点的时候
					{
						while(Go_Forward_Counter<90)
							tracking();	  //循迹
						Go_Forward_Counter = 0;
		
						machine(50,132,132);machine(2,132,132);//后转//后转
						calibration(10);
						flag_1=0;
						break;
					}
					else{
						while(Go_Forward_Counter<90)
							tracking();	  //循迹
						Go_Forward_Counter = 0;
		
						machine(50,172,172);//machine(2,132,132);后转//后转
						calibration(10);
						flag_1=0;
						break;					
					
					}
				} 
			}
			break;
	    }
   }
 }


 /***************************************************************************************************
 *
 *	 函数名称：	    	JudgeWedges
 *   作用：             判断会否有物块，如果有进前进去拿物块，拿到后旋转180度返回
 *        
 ***************************************************************************************************/
 
 void JudgeWedges(void)
 {
    Go_Forward_Counter = 0;
 	while(1)
	{	
		Delay1ms(10);
		if(ultrasonic()>25)		//判断是否有物块
		{
			if(Counter >= 4)
			{
				Counter++;
				RotationAngle(-90);
				calibration(10);
				go_forward(10);
				calibration(10);
				break;
			}
			else{

			  	machine(13,172,172); //右转45
				Counter++;		  //Counter为0时为A,1为B，2为C，3为D，4为E
				calibration(20);			
				
			}

		}	 
		else if(ultrasonic()<25)
		{
			if(flag_1 == 0)
			{
				go_forward(25);
				flag_1++;
			}
			while(1)
			{
				tracking();	  //循迹
				if(ultrasonic()<3/*&&ultrasonic()>=0*/)
				{
					if(Counter != 0/*||Counter != 3*/)	//当不是A点和E点的时候
					{
						while(Go_Forward_Counter<82)
							tracking();	  //循迹
						Go_Forward_Counter = 0;
		
						machine(50,132,132);machine(2,132,132);//后转//后转
						calibration(10);
						flag_1=0;
						break;
					}
					else{
						while(Go_Forward_Counter<75)
							tracking();	  //循迹
						Go_Forward_Counter = 0;
		
						machine(50,172,172);//machine(2,132,132);后转//后转
						calibration(10);
						flag_1=0;
						break;					
					
					}
				} 
			}
			break;
	    }
   }
 }

 /***************************************************************************************************
 *
 *	 函数名称：	    	Return_A
 *   作用：             取到在A的物块以后把物块放回出发区
 *        
 ***************************************************************************************************/
 void Return_A(void)
 {

	COLOUR = JudgeColour();	//判断颜色
	while(COLOUR == 0)
		COLOUR = JudgeColour();

   if(COLOUR==COLOUR_YELLOW)//-------------------------如果是黄色
   {
   		calibration(10);
		RotationAngle(180);//-----------------------右转180度指向黑色区域
		calibration(10);
		LeftGetBlackLine();//-----------------------旋转不够的时候同过微转来寻找黑线

		while(1)//-----------------------------------把物块放到黄色区域
		{
			if(Left_2_led != 1)
			{
				tracking();	  //循迹
			}
			else
			{
//				go_forward(3);	 //再前进一定的距离，把黑色物块放在黄色区域中
				calibration(10);	 
				break;
			}
		}
//-------------------------------------------------放完物块后	后退	
		draw_back(25);
		RotationAngle(-180);//	 machine(2,132,132); //旋转180度
		calibration(10); 
		RightGetBlackLine();//-------------------------当旋转的角度不够的时候，同过向左微转来寻找黑线
//------------------------------------------------通过循迹回道黑线交叉的地方方向指向出发区放物块的黑线上	  	
		
		while(1)
		{
			if(Left_2_led != 1)
			{
				tracking();	  //循迹
			}
			else
			{
				go_forward(25);	//红外线检测到十字路口后再前进一点保证在十字路口的中心位置
				calibration(10);
				Counter++;		 
				RotationAngle(135);//RotationAngle(45);	 //旋转135度，指向B点
				calibration(10);	 
				break;
			}
		}
   }
   else	//-------------------------如果不是黑色，就把物块送到黑线交叉处，进行判断
   {
   		RightGetBlackLine();

		while(1)
		{
			if(Left_2_led != 1)
			{
				tracking();	  //循迹
			}
			else
			{
				go_forward(20);	//红外线检测到十字路口后再前进一点保证在十字路口的中心位置
				calibration(10);	 
				RotationAngle(-90);	 //旋转90度后。再前进一点
				go_forward(25);
				calibration(10);	 
				break;
			}
		}

		RightGetBlackLine();
		Counter_Wedges++;
		ArrangeWedges(Counter_Wedges);
    	RightGetBlackLine();  

		while(1) //把物块放回到出发区后要返回到十字路口停下
		{
			if(Left_2_led != 1)
			{
				tracking();
			}
			else
			{
				go_forward(25);		 
				calibration(10);
				break;
			}
		}

		Counter++;		  //Counter为0时为A,1为B，2为C，3为D，4为E
	 	RotationAngle(45);machine(2,132,132);	//旋转到45度以后指定到B点
		calibration(10);	 

   }


 }


 /***************************************************************************************************
 *
 *	 函数名称：	    	Return_B
 *   作用：             取到在B的物块以后把物块放回出发区
 *        
 ***************************************************************************************************/
 void Return_B(void)
 {
// 	go_forward(10);	
	LeftGetBlackLine();//当旋转的角度不够的时候，同过向左微转来寻找黑线

	COLOUR = JudgeColour();	//判断颜色
	while(COLOUR == 0)
		COLOUR = JudgeColour();

   if(COLOUR==COLOUR_WHITE)//-------------------------如果是白色
   {
//   		calibration(10);
		RotationAngle(180);//-----------------------右转180度指向黑色区域
		calibration(10);

		LeftGetBlackLine();//-----------------------旋转不够的时候同过微转来寻找黑线

		PlaceWedges(); //放物块，然后返回黑箱交叉的地方

		go_forward(15);	
		RotationAngle(135);
		Counter++;		  //Counter为0时为A,1为B，2为C，3为D，4为E
		calibration(10);

   }
   else if(COLOUR==COLOUR_YELLOW){//如果为黄色

		while(1)
		{
			if(Right_led == 1)
			{
				tracking();	  //循迹
			}
			else
			{
				go_forward(25);	//红外线检测到十字路口后再前进一点保证在十字路口的中心位置
				calibration(10);	 
				RotationAngle(-135); //旋转45度后,指向C点
				go_forward(15);
				calibration(10);	 
				break;
			}
		}  
		
		LeftGetBlackLine();

		PlaceWedges(); //放物块，然后返回黑箱交叉的地方
		Counter++;		  //Counter为0时为A,1为B，2为C，3为D，4为E
		RotationAngle(90);	//左转90度指向D点
		calibration(10);
   
   }
   else{

		while(1)
		{
			if(Right_led == 1)
			{
				tracking();	  //循迹
			}
			else
			{
				go_forward(25);	//红外线检测到十字路口后再前进一点保证在十字路口的中心位置
				calibration(10);	 
				RotationAngle(-45);	 //旋转45度后。再前进一点
				go_forward(20);
				calibration(10);	 
				break;
			}
		}
	
		LeftGetBlackLine();//当旋转的角度不够的时候，同过向左微转来寻找黑线

		Counter_Wedges++;
	
		ArrangeWedges(Counter_Wedges); //排列转移到黑线上的物体
	
		RightGetBlackLine();//当旋转的角度不够的时候，同过向左微转来寻找黑线
			 	 
		while(1) //把物块放回到出发区后要返回到十字路口停下
		{
			if(Left_2_led != 1)
			{
				tracking();
			}
			else
			{
				go_forward(24);		 
				calibration(10);
				break;
			}
		}
		Counter++;		  //Counter为0时为A,1为B，2为C，3为D，4为E
		calibration(10);   
   
   }

 }

 /***************************************************************************************************
 *
 *	 函数名称：	    	Return_C
 *   作用：             取到在C的物块以后把物块放回出发区 ，到达路口后指向了D点
 *        
 ***************************************************************************************************/
 void Return_C(void)
 {
	
	LeftGetBlackLine();//当旋转的角度不够的时候，同过向左微转来寻找黑线

	COLOUR = JudgeColour();	//判断颜色
	while(COLOUR == 0)
		COLOUR = JudgeColour();

   if(COLOUR==COLOUR_RED)//-------------------------如果是红色
   {
   		calibration(10);
		RotationAngle(180);//-----------------------右转180度指向黑色区域
		calibration(10);
		LeftGetBlackLine();//-----------------------旋转不够的时候同过微转来寻找黑线

		PlaceWedges(); //放物块，然后返回黑箱交叉的地方

		go_forward(15);	
		RotationAngle(135);// RotationAngle(90); //machine(2,132,132);//旋转指向D点
		Counter++;		  //Counter为0时为A,1为B，2为C，3为D，4为E
		calibration(10);

   }
	else{

		while(1)//--------------------------通过循迹回到黑线交叉的地方，然后转向红色区域C点
		{
			if(Right_led != 0)
			{
				tracking();	  //循迹
			}
			else
			{
				go_forward(35);	//红外线检测到十字路口后再前进一点保证在十字路口的中心位置
				calibration(10);	 
 
				break;
			}
		 }

		 if(COLOUR!=COLOUR_WHITE&&COLOUR!=COLOUR_YELLOW)
		 {
		  	go_forward(20);	

			LeftGetBlackLine();//当旋转的角度不够的时候，同过向左微转来寻找黑线

			Counter_Wedges++;
		
			ArrangeWedges(Counter_Wedges); //排列转移到黑线上的物体
		
			RightGetBlackLine();//当旋转的角度不够的时候，同过向左微转来寻找黑线	

			while(1)//--------------------------通过循迹回到黑线交叉的地方，然后转向红色区域C点
			{
				if(Left_2_led != 1)
				{
					tracking();	  //循迹
				}
				else
				{
					go_forward(24);	
					calibration(10);	 
	 				RotationAngle(-45);	 //旋转45度指向D点
					Counter++;		  //Counter为0时为A,1为B，2为C，3为D，4为E
					calibration(10);
					break;
				}
			 }
				 
		 }
		 else{
	
			 RotationAngle(180);//machine(4,132,132);
			 calibration(10);	
	
			 WedgesAddressChoice(COLOUR);//从黑线交叉的地方前往指定颜色的区域
	
		 	 RotationAngle(135);//旋转135度，指向D点
			 Counter++;		  //Counter为0时为A,1为B，2为C，3为D，4为E

			 calibration(10);	 				 
		 
		 }	
	
	}
	
 }

 /***************************************************************************************************
 *
 *	 函数名称：	    	Return_D
 *   作用：             取到在D的物块以后把物块放回出发区 ，到达路口后指向了E点
 *        
 ***************************************************************************************************/
 void Return_D(void)
 {

	LeftGetBlackLine();//当旋转的角度不够的时候，同过向左微转来寻找黑线

	COLOUR = JudgeColour();
	while(COLOUR == 0)
		COLOUR = JudgeColour();  

   if(COLOUR==COLOUR_BLACK)//-------------------------如果是黑色
   {
   		calibration(10);
		RotationAngle(180);//-----------------------右转180度指向黑色区域
		calibration(10);
		LeftGetBlackLine();//-----------------------旋转不够的时候同过微转来寻找黑线

		PlaceWedges(); //放物块，然后返回黑箱交叉的地方

		go_forward(15);		
		calibration(10);
		RotationAngle(45); RotationAngle(90); //旋转指向E点
		Counter++;		  //Counter为0时为A,1为B，2为C，3为D，4为E
		calibration(10);

   }
   else{

		while(1)//--------------------------通过循迹回到黑线交叉的地方，然后转向红色区域C点
		{
			if(Right_led == 1)
			{
				tracking();	  //循迹
			}
			else
			{
				go_forward(30);	//红外线检测到十字路口后再前进一点保证在十字路口的中心位置
				calibration(10);	 
				break;
			}
		 }
		 
		 if(COLOUR!=COLOUR_WHITE&&COLOUR!=COLOUR_YELLOW&&COLOUR!=COLOUR_RED) 
		 {
		 	RotationAngle(45);
			calibration(10);
		  	go_forward(15);	
			calibration(10);

			LeftGetBlackLine();//当旋转的角度不够的时候，同过向左微转来寻找黑线

			Counter_Wedges++;
		
			ArrangeWedges(Counter_Wedges); //排列转移到黑线上的物体
//			machine(2,132,132);
			RightGetBlackLine();//当旋转的角度不够的时候，同过向左微转来寻找黑线	

			while(1)//--------------------------通过循迹回到黑线交叉的地方，然后转向红色区域C点
			{
				if(Left_2_led != 1)
				{
					tracking();	  //循迹
				}
				else
				{
					go_forward(25);	//红外线检测到十字路口后再前进一点保证在十字路口的中心位置
					calibration(10);	 
	 				RotationAngle(-90);machine(3,132,132);	 //旋转45度指向E点
					Counter++;		  //Counter为0时为A,1为B，2为C，3为D，4为E
					calibration(10);
					break;
				}
			 }		 
		 
		 }
		 else{

			 RotationAngle(-90);RotationAngle(-45);
			 calibration(10);	 
			 WedgesAddressChoice(COLOUR);//从黑线交叉的地方前往指定颜色的区域
	
		 	 RotationAngle(90);//旋转90度，指向E点
			 LeftGetBlackLine();
			 Counter++;		  //Counter为0时为A,1为B，2为C，3为D，4为E
			 calibration(10);			 		 		 
		 }
   }

 }

 /***************************************************************************************************
 *
 *	 函数名称：	    	Return_E
 *   作用：             当D点有物块的时候
 *				
 ***************************************************************************************************/
 void Return_E(void)
 {
	LeftGetBlackLine();//当旋转的角度不够的时候，同过向左微转来寻找黑线

	COLOUR = JudgeColour();
	while(COLOUR == 0)
		COLOUR = JudgeColour();  

   if(COLOUR==COLOUR_BLUE)//-------------------------如果是蓝色
   {
   		calibration(10);
		RotationAngle(-180);//-----------------------右转180度指向蓝色区域
		calibration(10);
		RightGetBlackLine();//-----------------------旋转不够的时候同过微转来寻找黑线

		PlaceWedges(); //放物块，然后返回黑箱交叉的地方
		go_forward(15);
		RotationAngle(90); //旋转指向出发区方向
		Counter++;		  //Counter为0时为A,1为B，2为C，3为D，4为E
		calibration(10);


   }
   else{
		while(1)//--------------------------通过循迹回到黑线交叉的地方，然后转向红色区域C点
		{
			if(Right_led == 1)
			{
				tracking();	  //循迹
			}
			else
			{
				go_forward(25);	//红外线检测到十字路口后再前进一点保证在十字路口的中心位置
				calibration(10);	 
				RotationAngle(-90);
				calibration(10);	 
				break;
			}
		 } 
		 
		 WedgesAddressChoice(COLOUR);//从黑线交叉的地方前往指定颜色的区域
		 Counter++;		  //Counter为0时为A,1为B，2为C，3为D，4为E
   
   }

 }

 /***************************************************************************************************
 *
 *	 函数名称：	    	GetBodyChoice
 *   作用：             去到物块后返回的路径选择
 *        
 ***************************************************************************************************/
 void GetBodyChoice(unsigned char counter)
 {
	switch(counter)
	{	
		case  0:  Return_A(); break;
		
		case  1:  Return_B(); break;
		
		case  2:  Return_C(); break;
		                          	
		case  3:  Return_D(); break;

		case  4:  Return_E(); break;

		default :  break;
	}	 
 
 }


 
 /***************************************************************************************************
 *
 *	 函数名称：	    	WedgesAddressChoice
 *   作用：             把物块从交叉处送回指定颜色区域的选择
 *   参数：          	是什么颜色
 *        
 ***************************************************************************************************/
 void WedgesAddressChoice(unsigned char x)
 {
	switch(x)
	{	
		case  COLOUR_RED:    GoRedAddress(); break;	 //红色
		
		case  COLOUR_WHITE:  GoWhiteAddress(); break;	 //白色
		
		case  COLOUR_BLUE:   GoBlueAddress(); break;	 //蓝色
		                          	
		case  COLOUR_BLACK:  GoBlackAddress(); break;	 //黑色

		case  COLOUR_YELLOW: GoYellowAddress(); break;  //黄色

		default :  break;
	}	  	 
 }

 /***************************************************************************************************
 *
 *	 函数名称：	    	GoRedAddress
 *   作用：             把物块从交叉处送回红色区域
 *   参数：          	
 *        
 ***************************************************************************************************/
 void GoRedAddress(void)
 {
 	go_forward(25);	
	calibration(2);
	Flag_Rotate = 0;
	while(Left_1_led==1&&Centre_1_led==1&&Right_1_led==1)
	{
		if(Flag_Rotate<17)
		{
			 machine(2,132,132); //都检测不到的时候，源地旋转
			 calibration(2);
			 Flag_Rotate++;
		}
		else{
			 machine(2,172,172); //都检测不到的时候，源地旋转
			 calibration(2);
			 Flag_Rotate++;		
		}
	}

	while(1)//------------------------------把物块放到红色区域
	{
		if(Left_2_led != 1/*||Right_2_led != 0*/)
		{
			tracking();//-------------------循迹
		}
		else
		{
//			go_forward(3);
			calibration(10);	 
			break;
		}
	}
	
	draw_back(30);//------------------------放完物块后	后退
	RotationAngle(180);//machine(2,132,132);//旋转180度
	calibration(10); 
	  
	LeftGetBlackLine();//-------------------当旋转的角度不够的时候，同过向左微转来寻找黑线

	while(1)
	{
		if(Right_led == 1/*||Right_2_led != 1*/)
		{
			tracking();	  //循迹
		}
		else
		{

			go_forward(30);	//--------------交叉路口太多黑线，到这里需要先前进一定的距离后再循迹
			calibration(10);	 
			break;
		}
	}
 }

 /***************************************************************************************************
 *
 *	 函数名称：	    	GoWhiteAddress
 *   作用：             把物块从交叉处送回白色区域
 *   参数：          	
 *        
 ***************************************************************************************************/
 void GoWhiteAddress(void)
 {
	RotationAngle(45);	 
	calibration(2);
	go_forward(25);

	LeftGetBlackLine();//-------------------当旋转的角度不够的时候，同过向左微转来寻找黑线

	while(1)//------------------------------把物块放到白色区域
	{
		if(Left_2_led != 1/*||Right_2_led != 0*/)
		{
			tracking();	//------------------循迹
		}
		else
		{
//			go_forward(2);
			calibration(1);	 
			break;
		}
	}
	
	draw_back(25);//------------------------放完物块后	后退
	RotationAngle(180);//machine(2,132,132);//旋转180度
	calibration(10); 
	  
	LeftGetBlackLine();//-------------------当旋转的角度不够的时候，同过向左微转来寻找黑线

	while(1)//------------------------------通过循迹回道黑线交叉的地方方向指向出发区放物块的黑线上
	{
		if(Right_led == 1/*||Right_2_led != 1*/)
		{
			tracking();	  //----------------循迹
		}
		else
		{
			go_forward(30);	//--------------红外线检测到十字路口后再前进一点保证在十字路口的中心位置
			calibration(10);	 
			RotationAngle(-45);	//----------旋转45度后。再前进一点
//			go_forward(10);
			calibration(10);	 
			break;
		}
	}


 }
 /***************************************************************************************************
 *
 *	 函数名称：	    	GoBlueAddress
 *   作用：             把物块从交叉处送回蓝色区域
 *   参数：          	
 *        
 ***************************************************************************************************/
 void GoBlueAddress(void)
 {
	RotationAngle(-90);	 
	calibration(2);
	go_forward(25);

	Flag_Rotate = 0;
	while(Left_1_led==1&&Centre_1_led==1&&Right_1_led==1)
	{
		if(Flag_Rotate<15)
		{
			 machine(2,132,132); //都检测不到的时候，源地旋转
			 calibration(2);
			 Flag_Rotate++;
		}
		else{
			 machine(2,172,172); //都检测不到的时候，源地旋转
			 calibration(2);
			 Flag_Rotate++;		
		}
	}

	while(1)//------------------------------把物块放到蓝色区域
	{
		if(Left_2_led != 1)
		{
			tracking();	  //----------------循迹
		}
		else
		{
			go_forward(10);
			calibration(10);	 
			break;
		}
	}

	draw_back(23);//-----------------------放完物块后	后退	
	RotationAngle(180);//machine(2,132,132);//旋转180度
	calibration(10); 

//--------------------------通过循迹回道黑线交叉的地方方向指向出发区放物块的黑线上
	  
	LeftGetBlackLine();//------------------当旋转的角度不够的时候，同过向左微转来寻找黑线

	while(1) //----------------------------循迹回到交叉口
	{
		if(Right_led == 1)
		{
			tracking();	  //---------------循迹
		}
		else
		{
			go_forward(25);	//-------------红外线检测到十字路口后再前进一点保证在十字路口的中心位置
			RotationAngle(90);
//			calibration(10);	 
			go_forward(15);
			calibration(10);	
			break; 
		}
	} 

 }
 /***************************************************************************************************
 *
 *	 函数名称：	    	GoBlackAddress
 *   作用：             把物块从交叉处送回黑色区域
 *   参数：          	
 *        
 ***************************************************************************************************/
 void GoBlackAddress(void)
 {
	RotationAngle(-45);	 
	calibration(2);
	go_forward(15);

	if(Counter<3)
		LeftGetBlackLine();//-------------------当旋转的角度不够的时候，同过向左微转来寻找黑线
	else
		RightGetBlackLine();	

	while(1)//------------------------------把物块放到黑色区域
	{
		if(Left_2_led != 1/*||Right_2_led != 0*/)
		{
			tracking();	  //循迹
		}
		else
		{
//			go_forward(3);
			calibration(1);	 
			break;
		}
	}

	draw_back(30); //-------------------------放完物块后	后退	
	RotationAngle(180);//machine(2,132,132);//--旋转180度
	calibration(10); 

//------------------------------通过循迹回道黑线交叉的地方方向指向出发区放物块的黑线上	 
 
	LeftGetBlackLine();//-------------------当旋转的角度不够的时候，同过向左微转来寻找黑线

	while(1)
	{
		if(Right_led == 1/*||Right_2_led != 1*/)
		{
			tracking();	  //-----------------循迹
		}
		else
		{
		
			go_forward(35);	//---------------红外线检测到十字路口后再前进一点保证在十字路口的中心位置
			calibration(10);	 
			RotationAngle(45);
			go_forward(10);
			calibration(10);	 
			break;
		}
	}
 
 }
 /***************************************************************************************************
 *
 *	 函数名称：	    	GoYellowAddress
 *   作用：             把物块从交叉处送回黄色区域
 *   参数：          	
 *        
 ***************************************************************************************************/
 void GoYellowAddress(void)
 {
	RotationAngle(90);	 
	calibration(2);
	go_forward(25);

	LeftGetBlackLine();//-------------------当旋转的角度不够的时候，同过向左微转来寻找黑线


	while(1)//------------------------------把物块放到黄色区域
	{
		if(Left_2_led != 1/*||Right_2_led != 0*/)
		{
			tracking();	  //循迹
		}
		else
		{
			go_forward(1);
			calibration(10);	 
			break;
		}
	}
	
	draw_back(25);//------------------------放完物块后	后退
	RotationAngle(180);//	 machine(2,132,132); //旋转180度
	calibration(10); 

//------------------------------------通过循迹回道黑线交叉的地方方向指向出发区放物块的黑线上
	  
	LeftGetBlackLine();//-------------------当旋转的角度不够的时候，同过向左微转来寻找黑线

	while(1) //-----------------------------循迹回到交叉口
	{
		if(Left_2_led != 1)
		{
			tracking();	  //----------------循迹
		}
		else
		{
			go_forward(24);	//--------------红外线检测到十字路口后再前进一点保证在十字路口的中心位置
			calibration(10);	 
			RotationAngle(-90);//	 machine(2,172,172);//----------旋转90度后
//			go_forward(10);
			calibration(10);	 
			break;
		}
	} 
 
 }

 /***************************************************************************************************
 *
 *	 函数名称：	    	GetBlackLineWedges
 *   作用：             从交叉处去取出发区黑线上的物块,并把它送回黑线的交叉处
 *   参数：          	
 *        
 ***************************************************************************************************/
 void GetBlackLineWedges(void)
 {
 	go_forward(10);
//	calibration(5);
//------------------------------------到这里小车已经指向了在黑线上的物块
	LeftGetBlackLine();//--------------当旋转的角度不够的时候，同过向左微转来寻找黑线
//------------------------------------去取下一个物块，并把它送回到交叉路口
	flag_1 = 0;
	Go_Forward_Counter = 0;
//	calibration(5);
	if(Counter_Wedges != 1)
	{
		while(1)
		{
			if(flag_1 == 0)
			{
				go_forward(5);
				flag_1++;
			}
			tracking();	  //循迹
			
			if(ultrasonic()<=2/*&&ultrasonic()>=0*/)
			{
				Go_Forward_Counter = 0;
				while(Go_Forward_Counter<16)
					tracking();	  //循迹
				Go_Forward_Counter = 0;
				calibration(5);
				RotationAngle(-180);//machine(2,172,172);//后转//后转
				calibration(10);
				flag_1 = 0;
				break;
			} 
		}
	
		RightGetBlackLine();//当旋转的角度不够的时候，同过向左微转来寻找黑线
		
		while(1) //取到物块后返回到十字路口停下
		{
			if(Left_2_led != 1/*||Right_2_led != 1*/)
			{
				tracking();
			}
			else
			{
				go_forward(25);		 
				calibration(10);
				break;
			}
		}
	}
	else{
		while(1)
		{
			if(Right_led == 1)
			{
				tracking();	  //循迹
			}
			else
			{
				go_forward(20);		
				RotationAngle(-180);//machine(2,172,172);//后转//后转
				calibration(10);
				break;
			}
		}
		
		RightGetBlackLine();//当旋转的角度不够的时候，同过向左微转来寻找黑线
		
		while(1) //取到物块后返回到十字路口停下
		{
			if(Left_2_led != 1/*||Right_2_led != 1*/)
			{
				tracking();
			}
			else
			{
				go_forward(25);		 
				calibration(10);
				break;
			}
		}	
	
	}			
 }

 /***************************************************************************************************
 *
 *	 函数名称：	    	LeftGetBlackLine
 *   作用：             当旋转角度不够的时候通过小角度的旋转找黑线
 *   参数：          	
 *        
 ***************************************************************************************************/
 void LeftGetBlackLine(void)
 {
	while(Left_1_led==1&&Centre_1_led==1&&Right_1_led==1)  //当三个红外线都检测不到时候
	{
		 machine(2,132,132); //都检测不到的时候，源地旋转
		 calibration(2);
		 Flag_Rotate++;
	} 
 
 }

 /***************************************************************************************************
 *
 *	 函数名称：	    	RightGetBlackLine
 *   作用：             当旋转角度不够的时候通过小角度的旋转找黑线
 *   参数：          	
 *        
 ***************************************************************************************************/
 void RightGetBlackLine(void)
 {
	while(Left_1_led==1&&Centre_1_led==1&&Right_1_led==1)  //当三个红外线都检测不到时候
	{
		 calibration(2);
		 machine(1,172,172); //都检测不到的时候，源地旋转
		 Flag_Rotate++;
	} 
 
 }




 /***************************************************************************************************
 *
 *	 函数名称：	    	ArrangeWedges
 *   作用：             排列转移物块再黑线上
 *   参数：          	
 *        
 ***************************************************************************************************/
 void ArrangeWedges(unsigned char y)
 {
 	Go_Forward_Counter = 0;

	switch(y)
	{	
		case  1:  
			while(1)
			{
				if(Right_led == 1)
				{
					tracking();	  //循迹
				}
				else
				{
//					go_forward(10);
					calibration(1);	 
					break;
				}
			}

			draw_back(27);//放完物块后	后退
			RotationAngle(-180);//	 machine(3,172,132); //旋转180度
			calibration(10);

	 	break;
		
		case  2:  
		 	while(Go_Forward_Counter<87)
				tracking();	  //循迹
			Go_Forward_Counter = 0;
		
			draw_back(25);//放完物块后	后退
			RotationAngle(-180);//	 machine(3,132,132); //旋转180度
			calibration(10);	
			
		 break;
		
		case  3: 
			while(Go_Forward_Counter<50)
				tracking();	  //循迹
			Go_Forward_Counter = 0;
		
			draw_back(25);//放完物块后	后退
			RotationAngle(-180);//	 machine(3,132,132); //旋转180度
			calibration(10);
				
		 break;
		                          	

		default :  break;
	}	 
 
 }

/***************************************************************************************************
 *
 *	 函数名称：	    	JudgeColour
 *   作用：             判断物块的颜色
 *   返回值：           定义颜色返回的值
 *        
 ***************************************************************************************************/
int JudgeColour()
{
	LED=1;//LED	
//	whitebalance();
	while(1)
	{	
		
		pulses=0;
		time=0;
		TR0=1; 
		s2=0;						  
		s3=0;
		//选择红色滤光
		while(time!=t[0]);
			red=pulses;
		TR0=0;
		time=0;
		pulses=0;
		TR0=1;
		//选择绿色滤光
		s2=1;
		s3=1;
		while(time!=t[2]);
			green=pulses;
		TR0=0;      
		time=0;
		pulses=0;
		TR0=1; 
		//选择蓝色滤光
		s2=0;
		s3=1;
		while(time!=t[1]); 
			blue=pulses;
		TR0=0;
		time=0;
		pulses=0;
		//关闭定时计数器，只为显示。
		//	     printf("red=%ld,green=%ld,blue=%ld\n",red,green,blue);
		red = red/2.4;green = green/3.9;blue = blue/3.9; 
		UART_Send_Str("red=");
		UART_Put_Num(red);
		UART_Send_Str("\n");
		UART_Send_Str("green=");
		UART_Put_Num(green);
		UART_Send_Str("\n");
		UART_Send_Str("blue=");
		UART_Put_Num(blue);
		UART_Send_Str("\n");
		delay_nms(200);

//---------------------------------------------------黑色
		if(red<10&&blue<10&&green<10)
		{
				UART_Send_Str("黑色");
				LED = 0;//LED	
				return COLOUR_BLACK;     
		}


//---------------------------------------------------白色  54 58 59 
		if(30<red&&30<green&&30<blue)
		{
//			if((-10)<(red-green)&&(red-green)<10&&(-10)<(blue-green)&&(blue-green)<10&&(-10)<(blue-red)&&(blue-red)<10)
//			{
				UART_Send_Str("白色");
				LED = 0;//LED	
				return COLOUR_WHITE;  
//			}   
		}

//---------------------------------------------------蓝色  5 7 13
		if(red<blue&&green<blue&&red<green&&blue<30&&6<blue)
		{
				UART_Send_Str("蓝色");
				LED = 0;//LED	
				return COLOUR_BLUE;     
		}

//---------------------------------------------------黄色  35 29 11
		if(red>blue&&green>blue&&red>green&&red>28&&green>20)
		{
				UART_Send_Str("黄色");	  //32 24 9
				LED = 0;//LED	
				return COLOUR_YELLOW;     
		}

//---------------------------------------------------红色  22 6 5 
		if(red>blue&&red>green&&(-9)<(blue-green)&&(blue-green)<9&&red<50&&green<20&&blue<20)
		{
				UART_Send_Str("红色");
				LED = 0;//LED	
				return COLOUR_RED;     
		}

		return 0;
	}
}

/***************************************************************************************************
 *
 *	 函数名称：	    	whitebalance
 *   作用：             获取白平衡时的比例系数，这函数在这里没有用到
 *   返回值：           
 *        
 ***************************************************************************************************/
void whitebalance()
{
	unsigned char i;	
	UartInit();	
	T0_init();//定时器初始化
	
	Outside_Init();//外部中断初始化
	//Time2_init(); //串口初始化

	s2=0;//红色
	s3=0;
	TR0=1;
//	UART_Send_Str("哈哈\n");   
//	UART_Put_Num(pulses);
	while(pulses<255)
		t[0] =time;//存储系数 
    
	TR0=0;
	time=0;
	pulses=0;	
	s2=0;//蓝色
	s3=1;
	TR0=1;	
	while(pulses<255)							    
		t[1] =time;//存储系数
	TR0=0;
	time=0;
	pulses=0;	
	s2=1;//绿色
	s3=1;
	TR0=1;
	while(pulses<255)
		t[2] =time;//存储系数
	TR0=0;	   
	for(i=0;i<3;i++)
	{
	//printf("%ld\n",t[i]);//输出比例系数
		UART_Put_Num(t[i]);
	} 	
}
	/*----------------------------------------------------------------
      函数名：Time2_init( ) 
      功  能：使用T2做串口时钟
-----------------------------------------------------------------*/
//void Time2_init(void)
//{
//	EA = 1;
//	T2MOD = 0; 
//	T2CON |= 0x10;
//	SCON = 0x50 ;
//	TH2 = (65536 - 36)/256;
//	TL2 = (65536-36)%256;
//	RCAP2H = (65536 - 36)/256;
//	RCAP2L =(65536 - 36)%256;
//	TR2 = 1;	 
//	TI = 1;
//}
void T0_init()//定时器0初始化
{ 
    TMOD |=0x01;//设置定时器0为工作方式1										
    TH0 =0xfc;//装初值
    TL0 =0x66;     //如果晶振是11.0592MHZ的话，误差是0.00017%
    EA=1;//开总中断
    ET0=1;//定时器0中断
//    TR0=1;//启动定时器0
}
//定时器0溢出中断//
void c1ms_out() interrupt 1//1ms中断子程序
{
	TR0=0;
	time=time+1;
	TH0=0xfc;
	TL0=0x66+TL0;
	TR0=1;  
}
void Outside_Init() //外部中断0初始化
{ 	
	EA = 1;     //开总中断 
	EX0 = 1;   //开外部中断0   
	IT0 = 1;   //下降沿触发      
}  
void Outside_Int() interrupt 0 //外部中断0 
{  
	pulses=pulses+1;
//	UART_Put_Num(8);

}


/***************************************************************************************************
 *
 *	 函数名称：	    	test_led
 *   作用：             led测试 闪烁3次
 *   返回值：           
 *        
 ***************************************************************************************************/
void test_led()
{
	unsigned char i;
	for(i=0;i<3;i++)
	{
		LED=0;
		delay_nms(200);
		LED=1;
		delay_nms(200);
	}
	LED=0;
}


 /***************************************************************************************************
 *
 *	 函数名称：	    	GetFGHIWedges
 *   作用：             获取点FGHI的物体
 *        
 ***************************************************************************************************/
 void GetFGHIWedges_Right(void)
 {
 	Counter_FGHI = 0;
	while(1)
	{	
		if(ultrasonic()>20)		//判断是否有物块
		{
		  	RotationAngle(30); //左转30
			Counter_FGHI++;		 
			calibration(10);

			if(Counter_FGHI==17)
			{
			 	RotationAngle(-90);
				calibration(20);
				Counter_FGHI = 0;
			}
		}	 
		else if(ultrasonic()<20)
		{
			go_forward(3);

			if(ultrasonic()<3/*&&ultrasonic()>=0*/)
			{
				go_forward(15);

				while(1)
				{
					if(Right_led == 1)
					{
						go_forward(1);
					}
					else
					{
						go_forward(18);
//						if(Counter_FGHI>=4)
//							 go_forward(15);
						calibration(10);
						machine(25,172,172);
						calibration(10);
						break;
					}
			    }					
				break;
			  }
			 			
			}
   }

//	if(Counter_FGHI<=4)
//	{
//		LeftGetBlackLine();
//	}
//	else
//	{
		RightGetBlackLine();
//	}

	while(1) 
	{
		if(Right_led == 1)
		{
			tracking();
		}
		else
		{
			go_forward(25);		 
			calibration(10);
			break;
		}
	} 

	RotationAngle(-135);
	go_forward(20);
	LeftGetBlackLine();
   
	while(1) 
	{
		if(Left_2_led != 1)
		{
			tracking();
		}
		else
		{
			go_forward(25);		 
			calibration(10);
			break;
		}
	} 

 }

 /***************************************************************************************************
 *
 *	 函数名称：	    	GetFGHIWedges
 *   作用：             获取点FGHI的物体
 *        
 ***************************************************************************************************/
 void GetFGHIWedges_Left(void)
 {
	while(1)
	{	
		if(ultrasonic()>20)		//判断是否有物块
		{
		  	RotationAngle(30); //左转30
			Counter_FGHI++;		 
			calibration(20);
		}	 
		else if(ultrasonic()<20)
		{
			go_forward(3);

			if(ultrasonic()<3/*&&ultrasonic()>=0*/)
			{
				go_forward(15);

				while(1)
				{
					if(Right_led == 1)
					{
						go_forward(1);
					}
					else
					{
						go_forward(18);
//						if(Counter_FGHI>=4)
//							 go_forward(15);
						calibration(10);
						machine(25,132,132);
						calibration(10);
						break;
					}
			    }
				break;	
			  }

			 		
		}
   }

//	if(Counter_FGHI<=4)
//	{
		LeftGetBlackLine();
//	}
//	else
//	{
//		RightGetBlackLine();
//	}

	while(1) 
	{
		if(Right_led == 1)
		{
			tracking();
		}
		else
		{
			go_forward(25);		 
			calibration(10);
			break;
		}
	} 

	RotationAngle(135);
	go_forward(20);
	RightGetBlackLine();
   
	while(1) 
	{
		if(Right_led == 1)
		{
			tracking();
		}
		else
		{
			go_forward(15);		 
			calibration(10);
			break;
		}
	} 

 }

 /***************************************************************************************************
 *
 *	 函数名称：	    	GoHome
 *   作用：             放完物体后小车从黑线的中心回到出发区
 *        
 ***************************************************************************************************/

 void GoHome(void)
 {
 	go_forward(25);
	calibration(5);
 	LeftGetBlackLine();
	while(1)
	{
		if(Right_led == 1)
		{
			tracking();	  //循迹
		}
		else
		{
			go_forward(60); 
			calibration(10);	 
			break;
		}
	}
	while(1)//--------------------------------物块搬运结束，停止
 		calibration(1000);
 }


 /***************************************************************************************************
 *
 *	 函数名称：	    	PlaceWedges
 *   作用：             放置物块，然后回到黑线的交叉处。
 *        
 ***************************************************************************************************/
 void PlaceWedges(void)
 {
	while(1)//-----------------------------------把物块放到有颜色的区域
	{
		if(Left_2_led != 1)
		{
			tracking();	  //循迹
		}
		else
		{
			go_forward(3);	 //再前进一定的距离，把黑色物块放在黑色区域中
			calibration(1);	 
			break;
		}
	}
//-------------------------------------------------放完物块后	后退	
	draw_back(25);
	RotationAngle(180);	// machine(2,132,132); //旋转180度
	calibration(10); 
	
//------------------------------------------------通过循迹回道黑线交叉的地方方向指向出发区放物块的黑线上	  
	LeftGetBlackLine();//-------------------------当旋转的角度不够的时候，同过向左微转来寻找黑线
	
	while(1)
	{
		if(Left_2_led != 1)
		{
			tracking();	  //循迹
		}
		else
		{
			go_forward(25);	//红外线检测到十字路口后再前进一点保证在十字路口的中心位置
			calibration(10);	 
			break;
		}
	} 

 }

