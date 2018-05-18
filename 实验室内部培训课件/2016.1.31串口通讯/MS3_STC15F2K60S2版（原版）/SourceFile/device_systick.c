/*
********************************************************************************
*                                嵌入式微系统
*                               msOS-mcu51-v0.1
*
*                               实用单片机系统
*                               McuSystem(MS3.3)
*                               
*                              主芯片:STC89C52
*                           深圳市雨滴科技有限公司
*
*                                作者:王绍伟
*                                网名:凤舞天
*                                标识:wangsw
*
*                                QQ:26033613
*                               QQ群:291235815
*                        淘宝店:http://52edk.taobao.com
*                      论坛:http://gongkong.eefocus.com/bbs/
*                博客:http://forum.eet-cn.com/BLOG_wangsw317_1268.HTM
********************************************************************************
*文件名   : system_tick.h
*作用     : 系统节拍
*创建时间 : 2013.8.17
********************************************************************************
*/

#include "system.h"

/*100 指每秒100次，也就是10mS一次*/
#define TickPerSencond  100        
/*******************************************************************************
* 函数名	: InitializeSystemTick
* 描述	    : 初始化系统节拍，节拍周期由TickPerSencond宏定义决定
* 输入参数  : 无     
* 返回参数  : 无
*******************************************************************************/
void InitializeSystemTick(void)
{
	ushort configure;
	configure = 0x10000 - (MainClock/12/TickPerSencond); 
	T2H = configure >> 8;;		//设置定时初值
	T2L = configure& 0xFF;		//设置定时初值
	EA = 1;
	IE2 |= 0x04 ;  //打开定时器2中断
	AUXR |= 0X10;	//运行定时器2
	
}

/*******************************************************************************
* 函数名	: SystemTickInterruptHandler
* 描述	    : 系统节拍中断入口，节拍周期由TickPerSencond宏定义决定
* 输入参数  : 无     
* 返回参数  : 无
*******************************************************************************/
void SystemTickInterruptHandler(void) interrupt 12 
{
    AUXR &= ~0X10 ;  //关闭定时器2
    	
    KeySystemTickService();             //按键例行服务程序
    RtcSystemTickService();             //时钟例行服务程序
    TimerSystemTickService();           //软件定时器例行服务程序

    /*例行程序*/

    AUXR |= 0X10 ;    //打开定时器2
}

