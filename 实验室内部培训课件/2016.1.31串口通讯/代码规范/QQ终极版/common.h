#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <strings.h>
#include <stdlib.h>
#include <time.h>
#include <dirent.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <pwd.h>
#include <signal.h>
#include <shadow.h>
#include <ctype.h>
#include <pthread.h>
#include <sys/resource.h>
#include <sys/select.h>
#include <poll.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SER_PORT 8888 //设置服务器端口
#define IP "127.0.0.1"

void my_err(const char *err_string, int line);



/******************************************************************************************
 *                  #####   枚举体、结构体、表头 类型声明   #####
 * *****************************************************************************************/

/* 登录错误校验 */
typedef enum lg_flag
{
	GETID,
	GETPWD,
	VALID,
	INVALID,
	VALPWD,
	INVALPWD,
	ONLINE,
	OFFLINE
} LgFlag;


/* 消息包类型 */
typedef enum msg_t
{
	LOGIN = 1,	//登录
	LOGOUT,		//注销
	REGIST,		//注册
	ADDFRND,    //加好友
	CHATFRND, 	//私聊
	CHATGRUP, 	//群聊
	FRNDLIST,	//所有好友列表
	GRUPLIST,	//群列表
	ONLIST,    	//在线列表
	FINDPWD,	//找回密码
	STAT,       //在线/离线状态
	CLOSE       //关闭客户端
} Msg_t;

//登录消息结构
typedef struct login
{
	char id[32];
	char passwd[32];
} Login;

//注册用户
//找回密码
typedef struct usr
{
	char name[32];
	char passwd[32];
	char id[32];
	char sex[4];
	char age[4];
	char city[32];
	char question[32];
	char answer[32];
} Regist, FindPwd;

//状态
typedef struct offmsg
{
	char id[32];
	LgFlag stat;
} Stat;


//好友信息
//在线好友
//消息来源
//消息目的地
typedef struct list
{
	char id[32];
	char name[32];
	int  cfd;
	LgFlag stat;
} ListAll, ListOn, From, Dest;


//消息包
typedef struct msgpack
{
	Msg_t 	msgtype; //消息类型
	int 	msgsize; //消息大小
	From	from;	 //来源
	Dest 	dest;	 //目的地
	char 	data[1024];  //消息内容
} MsgPack;

/* 链表节点 */
typedef struct node
{
	void        *data;
	struct node *next;
	struct node *prev;
} Node;

/* 用户数据库 表头 */
typedef struct table
{
	int     num;
	int     size;
	Node 	head;
} Table;


/* 用户详细信息 */
typedef struct user
{
	char name[32];
	char passwd[32];
	char id[32];
	char sex[4];
	char age[4];
	char city[32];
	char question[32];
	char answer[32];
	Table *frnd;
	Table *grup;
	int  newfd;
	LgFlag  online;
} StuUsr;


Table *UsrTable;
Table *FrdTable;
Table *OliTable;



/******************************************************************************************
 *                  #####   结构体大小、错误处理 宏定义 #####
 * *****************************************************************************************/
/* 错误处理 */
#define ERRP(msg, lable)        \
	do{                         \
		perror(msg);            \
		goto lable;             \
	}while(0)

/* 读取姓名/密码 字符串 */
#define GETSTR(str, len)            \
	do{                             \
		fgets(str, len, stdin);     \
		str[strlen(str)-1] = '\0';  \
	}while(0)

/* 读取命令 选项 */
#define GETCMD(cmd)                 \
	do{                             \
		cmd = getchar();            \
		while(getchar() != '\n');   \
	}while(0)

#define  MSG_SIZE    sizeof(MsgPack) //消息包大小


/* 创建空表 */
Table *create_empty_table(int size);

/* 在尾部插入一个用户*/
void insert_to_tail(void *data, Table *tb);





#endif

