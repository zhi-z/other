#ifndef __CLIENT_H__
#define __CLIENT_H__




//菜单
void menu(void);

//注册
int regist(void);

//找回密码
void *findpwd(void *msgpack);

//登录
void login(void);

//接收登录信息
int get_id(LgFlag flg, char *data);

//登录成功
void login_succ(MsgPack *msg);




// 建立私聊
int chatfrnd(int row);

// 发送私聊消息
void *send_frnd_msg(Msg_t type);

// 接收私聊消息
void *recv_frnd_msg(MsgPack *msg);

// 建立群聊
int chatgrup(int row);

// 发送群聊消息
void *send_grup_msg(Msg_t type);

// 接收群聊消息
void *recv_grup_msg(MsgPack *msg);



//添加好友
int send_addfrnd(void);

//接收好友请求
int recv_addfrnd(MsgPack *msg);

//请求好友列表
void ask_frnd_list(void);

//接收好友列表
void recv_frnd_list(MsgPack *msg);

//显示好友列表
void frndlist_view(void);

//更新在线好友列表/私聊
void stat_upd(MsgPack *msg);

//接收群列表
void ask_grup_list(void);

//接收群列表
void recv_grup_list(MsgPack *msg); 

//显示群列表
void gruplist_view(void);



//接收消息处理
void *recv_handler(void *argv);

//中断处理
void sig(int argv);

//链接测试处理
void TestSer(int argv);

//初始化
int init(void);

#endif
