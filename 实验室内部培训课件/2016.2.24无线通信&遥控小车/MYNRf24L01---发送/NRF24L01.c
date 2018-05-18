#include "NRF24L01.h"

//-------------------------- 状态标志
uint 	bdata sta;   // 用于保存状态寄存器的值   bdata用于将变量定义在可位寻址片内数据存储区，允许位与字节混合访问。
sbit	Rx_DR	=sta^6;    //接收状态
sbit	Tx_DS	=sta^5;    //发送状态
//sbit	Max_RT	=sta^4;

//------------------------------------------------------ 传送的数据的特征设置
#define Tx_Address_Width    5   // 要发送的地址的宽度为5字节，要于接收模块对应
#define Rx_Address_Width    5   // 要接收的地址的宽度为5字节，要于发送模块对应
uint const Tx_Address[Tx_Address_Width]= {0x34,0x43,0x10,0x10,0x01};//本地地址，5字节
uint const Rx_ADDRESS[Rx_Address_Width]= {0x34,0x43,0x10,0x10,0x01};//接收地址，5字节


/*******************************************************************************
* 功能描述 : 延时函数，延时参数为 1 时，大约延时10us
* 函数属性 : 内部
* 输入参数 : 延时参数
* 返回参数 : 无
* 函数详解 : 
*******************************************************************************/

void Delay_us(unsigned char n)
{
		unsigned char i;
		
		while(n--)
		{
				_nop_();
				_nop_();
				_nop_();
				i = 52;
				while (--i);
		}
}

/*******************************************************************************
* 功能描述 : SPI读写函数，最基本的函数
* 函数属性 : 内部
* 输入参数 : 要写入的数据
* 返回参数 : 读到的数据
* 函数详解 : 

	最基本的函数，完成 GPIO 模拟 SPI 的功能。将输出字节（MOSI）从 MSB 循环输出，
同时将输入字节（MISO）从 LSB 循环移入。上升沿读入，下降沿输出。

*******************************************************************************/

 uint SPI_ReadWrite(uint dat)
{
	  uint bit_ctr;
   	for(bit_ctr=0;bit_ctr<8;bit_ctr++) //一个字节数据循环8次
   	{
		MOSI = (dat & 0x80);    //将dat的高位输出MOSI
		dat  = (dat << 1);      //dat数据左移，最高位数据抛弃，次高位数据占领最高位
		SCK = 1;                // SCK置高，准备传输
		dat |= MISO;       	    // 读取MISO数据存入到dat的最低位
		SCK = 0;                // 开始传输数据
   	}
    return(dat);           		  //返回读到的值
}

/*******************************************************************************
* 功能描述 : NRF24L01读寄存器
* 函数属性 : 内部
* 输入参数 : dat: 指令+寄存器地址
* 返回参数 : 寄存器的值
* 函数详解 : 形参为：

	读取寄存器值的函数：基本思路就是通过 READ_REG 命令（形参为：0x00+寄存器地址），
把寄存器中的值读出来并将值返回

*******************************************************************************/

uchar Read_Reg(uchar dat)
{
		uchar reg_val;
		
		CSN = 0;                // CSN low, initialize SPI communication...
		SPI_ReadWrite(dat);            // Select register to read from..
		reg_val = SPI_ReadWrite(0);    // ..then read registervalue
		CSN = 1;                // CSN high, terminate SPI communication
		
		return(reg_val);        // return register value
}

/*******************************************************************************
* 功能描述 : NRF24L01读、写寄存器
* 函数属性 : 内部
* 输入参数 : dat: 指令+寄存器地址     value： 要写入的值
* 返回参数 : 无
* 函数详解 : 

	调用该函数时使用写寄存器指令（WRITE_REG），即 dat = WRITE_REG + 需要写入数据
的寄存器的地址，即可将value写入到需要写入的寄存器

*******************************************************************************/

uint ReadWrite_Reg(uchar dat, uchar value)
{
		uint status;
		
		CSN = 0;                   // CSN low, init SPI transaction
		status = SPI_ReadWrite(dat);      // select register
		SPI_ReadWrite(value);             // ..and write value to it..
		CSN = 1;                   // CSN high again
		
		return(status);            // return nRF24L01 status uchar
}

/*******************************************************************************
* 功能描述 : 将NRF24L01的FIFO缓存区的数据读取存放到用户的缓存区中
* 函数属性 : 内部
* 输入参数 : dat: 指令+寄存器地址，pBuf：为待读出数据地址，uchars：读出数据的个数
* 返回参数 : 
* 函数详解 : 

	接收缓冲区访问函数：主要用来读取NRF24L01的FIFO缓冲区中的数据。基本思路就是通过
READ_REG 命令把数据从NRF24L01的FIFO缓存区（RD_RX_PLOAD）中读出并存到用户的缓存区
（数组）

*******************************************************************************/

uint Read_From_FIFO_Buf(uchar dat, uchar *pBuf, uchar uchars)
{
		uint status,uchar_ctr;
		
		CSN = 0;                    		// Set CSN low, init SPI tranaction
		status = SPI_ReadWrite(dat);       		// Select register to write to and read status uchar
		
		for(uchar_ctr=0;uchar_ctr<uchars;uchar_ctr++)
		{
			  pBuf[uchar_ctr] = SPI_ReadWrite(0);    // 
		}
			
		CSN = 1;                           
		
		return(status);                    // return nRF24L01 status uchar
}

