/*
********************************************************************************
*                                嵌入式微系统
*                               msOS for Mcu51
*
*                               主芯片:MCU51
*                           深圳市雨滴科技有限公司
*
*                                作者:王绍伟
*                                网名:凤舞天
*                                标识:Wangsw
*
*                                QQ:26033613
*                               QQ群:291235815
*                        淘宝店:http://52edk.taobao.com
*                      论坛:http://gongkong.eefocus.com/bbs/
*                    Google开源:https://code.google.com/p/msos/
*                博客:http://forum.eet-cn.com/BLOG_wangsw317_1268.HTM
********************************************************************************
*文件名     : app.c
*作用       : 应用层，系统入口
*原理       : 无
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2013/09/23       初始版本
********************************************************************************
*/

#include "system.h"
#include "app.h"


void Initialize(void)
{			
    InitializeRtc();
    
	  InitializeUsart();

    InitializeMmi();

    InitializeMessageQueue();

    InitializeSystemTick();
}



/*******************************************************************************
* 函数名	: main
* 描述	    : 系统函数入口
* 输入参数  : 无
* 返回参数  : 无
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2013/09/23       初始版本
*******************************************************************************/
unsigned int text = 0 ;
int main(void)
{
    ushort idata messageData;
    byte idata type;
    byte idata value;
	
    Initialize();
	
	
    while(true)
    {
        messageData = PendMessageQueue();
		
        type = GetMessageType(messageData);
        value = GetMessageData(messageData);
		
		text = value ;
		printf("%d\n",text);
		
        switch(type)						
        {
            case KeyMessageType:                //状态机与函数指针方式，选一个
#if 0
                (*MmiFunction)(value);             //函数指针处理方式
#else
                KeyProcess(value);              //状态机处理方式
#endif
                break;
#if 1
            case UsartMessageType:
                UsartProcess(value);
                break;
#endif
            //请填充消息及处理函数
            default:                //软件定时器处理
                Function(messageData);
                break;
        }
    }
}





