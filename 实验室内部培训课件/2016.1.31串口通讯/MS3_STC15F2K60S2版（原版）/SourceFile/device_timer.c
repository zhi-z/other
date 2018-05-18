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
*文件名   : device_timer.c
*作用     : 软件定时器设备
*创建时间 : 2013.8.17
********************************************************************************
*/

#include "system.h"

#define TimerSum		0x08     //软件定时器数量
static byte idata State = 0;    //工作标记寄存器，按位标记，共8路，标记是否就绪

struct Timer                     //Timer类型，包含延时时间和回调函数变量
{
    ushort  Delay;
    function CallBackFuction;
};
static struct Timer idata Block[TimerSum];  //存储延时及回调函数Timer类型数组
static TimerhandleMode idata Mode;          //工作模式




void TimerSystemTickService(void)
{
    byte i = 0;
    byte stateCopy;
    
    if (State == 0x00) return;      //就绪表为空，跳出

    EnterCritical();
  
    stateCopy = State;              //复制一份就绪表
    while (stateCopy)	
    {
        if ((stateCopy & 0x01) == 1) 
        {
            if ((--Block[i].Delay) == 0) //计数递减到零时
            {
                if (GetBit(Mode, i))     //获取工作模式，决定处理方式
                    //抛出消息到大循环中执行
                    PostMessage(TimerMessageType, (ushort)(Block[i].CallBackFuction));
                else 
                    (*(Block[i].CallBackFuction))(); //直接在系统节拍中处理
                    
                ResetBit(State, i);     //计数减到零后，这一路定时器动态退出                 
            }
        }
        stateCopy = stateCopy >> 1;
        i++;
    }
    ExitCritical();
}

/*****************************************************************************
* 函数名	: TimerStart
* 描述	    : 软件定时器
* 输入参数  : handleMode: 两种处理方式，一种直接在节拍中断中处理，适合费用低的,
*                         另一种在消息中处理，适合处理费用高的。
*           : delay:延时节拍数，以系统节拍为单位
*           : callBackFunction: 回调执行函数，延时超时后，执行此函数。
*             data:消息值，MessageTimer类型为32bit地址,其他类型下都是24bit数据
* 返回参数  : byte类型，返回ID号，从0开始，若失败则返回invalid(-1)
*****************************************************************************/
byte TimerStart(TimerhandleMode handleMode, ushort delay, function callBackFunction)
{
    byte i;
	unsigned int j;
    EnterCritical();
    for(i = 0; i < TimerSum; i++) 
    {
        if(!GetBit(State, i)) 
        {			
            Block[i].Delay = delay;                                 //延时时间
            Block[i].CallBackFuction = callBackFunction;            //回调函数
            if(handleMode)                                          //工作模式
            {
                SetBit(Mode, i);
				j = Mode;
				printf("Mode = %d\n",j);
            } 
            else 
            {
                ResetBit(Mode, i);
				j = Mode;
				printf("Mode = %d\n",j);
            }
            SetBit(State, i); 			//置位开启
            ExitCritical();
            return(i);
        }
    }
    ExitCritical();
    return(invalid);
}


/*******************************************************************************
* 函数名	: TimerStop
* 描述	    : 关闭某一路定时器
* 输入参数  : id TimerStart执行后返回的id，0、1、2...
* 返回参数  : 无
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2013/09/25       初始版本
*******************************************************************************/
void TimerStop(byte id) 
{
    if (id >= TimerSum) return;     
        
    EnterCritical();
	
    ResetBit(State, id);            //复位就绪位
	
    ExitCritical();
}


