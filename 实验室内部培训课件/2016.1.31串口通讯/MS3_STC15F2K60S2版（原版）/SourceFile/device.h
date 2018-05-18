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
*文件名   : device.h
*作用     : 设备总头文件
*创建时间 : 2013.8.17
********************************************************************************
*/

#ifndef __DEVICE_H
#define __DEVICE_H


/*按键根据自己定义来修改*/
typedef enum 
{
    AddKey                  = 0x00,
    AddLongKey              = 0x30,
    
    SubKey                  = 0x04,
    SubLongKey              = 0x34,

    AssistUpKey             = 0x01,
    AssistUpLongKey         = 0x31,

    AssistDownKey           = 0x05,
    AssistDownLongKey       = 0x35,
    
    SaveKey                 = 0x02,
    SaveLongKey             = 0x32,

    TextBoxFocusKey         = 0x06,
    FormFocusLongKey        = 0x36,
    
    StartKey                = 0x03,
    StartLongKey            = 0x33,
    
    StopKey                 = 0x07,
    StopLongKey             = 0x37
}Key;

typedef enum 
{
    TimerSystemTickHandle   = 0,        //系统节拍中处理
    TimerMessageHandle      = 1	        //大循环消息中处理
}TimerhandleMode;	

typedef struct
{
    byte Second;                        //秒
    byte Minute;                        //分
    byte Hour;                          //时
    ushort Day;                         //天
}Rtc;

typedef enum
{
    SystemTick10000,
    SystemTick1000,
    SystemTick100
}SystemTick;


extern Rtc idata RtcEntity;

extern void KeySystemTickService(void);

extern void RtcSystemTickService(void);

extern void TimerSystemTickService(void);


extern void InitializeRtc(void);

extern void InitializeUsart(void);

extern void InitializeSystemTick(void);



#endif

