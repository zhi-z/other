/*******************************************************************************
* 文件名 : NRF24L01.h
* 实现功能:NRF24L01头文件
* 版本     作者            日期            说明
* V1.0                   2015/08/05       初始版本

* 描述   : MCU: STC15f2k60s2     晶振: 22.1184  MHz
			
           
*******************************************************************************/ 
#ifndef __NRF24L01_H__
#define __NRF24L01_H__

#include <STC15F2K60S2.H>
#include <intrins.h>

typedef unsigned char uchar;
typedef unsigned char uint;

#define Tx_Data_Width  3  	// 要发送的数据的宽度为3个字节，根据需要修改
#define Rx_Data_Width  3  	// 要接收的数据的宽度为3个字节，根据需要修改

//--------------------------------------------------- NRF24L01寄存器指令
#define WRITE_REG       0x20 	// 写寄存器指令
#define RD_RX_PLOAD     0x61  	// 读取接收数据指令
#define WR_TX_PLOAD     0xA0  	// 写待发数据指令

//------------------------------------SPI(nRF24L01)寄存器地址
#define CONFIG          0x00  // 配置收发状态，CRC校验模式以及收发状态响应方式
#define EN_AA           0x01  // 自动应答功能设置
#define EN_RXADDR       0x02  // 可用信道设置
#define RF_CH           0x05  // 工作频率设置
#define RF_SETUP        0x06  // 发射速率、功耗功能设置
#define STATUS          0x07  // 状态寄存器     
#define RX_ADDR_P0      0x0A  // 频道0接收数据地址
#define TX_ADDR         0x10  // 发送地址寄存器
#define RX_PW_P0        0x11  // 接收频道0接收数据长度

sbit	CSN		=P0^2;
sbit 	MOSI	 =P0^3;
sbit	IRQ		=P0^4;
sbit	CE	  =P0^5;
sbit	SCK	  =P0^6;
sbit 	MISO	=P0^7;

void init_NRF24L01(void); // 初始化NRF24L01 

uchar NRF24L01_TxPacket(unsigned char * Tx_Buf);	// 将tx_buf中的数据发送出去

unsigned char NRF24L01_RxPacket(unsigned char* Rx_buf);//接收数据并将数据放入rx_buf接收缓冲区中
	
#endif /*__NRF24L01_H__*/


