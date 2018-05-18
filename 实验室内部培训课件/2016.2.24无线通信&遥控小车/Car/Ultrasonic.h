/*******************************************************************************
* 文件名 : ultrasonic.h
* 实现功能:
* 版本     作者            日期            说明
* V1.0                   2015/08/5      初始版本

* 描述   : MCU: STC89c52     晶振: 11.0592   MHz
           
*******************************************************************************/

#ifndef __ultrasonic_H_
#define __ultrasonic_H_

#include "intrins.h"
#include "reg52.h"

sbit Trig = P3^5;
sbit Echo = P3^3;

extern int ultrasonic();	  //启动超声波，返回超声波测量的值


#endif