/*******************************************************************************
* 功能描述 : 将用户缓存区的数据写入到NRF24L01的FIFO缓存区
* 函数属性 : 内部
* 输入参数 : dat: 指令+寄存器地址，pBuf：为待读出数据地址，value：读出数据的个数
* 返回参数 : 
* 函数详解 : 

	主要用来把用户缓存区（数组）里的数写入到NRF24L01的FIFO缓冲区中。基本思
路就是通过WRITE_REG 命令把数据存到发射 FIFO（WR_TX_PLOAD）中去

*******************************************************************************/

uint Write_To_FIFO_Buf(uchar dat, uchar *pBuf, uchar uchars)
{
		uint status,uchar_ctr;//定义中间变量
		
		CSN = 0;            //SPI使能       
		status = SPI_ReadWrite(dat);   
		for(uchar_ctr=0; uchar_ctr<uchars; uchar_ctr++) //将用户数据依次写入fifo缓冲区
		{
			  SPI_ReadWrite(*pBuf++);
		}
			
		CSN = 1;           //关闭SPI
		return(status);    // 
}

/*******************************************************************************
* 功能描述 : 配置为数据接收模式（要使用接收模式时需要调用该函数） 
* 函数属性 : 外部
* 输入参数 : 无
* 返回参数 : 无
* 函数详解 : 
*******************************************************************************/

void SetRX_Mode(void)
{
		CE=0;
		ReadWrite_Reg(WRITE_REG + CONFIG, 0x0f);  // IRQ收发完成中断响应，16位CRC	，主接收
		CE = 1; 
		Delay_us(130);
}

/*******************************************************************************
* 功能描述 : 接收发射模块的数据，并将数据放入rx_buf用户接收缓冲区中
* 函数属性 : 外部
* 输入参数 : 用户的缓冲区的地址
* 返回参数 : 
* 函数详解 : 
*******************************************************************************/

unsigned char NRF24L01_RxPacket(unsigned char* Rx_Buf)
{
    unsigned char revale=0;
		
		SetRX_Mode();//设置为接收模式
		
		sta=Read_Reg(STATUS);	// 读取状态寄存其来判断数据接收状况
		if(Rx_DR)				// 判断是否接收到数据
		{
			  CE = 0; 			//SPI使能
			  Read_From_FIFO_Buf(RD_RX_PLOAD,Rx_Buf,Tx_Data_Width);// read receive payload from RX_FIFO buffer
			  revale =1;			//读取数据完成标志
		}
		ReadWrite_Reg(WRITE_REG+STATUS,sta);   //接收到数据后RX_DR,TX_DS,MAX_PT都置高为1，通过写1来清楚中断标志
		return revale;
}

/*******************************************************************************
* 功能描述 : 将tx_buf用户缓冲区中的数据发送给接收模块
* 函数属性 : 外部
* 输入参数 : 用户的缓冲区的地址
* 返回参数 : 发送的结果
* 函数详解 : 
*******************************************************************************/

uchar NRF24L01_TxPacket(unsigned char * Tx_Buf)
{
		unsigned char revale=0;
		CE=0;			//StandBy I模式	
		Write_To_FIFO_Buf(WRITE_REG + RX_ADDR_P0, Tx_Address, Tx_Address_Width); // 装载接收端地址
		Write_To_FIFO_Buf(WR_TX_PLOAD, Tx_Buf, Tx_Data_Width); 			 // 装载数据	
		ReadWrite_Reg   (WRITE_REG + CONFIG, 0x0e);   		 // IRQ收发完成中断响应，16位CRC，主发送
		CE=1;		 //置高CE，激发数据发送
		Delay_us(10);
		
		ReadWrite_Reg(WRITE_REG+STATUS,0XFF); //清楚标志位寄存器
		sta=Read_Reg(STATUS);	// 读取状态寄存其来判断数据接收状况
		if(!Tx_DS)//如果发送成功，返回1（发送成功后标志位TX_DS被置0）
		{
			  revale = 1;
		}
		return revale; //返回发送结果
}

/*******************************************************************************
* 功能描述 : NRF24L01初始化
* 函数属性 : 外部
* 输入参数 : 无
* 返回参数 : 无
* 函数详解 : 
*******************************************************************************/

void init_NRF24L01(void)
{
  
	CE=0;    // 使能
	CSN=1;   // Spi disable 
	SCK=0;   // Spi clock line init high
	
	Write_To_FIFO_Buf(WRITE_REG + TX_ADDR,    Tx_Address, Tx_Address_Width);//写本地地址
	Write_To_FIFO_Buf(WRITE_REG + RX_ADDR_P0, Rx_ADDRESS, Rx_Address_Width);//写接收端地址
	ReadWrite_Reg(WRITE_REG + EN_AA, 0x01); //频道0自动	ACK应答允许	
	ReadWrite_Reg(WRITE_REG + EN_RXADDR, 0x01); //允许接收地址只有频道0，如果需要多频道可以参考Page21  
	ReadWrite_Reg(WRITE_REG + RF_CH, 0);  //设置信道工作为2.4GHZ，收发必须一致
	ReadWrite_Reg(WRITE_REG + RX_PW_P0, Rx_Data_Width);//设置接收数据长度，本次设置为RX_PLOAD_WIDTH个字节
	ReadWrite_Reg(WRITE_REG + RF_SETUP, 0x07); //设置发射速率为1MHZ，发射功率为最大值0dB	
	ReadWrite_Reg(WRITE_REG + CONFIG, 0x0e); //IRQ收发完成中断响应，16位CRC，主发送
}

