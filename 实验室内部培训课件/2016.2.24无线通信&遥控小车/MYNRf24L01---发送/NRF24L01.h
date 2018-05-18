#ifndef __NRF24L01_H__
#define __NRF24L01_H__

#include <STC15F2K60S2.H>
#include <intrins.h>

typedef unsigned char uchar;
typedef unsigned char uint;

#define Tx_Data_Width  3  	// 要发送的数据的宽度为3个字节，根据需要修改
#define Rx_Data_Width  3  	// 要接收的数据的宽度为3个字节，根据需要修改

//--------------------------------------------------- NRF24L01寄存器指令
#define READ_REG        0x00  	// 读寄存器指令
#define WRITE_REG       0x20 	// 写寄存器指令
#define RD_RX_PLOAD     0x61  	// 读取接收数据指令
#define WR_TX_PLOAD     0xA0  	// 写待发数据指令
#define FLUSH_TX        0xE1 	// 冲洗发送 FIFO指令
#define FLUSH_RX        0xE2  	// 冲洗接收 FIFO指令
#define REUSE_TX_PL     0xE3  	// 定义重复装载数据指令
#define NOP             0xFF  	// 保留

//------------------------------------SPI(nRF24L01)寄存器地址
#define CONFIG          0x00  // 配置收发状态，CRC校验模式以及收发状态响应方式
#define EN_AA           0x01  // 自动应答功能设置
#define EN_RXADDR       0x02  // 可用信道设置
#define SETUP_AW        0x03  // 收发地址宽度设置
#define SETUP_RETR      0x04  // 自动重发功能设置
#define RF_CH           0x05  // 工作频率设置
#define RF_SETUP        0x06  // 发射速率、功耗功能设置
#define STATUS          0x07  // 状态寄存器
#define OBSERVE_TX      0x08  // 发送监测功能
#define CD              0x09  // 地址检测           
#define RX_ADDR_P0      0x0A  // 频道0接收数据地址
#define RX_ADDR_P1      0x0B  // 频道1接收数据地址
#define RX_ADDR_P2      0x0C  // 频道2接收数据地址
#define RX_ADDR_P3      0x0D  // 频道3接收数据地址
#define RX_ADDR_P4      0x0E  // 频道4接收数据地址
#define RX_ADDR_P5      0x0F  // 频道5接收数据地址
#define TX_ADDR         0x10  // 发送地址寄存器
#define RX_PW_P0        0x11  // 接收频道0接收数据长度
#define RX_PW_P1        0x12  // 接收频道0接收数据长度
#define RX_PW_P2        0x13  // 接收频道0接收数据长度
#define RX_PW_P3        0x14  // 接收频道0接收数据长度
#define RX_PW_P4        0x15  // 接收频道0接收数据长度
#define RX_PW_P5        0x16  // 接收频道0接收数据长度
#define FIFO_STATUS     0x17  // FIFO栈入栈出状态寄存器设置

sbit	CSN		=P0^2;
sbit 	MOSI	=P0^3;
sbit	IRQ		=P0^4;
sbit	CE	  =P0^5;
sbit	SCK	  =P0^6;
sbit 	MISO	=P0^7;

void init_NRF24L01(void); // 初始化NRF24L01 

uchar NRF24L01_TxPacket(unsigned char * Tx_Buf);	// 将tx_buf中的数据发送出去

unsigned char NRF24L01_RxPacket(unsigned char* Rx_buf);//接收数据并将数据放入rx_buf接收缓冲区中
	
#endif /*__NRF24L01_H__*/


