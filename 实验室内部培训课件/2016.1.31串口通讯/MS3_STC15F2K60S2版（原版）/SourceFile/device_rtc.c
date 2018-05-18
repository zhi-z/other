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
*文件名   : device_rtc.c
*作用     : 虚拟时钟设备
*创建时间 : 2013.8.17
********************************************************************************
*/


#include "system.h"


Rtc idata RtcEntity;            //定义Rtc实体

void RtcSystemTickService(void) 
{
    static byte idata Counter = 0;
    
    if(++Counter == 100)        //系统节拍10ms一次，1S需要100次
    {
        Counter = 0;
        RtcEntity.Second++;
    }
    
    if(RtcEntity.Second == 60)
    {
        RtcEntity.Second = 0;
        RtcEntity.Minute++;
    }
    
    if(RtcEntity.Minute == 60)
    {
        RtcEntity.Minute = 0;
        RtcEntity.Hour++;
    }
    
    if(RtcEntity.Hour == 24)
    {
        RtcEntity.Hour = 0;
        RtcEntity.Day++;
    }	
}

void InitializeRtc(void) 
{
    RtcEntity.Second = 0;
    RtcEntity.Minute = 0;
    RtcEntity.Hour = 0;
    RtcEntity.Day = 0;
}

