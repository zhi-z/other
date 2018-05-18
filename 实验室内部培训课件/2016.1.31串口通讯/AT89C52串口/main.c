
/*******************************************************************************
* 文件名 : MAIN.C

* 版本     作者            日期            说明
* V1.0                    2015/10/11       初始版本

* 描述   : MCU:      晶振:    MHz
           
*******************************************************************************/


 #include<reg52.h>
 #include "uart.h"	
 #include<stdio.h>	

 extern unsigned char date;
 extern unsigned char flag;
 
 void main(void)
 {
 
   UartInit();
   while(1)
   {
//		UART_Send_Byte(0+48);	//加上48转换成ASCII码
//		UART_Send_Str("abcde\n");
		UART_Put_Num(456) ;	 //打印数字。字节输出456
		UART_Put_Num(456) ;
		str_to_u32(123456);
   }
 
 }