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
*文件名   : system.c
*作用     : 系统应用文件
*创建时间 : 2013.8.17
********************************************************************************
*/

#include "system.h"



#define MessageBufferSum 4                              //定义消息队列存储长度
static ushort idata MessageBuffer[MessageBufferSum];

struct MessageQueue                                     //定义一个队列结构体
{                                                       
    ushort  * Start;                                    //指针指向队列开始
    ushort  * End;                                      //指针指向队列结束
    ushort  * In;                                       //指针插入一个消息
    ushort  * Out;                                      //指针取出一个消息
    byte      Size;                                     //队列长度
    byte      Entries;                                  //消息长度
} ;
static struct MessageQueue idata MessageQueue;



static bool EaSave;
static byte CriticalNesting = 0;
byte code HextoAscii[16] = 
{
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

void Delay(ushort delayClock)
{
	while(delayClock--);
}

void DelayUs(ushort delayTime)
{
    while(delayTime--);
}

void DelayMs(ushort delayTime)
{
	while(delayTime--)
	{
		DelayUs(1000);
	}
}

byte HexToAscii(byte hex)
{
	Assert(hex < 16);                   //断言，一旦条件错误，就执行
	return HextoAscii[hex];
}


/*******************************************************************************
* 函数名	: EnterCritical
* 描述	    : 进入临界态，防止中断产生
* 输入参数  : 无
* 返回参数  : 无
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2013/10/11       初始版本
*******************************************************************************/
void EnterCritical(void)
{
    if (CriticalNesting == 0)   //第一级临界态嵌套，保存全局中断寄存器，关闭中断
    {
        EaSave = EA;
        EA = 0;
    }
    CriticalNesting++;    
}


/*******************************************************************************
* 函数名	: ExitCritical
* 描述	    : 退出临界态，恢复原来状态
* 输入参数  : 无
* 返回参数  : 无
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2013/10/11       初始版本
*******************************************************************************/
void ExitCritical(void)
{
    if (CriticalNesting > 0)
    {
        CriticalNesting--;
        if (CriticalNesting == 0)//第一级临界态嵌套，恢复原状态
        {
            EA = EaSave;
        }
    }
}

void InitializeMessageQueue(void)
{
    MessageQueue.Start = &MessageBuffer[0];
    MessageQueue.End = &MessageBuffer[MessageBufferSum];
    MessageQueue.In = MessageQueue.Start;
    MessageQueue.Out = MessageQueue.Start;
    MessageQueue.Size = MessageBufferSum;
    MessageQueue.Entries = 0;
}

/*******************************************************************************
* 函数名	: PostMessage
* 描述	    : 发送消息
* 输入参数  : message: 消息类型, 
*             value:消息值，MessageTimer类型为16bit地址，其他类型下都是8bit数据         
* 返回参数  : 无
*******************************************************************************/
void PostMessage(MessageType messageType, ushort value)
{
	Assert(MessageQueue.Entries < MessageQueue.Size);//断言，判断消息队列是否满。
    
    EnterCritical();            //保存状态 ，关闭中断防止操作被打断导致消息混乱

    if (messageType == TimerMessageType)  //软件定时器类型是特殊消息类型特殊处理
    {
        *MessageQueue.In = value;         //软件定时器传递的是函数指针，直接赋值
    }
    else                                      //常规消息，把消息类型与消息值合并
    {
        UshortToByte1(*MessageQueue.In) = messageType;//把类型赋给消息高地址
        UshortToByte0(*MessageQueue.In) = value & 0xFF;//把值赋给消息低地址
    }
    
	MessageQueue.In++;                                          //消息插入口加一        
    MessageQueue.Entries++;                                     //消息数量加一 

    if(MessageQueue.In == MessageQueue.End)                     //插入口循环
    {
        MessageQueue.In = MessageQueue.Start;
    }
    
	ExitCritical();                                         //回复关闭中断前状态
}

/*******************************************************************************
* 函数名	: PendMessageQueue
* 描述	    : 等待消息
* 输入参数  : 无       
* 返回参数  : 无
*******************************************************************************/
ushort PendMessageQueue(void)
{    
    ushort message;
    
    while(MessageQueue.Entries == 0)//检测是否存在消息
    {
      //推荐在这儿喂狗
    };          

    //EnterCritical();              //可以不用临界态，若只有一个地方读取

    message = *MessageQueue.Out++;

    MessageQueue.Entries--;
    
    if (MessageQueue.Out == MessageQueue.End) 
    {
        MessageQueue.Out = MessageQueue.Start;
    }

    //ExitCritical();
    return(message);
}


