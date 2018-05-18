/*******************************************************************************
* 文件名 : mmi.c

* 版本     作者            日期            说明
* V1.0                    2015/01/29     初始版本

* 描述   : MCU:stc89c52      晶振:    MHz
* 作用   ： 使用串口实现二级界面 
*******************************************************************************/
#include "uart.h"
#include "mmi.h"

unsigned char State;		 //显示状态标志位

/*******************************************************************************
* 函数名	: Delay10ms
* 描述	    : 延时作用
* 输入参数  : x x个10毫秒     
* 返回参数  : 无
*******************************************************************************/
void Delay10ms(unsigned char x)		//@11.0592MHz
{
	unsigned char i, j,y;
	for(y = 0;y<x;y++)
	{
		i = 18;
		j = 235;
		do
		{
			while (--j);
		} while (--i);
	}
}
/*******************************************************************************
* 函数名	: KeyProcess
* 描述	    : 按键处理函数
* 输入参数  : key   按键的值（按键输入）   
* 返回参数  : 无
*******************************************************************************/
void KeyProcess(unsigned char key)
{
    switch (State)
    {
        case FIRSTUISTATE_1:
            FirstUI_1(key);
            break;
        case FIRSTUISTATE_2:
            FirstUI_2(key);
            break;
        case FIRSTUISTATE_3:
            FirstUI_3(key);
            break;
        case SECONDUISTATE_1:
            SecondUI_1(key);
            break;
        case SECONDUISTATE_2:
            SecondUI_2(key);
            break;
    }
}

/*******************************************************************************
* 函数名	: RemapKey
* 描述	    : 按键输入返回
* 输入参数  : 无     
* 返回参数  : Keyreturn  按键指定的数字
*******************************************************************************/
int RemapKey(void)
{
	unsigned char Keyreturn = 0;

	if(key_1 == 0)  //-------key_1
	{
		Delay10ms(1);
		if(key_1 == 0)
		{
			Keyreturn = RETURNKEY_1;
			while(!key_1);  //等待按键释放
		
		}
	}	
	if(key_2 == 0)	//   key_2
	{
		Delay10ms(1);
		if(key_2 == 0)
		{
			Keyreturn = RETURNKEY_2;
			while(!key_2);  //等待按键释放
		
		}
	}
	if(key_3 == 0)   //   key_3
	{
		Delay10ms(1);
		if(key_3 == 0)
		{
			Keyreturn = RETURNKEY_3;
			while(!key_3);  //等待按键释放
		
		}
	}	
	if(key_4 == 0)	//  key_4
	{
		Delay10ms(1);
		if(key_4 == 0)
		{
			Keyreturn = RETURNKEY_4;
			while(!key_4);  //等待按键释放
		
		}
	}
	
	return Keyreturn;					 
}

/*******************************************************************************
* 函数名	: Init_UI
* 描述	    : 初始化界面
* 输入参数  : 无     
* 返回参数  : 无
*******************************************************************************/
void Init_UI()
{
	State = FIRSTUISTATE_1;
    UART_Send_Str("    ****************************\n");
	UART_Send_Str("    *      初始化成功          *\n");
	UART_Send_Str("    *                          *\n");
	UART_Send_Str("    *         Welcome          *\n");
	UART_Send_Str("    *                          *\n");
	UART_Send_Str("    *   Intelligent Control    *\n");
	UART_Send_Str("    *                          *\n");
	UART_Send_Str("    *                          *\n");
	UART_Send_Str("    ****************************\n");
	UART_Send_Enter();
	UART_Send_Enter();
	UART_Send_Enter();
	Delay10ms(50);
    UART_Send_Str("    *********欢 迎 选 择********\n");
	UART_Send_Str("    *                          *\n");
	UART_Send_Str("    *                          *\n");
	UART_Send_Str("    *          〉 1            *\n");
	UART_Send_Str("    *             2            *\n");
	UART_Send_Str("    *             3            *\n");
	UART_Send_Str("    *                          *\n");
	UART_Send_Str("    *                    确定  *\n");
	UART_Send_Str("    ****************************\n");
	UART_Send_Enter();
	UART_Send_Enter();
	UART_Send_Enter();
}

/*******************************************************************************
* 描述	    : 界面显示
* 输入参数  : key    
* 返回参数  : 无
*******************************************************************************/
void FirstUI_1(unsigned char key)
{

	if (key == RETURNKEY_1)
	{
	    UART_Send_Str("    *********欢 迎 选 择********\n");
		UART_Send_Str("    *                          *\n");
		UART_Send_Str("    *                          *\n");
		UART_Send_Str("    *             1            *\n");
		UART_Send_Str("    *          〉 2            *\n");
		UART_Send_Str("    *             3            *\n");
		UART_Send_Str("    *                          *\n");
		UART_Send_Str("    *                    确定  *\n");
		UART_Send_Str("    ****************************\n");
		UART_Send_Enter();
		UART_Send_Enter();
		UART_Send_Enter();
		State = FIRSTUISTATE_2;
			
	}
}

