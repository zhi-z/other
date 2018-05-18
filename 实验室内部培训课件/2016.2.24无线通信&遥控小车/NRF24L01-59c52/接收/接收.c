#include <reg52.H>
#include "uart.h"
#include "NRF24L01.h"

#define uchar unsigned char
#define uint unsigned int

/*******************************************************************************
* 功能描述 : 主函数
* 函数属性 : 
* 输入参数 : 
* 返回参数 : 
* 函数详解 : 
*******************************************************************************/


void Delay500ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	i = 4;
	j = 129;
	k = 119;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void main(void)
{
	//请到头文件中修改数据的宽度，以保证匹配
	uchar i = 0;
	uchar a[Rx_Data_Width] = 0;//定义用户缓冲区，接收到的数据保存于此
	
	UartInit();
	init_NRF24L01() ;
	
	UART_Send_Str("开始接收信息\r\n\r\n\r\n");
	UART_Send_Str("Recive: ");
	
	while(1)
	{
		if(NRF24L01_RxPacket(a))
		{
//			UART_Send_Str("Recive: ");
			
			for(i=0;i<3;i++)
			{
				UART_Send_Byte(a[i]);
			}
			//判读Tx_DS,如果模块的连接不正常，将没有串口、LED的提示
			UART_Send_Str("\r\n");
//			Delay500ms()	;
	  }
		 
	}	

}	