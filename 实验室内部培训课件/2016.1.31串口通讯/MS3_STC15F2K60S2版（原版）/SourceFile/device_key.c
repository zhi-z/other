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
*文件名   : device_key.c
*作用     : 按键设备
*创建时间 : 2013.8.17
********************************************************************************
*/


#include "system.h"


#define ShortInterval			3		//短按按键节拍，不得低于3
#define LongInterval			40		//长按按键节拍
#define DoubleHitInterval	    10		//防止连续双击误动节拍

static byte idata ScanValueSave = invalid;    //第一次按键存储值
static byte idata ScanCounter = 0;            //按键按下节拍计数器
static byte idata DoubleHitCounter = 0;       //防止连续双击误动节拍计数器



/*******************************************************************************
* 函数名	: RemapKey
* 描述	    : 短按键映射
* 输入参数  : scanValue:扫描值
* 返回参数  : 短按按键映射值
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2013/09/27       初始版本
*******************************************************************************/
static byte RemapKey(byte scanValue) 
{
    switch(scanValue)
    {
        case 0x77:
            return(12);
        case 0x7B:
            return(8);
        case 0x7D:
            return(4);
        case 0x7E:
            return(0);
        case 0xB7:
            return(13);
        case 0xBB:
            return(9);
        case 0xBD:
            return(5);
        case 0xBE:
            return(1);
        case 0xD7:
            return(14);
        case 0xDB:
            return(10);
        case 0xDD:
            return(6);
        case 0xDE:
            return(2);
        case 0xE7:
            return(15);
        case 0xEB:
            return(11);
        case 0xED:
            return(7);
        case 0xEE:
            return(3);
        default:
            return(invalid);
    }
}



/*******************************************************************************
* 函数名	: RemapLongKey
* 描述	    : 长按按键映射
* 输入参数  : scanValue 扫描值
* 返回参数  : 长按按键映射值
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2013/09/27       初始版本
*******************************************************************************/
static byte RemapLongKey(byte scanValue) 
{
    switch(scanValue)
    {
        case 0x77:
            return(0x30);
        case 0x7B:
            return(0x31);
        case 0x7D:
            return(0x32);
        case 0x7E:
            return(0x33);
        case 0xB7:
            return(0x34);
        case 0xBB:
            return(0x35);
        case 0xBD:
            return(0x36);
        case 0xBE:
            return(0x37);
        case 0xD7:
            return(0x38);
        case 0xDB:
            return(0x39);
        case 0xDD:
            return(0x3A);
        case 0xDE:
            return(0x3B);
        case 0xE7:
            return(0x3C);
        case 0xEB:
            return(0x3D);
        case 0xED:
            return(0x3E);
        case 0xEE:
            return(0x3F);
        default:
            return(invalid);
    }	
}

/*****************************************************************************
* 函数名	: ScanPin
* 描述	    : P1口4*4矩阵按键扫描
* 输入参数  : 无
* 返回参数  : byte类型按键扫描值
*****************************************************************************/
static byte ScanPin(void)
{
    byte scanValue;
	
    P1 = 0x0F;                      //P17~P14置低
    Delay(1);                       //延时，等待电平稳定
    scanValue = P1;                 //读取低4位按键值
	
    P1 = 0xF0;                      //P17~P14置低
    Delay(1);	                    //延时，等待电平稳定
    scanValue = scanValue | P1 ;    //读取高四位按键值并与低四位合并
	
    return(scanValue);              //返回值
}

/*******************************************************************************
* 函数名	: KeySystemTickService
* 描述	    : 按键扫描服务程序，间隔10mS一次扫描获取按键信息
* 输入参数  : 无
* 返回参数  : 抛出长短两种按键值消息
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2013/09/27       初始版本
*******************************************************************************/
void KeySystemTickService(void) 
{
    byte scanValue;

    scanValue = ScanPin();                     //读取矩阵按键值

    if (scanValue == invalid)                  //判断是否释放按键
    {
        if (DoubleHitCounter)                  //防止双击计数器
        {
            DoubleHitCounter--;
            ScanCounter = 0;
            ScanValueSave = invalid;
            return;
        }
        
        if (ScanValueSave == invalid)           //判断按键值是否有效
        {
            ScanCounter = 0;
            return;
        }
            
        if (ScanCounter == LongInterval)        //判断为长按键，发送长按键消息 
        {
            PostMessage(KeyMessageType, RemapLongKey(ScanValueSave)); 
        } 
        else if (ScanCounter > ShortInterval)   //判断为短按键，发送短按键消息 
        {
            PostMessage(KeyMessageType, RemapKey(ScanValueSave));
        }
        ScanCounter = 0;                        
        ScanValueSave = invalid;
        DoubleHitCounter = DoubleHitInterval;   //设定防止双击计数值
    } 
    else                                        //检测到按键值
    {
        ScanCounter++;
	
        if (ScanCounter == 1)                   //存储第一次按键值
        {
            ScanValueSave = scanValue;
        } 
        else if (ScanCounter == 2)              //判断按键值是否有效
        {
            if (scanValue != ScanValueSave)
            {
                ScanValueSave = invalid;
                ScanCounter = 0;
            }
        }
        else if (ScanCounter >= LongInterval)   //防止计数值溢出循环
        {
            ScanCounter = LongInterval;
        }
    }
}