void FirstUI_2(unsigned char key)
{
	if (key == RETURNKEY_1)
	{
	    UART_Send_Str("    *********欢 迎 选 择********\n");
		UART_Send_Str("    *                          *\n");
		UART_Send_Str("    *                          *\n");
		UART_Send_Str("    *             1            *\n");
		UART_Send_Str("    *             2            *\n");
		UART_Send_Str("    *          〉 3            *\n");
		UART_Send_Str("    *                          *\n");
		UART_Send_Str("    *                    确定  *\n");
		UART_Send_Str("    ****************************\n");
		UART_Send_Enter();
		UART_Send_Enter();
		UART_Send_Enter();
		State = FIRSTUISTATE_3;	
	}
	if (key == RETURNKEY_2)
	{
	    UART_Send_Str("    *********欢 迎 选 择********\n");
		UART_Send_Str("    *                          *\n");
		UART_Send_Str("    *                          *\n");
		UART_Send_Str("    *          〉 1            *\n");
		UART_Send_Str("    *             2            *\n");
		UART_Send_Str("    *             3            *\n");
		UART_Send_Str("    *                          *\n");
		UART_Send_Str("    *                    确定  *\n");
		UART_Send_Str("    ****************************\n");
		UART_Send_Enter();
		UART_Send_Enter();
		UART_Send_Enter();
		State = FIRSTUISTATE_1;			
	}
	if (key == RETURNKEY_3)
	{
	    UART_Send_Str("    *********欢 迎 选 择********\n");
		UART_Send_Str("    ****************************\n");
		UART_Send_Str("    ****************************\n");
		UART_Send_Str("    *          〉 1            *\n");
		UART_Send_Str("    *             2            *\n");
		UART_Send_Str("    * 已经进入二级选项   返回  *\n");
		UART_Send_Str("    ****************************\n");
		UART_Send_Str("    ****************************\n");
		UART_Send_Str("    ****************************\n");
		UART_Send_Enter();
		UART_Send_Enter();
		UART_Send_Enter();
		State = SECONDUISTATE_1;		
	}
}

void FirstUI_3(unsigned char key)
{
	if (key == RETURNKEY_2)
	{
	    UART_Send_Str("    *********欢 迎 选 择********\n");
		UART_Send_Str("    *                          *\n");
		UART_Send_Str("    *                          *\n");
		UART_Send_Str("    *             1            *\n");
		UART_Send_Str("    *          〉 2            *\n");
		UART_Send_Str("    *             3            *\n");
		UART_Send_Str("    *                          *\n");
		UART_Send_Str("    *                    确定  *\n");
		UART_Send_Str("    ****************************\n");
		UART_Send_Enter();
		UART_Send_Enter();
		UART_Send_Enter();
		State = FIRSTUISTATE_2;		
	}
}

void SecondUI_1(unsigned char key)
{
	if (key == RETURNKEY_2)
	{
	    UART_Send_Str("    *********欢 迎 选 择********\n");
		UART_Send_Str("    ****************************\n");
		UART_Send_Str("    ****************************\n");
		UART_Send_Str("    *             1            *\n");
		UART_Send_Str("    *          〉 2            *\n");
		UART_Send_Str("    * 已经进入二级选项   返回  *\n");
		UART_Send_Str("    ****************************\n");
		UART_Send_Str("    ****************************\n");
		UART_Send_Str("    ****************************\n");
		UART_Send_Enter();
		UART_Send_Enter();
		UART_Send_Enter();
		State = SECONDUISTATE_2;		
	}
	if (key == RETURNKEY_4)
	{
	    UART_Send_Str("    *********欢 迎 选 择********\n");
		UART_Send_Str("    *                          *\n");
		UART_Send_Str("    *                          *\n");
		UART_Send_Str("    *          〉 1            *\n");
		UART_Send_Str("    *             2            *\n");
		UART_Send_Str("    *             3            *\n");
		UART_Send_Str("    *                          *\n");
		UART_Send_Str("    *                    确定  *\n");
		UART_Send_Str("    ****************************\n");
		UART_Send_Enter();
		UART_Send_Enter();
		UART_Send_Enter();
		State = FIRSTUISTATE_1;		
	}

}

void SecondUI_2(unsigned char key)
{
	if (key == RETURNKEY_1)
	{
	    UART_Send_Str("    *********欢 迎 选 择********\n");
		UART_Send_Str("    ****************************\n");
		UART_Send_Str("    ****************************\n");
		UART_Send_Str("    *          〉 1            *\n");
		UART_Send_Str("    *             2            *\n");
		UART_Send_Str("    * 已经进入二级选项   返回  *\n");
		UART_Send_Str("    ****************************\n");
		UART_Send_Str("    ****************************\n");
		UART_Send_Str("    ****************************\n");
		UART_Send_Enter();
		UART_Send_Enter();
		UART_Send_Enter();
		State = SECONDUISTATE_1;		
	}
	if (key == RETURNKEY_4)
	{
	    UART_Send_Str("    *********欢 迎 选 择********\n");
		UART_Send_Str("    *                          *\n");
		UART_Send_Str("    *                          *\n");
		UART_Send_Str("    *          〉 1            *\n");
		UART_Send_Str("    *             2            *\n");
		UART_Send_Str("    *             3            *\n");
		UART_Send_Str("    *                          *\n");
		UART_Send_Str("    *                    确定  *\n");
		UART_Send_Str("    ****************************\n");
		UART_Send_Enter();
		UART_Send_Enter();
		UART_Send_Enter();
		State = FIRSTUISTATE_1;		
	}

}