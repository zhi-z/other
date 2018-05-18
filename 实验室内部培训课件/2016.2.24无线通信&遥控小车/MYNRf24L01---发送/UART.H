#ifndef __UART_H__
#define __UART_H__

#include <string.h>
#include <stdio.h>

#include <STC15F2K60S2.H>

//UART 相关函数------------------------

extern void UartInit(void); //51单片机的串口初始化
extern void UART_Send_Byte(unsigned char mydata); //串口发送一个字节的函数
extern void UART_Send_Str(char *s);  		//串口发送字符串
extern void UART_Put_Num(unsigned long dat); //串口发送数值,以10进制的形式显示,可用于观察变量的值
extern void UART_Send_Enter();                          //串口发送0d和0a两个字节,即回车换行
extern void UART_Put_Inf(char *inf,unsigned long dat);//单片机的串口发送调试信息,														
extern void u32_to_str(unsigned long dat,char *str); //将一个32位的变量dat转为字符串，比如把1234转为"1234"



#endif /*__UART_H__*/


