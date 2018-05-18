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
*文件名   : system.h
*作用     : 系统库全局头文件
*创建时间 : 2013.8.17
********************************************************************************
*/


#ifndef __SYSTEM_H
#define __SYSTEM_H


#include <string.h>
#include <stdio.h>
#include "STC15F2K60S2.h"

/*
1、类型定义兼容C#，尽可能用C#标准，常用的为int
2、char在C#中为16bit的unicode，keil C下为8bit
*/
//8bit单字节
typedef signed char         sbyte;
typedef unsigned char       byte;

//16bit双字节
//typedef signed short        short         默认，不需要定义
typedef unsigned short      ushort;

//32bit四字节
//typedef signed long         long;         默认，不需要定义
typedef unsigned long       ulong;

//字符串
typedef signed char *       string;

//布尔变量
typedef unsigned char       bool;

typedef void (*function)(void);
#define Function(address)  (*((function)address))()






/*定义常量*/
#define true                1
#define false               0
#define null                ((void *) 0)
#define invalid             0xFF
#define MainClock           22118400        //MCU系统时钟

#define SetBit(data, offset)     data |= 1 << offset     //置位某位为1
#define ResetBit(data, offset)   data &= ~(1 << offset)  //复位某位为0
#define GetBit(data, offset)     ((data >> offset) &0x01) //获取某位

#define UshortToByte1(data)     ((byte *)(&data))[0]//获取ushort类型数据高位     
#define UshortToByte0(data)     ((byte *)(&data))[1]//获取ushort类型数据低位

#define GetMessageType(message)            UshortToByte1(message)
#define GetMessageData(message)            UshortToByte0(message)

#define Assert(express)  if (!(express)){printf("\nASSERT: " #express "\n");while(1);}   //(void)0


typedef enum
{
    KeyMessageType      = 0xFF,         //按键消息类型
    UsartMessageType    = 0xFE,         //串口消息类型 
    Int0MessageType     = 0xFD,         //外部中断0消息类型
    Int1MessageType     = 0xFC,         //外部中断1消息类型
    //请填充类型
    TimerMessageType    = 0xF0          //最小值,不允许有值小于它
}MessageType;


#include "device.h"

extern void EnterCritical(void);
extern void ExitCritical(void);

extern void InitializeMessageQueue(void);
extern void PostMessage(MessageType messageType, ushort value);
extern ushort PendMessageQueue(void);

extern void Delay(ushort delayClock);

extern void DelayUs(ushort delayTime);

extern void DelayMs(ushort delayTime);


extern byte HexToAscii(byte hex);

extern void PutByte(byte byte);
extern void PutString(string string, byte sum);

extern bool TimerStart(TimerhandleMode handleMode, ushort delay, function callBackFunction);
extern void TimerStop(byte id);

#endif
