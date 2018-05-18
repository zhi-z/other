# include <stc15f2k60s2.h>
# include "NRF24L01.h"
# include "uart.h"
# include "intrins.h"


sbit key_1 = P3^2;
sbit key_2 = P3^3;
sbit key_3 = P3^4;
sbit key_4 = P3^5;


void Delay10us()		//@22.1184MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	_nop_();
	i = 52;
	while (--i);
}


/*******************************************************************************
* 功能描述 : 延时函数，延时参数为 1 时，大约延时600ms
* 函数属性 : 内部
* 输入参数 : 延时参数
* 返回参数 : 无
* 函数详解 : 
*******************************************************************************/

void Delay600ms()		//@22.1184MHz
{
	unsigned char i, j, k,d;

	for(d = 0;d<6;d++)
	{
		_nop_();
		_nop_();
		i = 9;
		j = 104;
		k = 139;
		do
		{
			do
			{
				while (--k);
			} while (--j);
		} while (--i);
	}
}

/*******************************************************************************
* 功能描述 : 主函数
* 函数属性 : 
* 输入参数 : 
* 返回参数 : 
* 函数详解 : 
*******************************************************************************/
void main()
{
	unsigned char Tx_Data[3]  = {'0','0','0'};
	init_NRF24L01();
	UartInit();
	Delay600ms();

	while(1)
	{
		if(key_1==0)
		{
			 Delay10us();Delay10us();
			 Tx_Data[0]	 = '1';
			 if(NRF24L01_TxPacket(Tx_Data))
			 		UART_Send_Str("Send: …1……… \n");
			 while(!key_1);
		}
		if(key_2==0)
		{
			 Delay10us();Delay10us();
			 Tx_Data[0]	 = '2';
			 if(NRF24L01_TxPacket(Tx_Data))
					 UART_Send_Str("Send: ……2…… \n");
			 while(!key_2);
		}
		if(key_3==0)
		{
			 Delay10us();Delay10us();
			 Tx_Data[0]	 = '3';
			 if(NRF24L01_TxPacket(Tx_Data))
				  UART_Send_Str("Send: ……3…… \n");			 
			 while(!key_3);
		}
		if(key_4==0)
		{
			 Delay10us();Delay10us();
			 Tx_Data[0]	 = '4';
			 if(NRF24L01_TxPacket(Tx_Data))
					UART_Send_Str("Send: ……4…… \n");			 
			 while(!key_4);
		}
	}

}