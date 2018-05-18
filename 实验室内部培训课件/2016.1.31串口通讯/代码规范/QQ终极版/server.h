#ifndef __SERVER_H__
#define __SERVER_H__

#include "common.h"


/* 用户文件路径 */
#define USR_PATH ("./data/usr_info")

/**************************************************************************************************************
 *
 *											#####	公共函数声明	#####
 *
 *************************************************************************************************************/

/* 遍历所有用户信息 */
void print_all(Table *tb);	

/* 打印一个用户结构体 */
void print_one(void *data);	

/* 根据ID获取用户结构体信息 */
void *get_usrinfo(char *id, void *ptmp); 

//用户登录
int login(MsgPack *msg, int fd);

//用户注销
int logout(MsgPack *msg);

//注册
void *regist(MsgPack *msg, int fd);

//加好友
int addfrnd(MsgPack *msg, int fd); //加好友

//私聊
void *chatfrnd(MsgPack *msg);

//群聊
void *chatgrup(MsgPack *msg);

//好友列表
void *frndlist(MsgPack *msg, int fd);

//群列表
void *gruplist(MsgPack *msg, int fd);

//在线列表
void *onlist(MsgPack *msg);

//离线处理自身
int  offline(int conn_fd, char *get_id);

// 上线、下线 通知好友
int  off_on_noti(char *off_id, LgFlag flg);

//找回密码
int findpwd(MsgPack *msg, int fd);

//新连接客户端 处理子线程
void *new_cli(void *args);

//初始化
int init(void);

//守护进程
void deamon(void);

#endif


























#if 0
/* 按所给项删除学生/教师 */
void delete_match_item(void *del_item, enumItem item, Table *tb); 

/* 修改学生/教师信息 */
void update_info(void *old_data, enumItem old_item, void *new_data, enumItem new_item, Table *tb);

/* 销毁表 */
void destory_table(Table *tb);	

/* 整型转字符串 */
void intToStr(int iNum, char szNum[]);

/* 判断是否usr是用户名，若不是，则获取登录者用户名 */
void *is_name_or_id(void *usr, enumItem usritem);



/*****************************************************************************************************************************
 *											####	外部声明函数	####
 * **************************************************************************************************************************/
/* 比较查找项与教师结构体中对应项是否相等，在 "teacher.h" 中声明 */
extern int is_T_item_equal( void *cmp_Data, enumItem item, Teacher *tea);

/* 比较查找项与学生结构体中对应项是否相等 ,在 "student.h" 中声明 */
extern int is_S_item_equal(void *cmp_Data, enumItem item, Student *stu);

/* 按所给参数修改教师信息  子函数息 ，在 "teacher.h" 中声明*/
extern void update_T_item(void *new_data, enumItem new_item, Teacher *tea);

/* 按所给参数修改学生信息  子函数息 ,在 "student.h" 中声明 */
extern void update_S_item(void *new_data, enumItem new_item, Student *stu);

#endif //注释



