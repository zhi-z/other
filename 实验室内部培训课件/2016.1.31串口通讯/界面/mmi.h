
/*******************************************************************************
* 文件名 : MMI.H

* 描述   :界面头文件
           
*******************************************************************************/

#ifndef __MMI_H__
#define __MMI_H__

#define RETURNKEY_1	 1
#define RETURNKEY_2	 2
#define RETURNKEY_3	 3
#define RETURNKEY_4	 4

#define FIRSTUISTATE_1   5
#define FIRSTUISTATE_2   6
#define FIRSTUISTATE_3	 7
#define SECONDUISTATE_1	 8
#define SECONDUISTATE_2	 9

/*延时*/
void Delay10ms(unsigned char x);		//@11.0592MHz
/*按键返回值*/
int RemapKey(void);
/*按键输入处理*/
void KeyProcess(unsigned char key);
/*初始化界面*/
void Init_UI();

void FirstUI_1(unsigned char key);
void FirstUI_2(unsigned char key);
void FirstUI_3(unsigned char key);
void SecondUI_1(unsigned char key);
void SecondUI_2(unsigned char key);

#endif /*__MMI_H__*/


