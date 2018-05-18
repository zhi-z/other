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
*文件名   : mmi.c
*作用     : 界面设计
*创建时间 : 2013.8.17
********************************************************************************
*/


#include "system.h"
#include "app.h"


/*******************************************************************************
以下为状态机方式界面
*******************************************************************************/
typedef enum
{
    WorkState = 1,
    SetupState = 2,
    ServiceState = 3
}MenuState;

static MenuState idata State = WorkState;

static void WorkStateProcess(Key key)
{
    printf("当前工作界面: key = %c\n", key);
    if (key == '2')
    {
        printf("    切换为设置界面\n");
        State = SetupState;
    }
}

static void SetupStateProcess(Key key)
{
    printf("当前设置界面: key = %c\n", key);
    if (key == '3')
    {
        printf("    切换为维护界面\n");
        State = ServiceState;
    }     
}

static void ServiceStateProcess(Key key)
{
    printf("当前维护界面: key = %c\n", key);
    if (key == '1')
    {
        printf("    切换为工作界面\n");
        State = WorkState;
    }  
}

/*******************************************************************************
* 函数名	: KeyProcess
* 描述	    : 状态机按键处理函数
* 输入参数  : key 按键输入
* 返回参数  : 无
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2013/09/30       初始版本
*******************************************************************************/
void KeyProcess(Key key)
{
    switch (State)
    {
        case WorkState:
            WorkStateProcess(key);
            break;
        case SetupState:
            SetupStateProcess(key);
            break;
        case ServiceState:
            ServiceStateProcess(key);
            break;
    }
}

/*******************************************************************************
以下为函数指针方式界面
*******************************************************************************/

void (*MmiFunction)(Key key);                           //定义函数指针变量


/*******************************************************************************
* 函数名	: InitializeIdleForm
* 描述	    : 界面初始化
* 输入参数  : 无
* 返回参数  : 无
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2013/09/30       初始版本
*******************************************************************************/
static void InitializeIdleForm(void)
{
    printf("\nmsOS-mcu51-v0.1(MS3.3) Start\n");
    printf("\ndate:2013.8.17\n");
    printf("\nHardware Plat:MCU51Series\n");
    printf("\Author: wangsw\n");
    printf("\n\n");
	
    printf("1:工作界面\n");
    printf("2:设置界面\n");
    printf("3:维护界面\n");
	
    printf("Menu = idle\n");
    printf("Please press 1 , 2 or 3  to select menu\n");
}

//声明函数，因为前面的调用后面的，编译器无法识别，所以需要声明
static void WorkPointerProcess(Key key);
static void SetupPointerProcess(Key key);
static void ServicePointerProcess(Key key);

static void WorkPointerProcess(Key key)
{
    printf("当前工作界面: key = %c\n", key);
    if (key == '2')
    {
        printf("    切换为设置界面\n");
        MmiFunction = SetupPointerProcess;
    }
}

static void SetupPointerProcess(Key key)
{
    printf("当前设置界面: key = %c\n", key);
    if (key == '3')
    {
        printf("    切换为维护界面\n");
        MmiFunction = ServicePointerProcess;
    }     
}

static void ServicePointerProcess(Key key)
{
    printf("当前维护界面: key = %c\n", key);
    if (key == '1')
    {
        printf("    切换为工作界面\n");
        MmiFunction = WorkPointerProcess;
    }  
}

/*******************************************************************************
* 函数名	: InitializeMmi
* 描述	    : MMI初始化
* 输入参数  : 无
* 返回参数  : 无
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2013/09/30       初始版本
*******************************************************************************/
void InitializeMmi(void)
{

    InitializeIdleForm();
	P1  = 0 ;
    MmiFunction = WorkPointerProcess;
}

/*******************************************************************************
* 函数名	: TimerCallBack
* 描述	    : 软件定时器测试函数，当调用时，自我循环打印信息
* 输入参数  : 无
* 返回参数  : 无
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2013/09/30       初始版本
*******************************************************************************/
void TimerCallBack(void)
{
    printf("软件定时器延时执行\n");
    TimerStart(TimerMessageHandle, 1000, TimerCallBack);
}

/*******************************************************************************
* 函数名	: UsartProcess
* 描述	    : 串口消息处理函数
* 输入参数  : value 接收数据
* 返回参数  : 无
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2013/09/30       初始版本
*******************************************************************************/
void UsartProcess(byte value)
{
    printf("Usart Receive = %x", (ushort)value);

    TimerCallBack();
}
