#include "common.h"
#include <termios.h>
#include "client.h"

struct termios newattr, oldattr;//屏幕中断码流的设
int conn_fd;
struct sockaddr_in seraddr;

Table *frndTb = NULL;
Table *grupTb = NULL;

volatile int dload_frnd_list_ok = 0;
volatile int dload_grup_list_ok = 0;

int flg_answer = 0;//应答好友请求
char destid[32]; //请求好友ID
char my_name[32]; //用户姓名
char my_id[32]; //用户QQ
char frndid[32]; //与哪个好友聊天
char grupid[32]; //与哪群聊天
int chatrow  = 7; //聊天行数
int chatcol  = 10;//聊天列数

void menu_view(void)
{
	system("clear");
	printf("     \e[35m\e[43m*********************\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m\e[34m       ╭︿︿︿╮红猪\e[0m\e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m\e[33m      {/ o  o /}   \e[0m\e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m\e[32m       ( (00) )    \e[0m\e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m\e[35m        ︶︶︶     \e[0m\e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                   \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                   \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                   \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                   \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                   \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                   \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                   \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                   \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                   \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                   \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                   \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                   \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*********************\e[0m\n");	
	fflush(NULL);    // 清缓
	return;
}

//菜单函数
void menu(void)
{
	char * arr[3] = {"登 录", "注 册", "退 出"};
	int row = 12;

	menu_view();

	printf("\e[14;12H注 册");
	printf("\e[16;12H退 出");
	printf("\e[12;12H\e[7m登 录\e[0m");
	fflush(NULL);
	tcgetattr(0, &oldattr);
	cfmakeraw(&newattr);
	while(1)
	{
		char key[3]= {0};
		memset(key, 0, 3);
		tcsetattr(0, TCSANOW, &newattr);
		int ret = read(0, key, 3);
		tcsetattr(0, TCSANOW, &oldattr);

		if(ret == 1)
		{
			if(key[0] == 3) //Ctrl+C按键
			{
				close(conn_fd);
				printf("已退出！\n感谢您使用红猪聊天系统，\n欢迎您再次使用！");
				exit(0);
			}
			else if(key[0] == 13) // Enter按键
			{
				switch( row )
				{
					case 12:
						login();
						menu_view();
						printf("\e[14;12H注 册");
						printf("\e[16;12H退 出");
						printf("\e[12;12H\e[7m登 录\e[0m");
						fflush(NULL);
						row = 12;
						break;
					case 14:
						if (regist() == 0)
						{
							login();
						}
						menu_view();
						printf("\e[14;12H注 册");
						printf("\e[16;12H退 出");
						printf("\e[12;12H\e[7m登 录\e[0m");
						fflush(NULL);
						row = 12;
						break;
					case 16:
						printf("\n已退出！\n感谢您使用红猪聊天系统，\n欢迎您再次使用！\n");
						exit(0);
						break;
					default:
						break;
				}
			}
		}
		else if(ret == 3)
		{
			if(key[0] == 27 && key[1] == 91 && key[2] == 65) //Up按键
			{
				if(row > 12)
				{
					printf("\e[%d;12H%s", row, arr[(row-12)/2]);
					row -= 2;
					printf("\e[%d;12H\e[7m%s\e[0m",row,arr[(row-12)/2]);
					fflush(NULL);
				}
			}
			else if(key[0] == 27 && key[1] == 91 && key[2] == 66) //Down按键
			{
				if(row < 16)
				{
					printf("\e[%d;12H%s",row,arr[(row-12)/2]);
					row += 2;
					printf("\e[%d;12H\e[7m%s\e[0m",row,arr[(row-12)/2]);
					fflush(NULL);
				}
			}
		}
	}
}



void login_view(void)
{
	system("clear");
	printf("     \e[35m\e[43m*********************\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m       ╭︿︿︿╮红猪\e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m      {/ o  o /}   \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m       ( (00) )    \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m        ︶︶︶     \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                   \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                   \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                   \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                   \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                   \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                   \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                   \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                   \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                   \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                   \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                   \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                   \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*********************\e[0m\n");

	printf("\e[10;8HQQ号:");
	printf("\e[11;8H密码:");
	printf("\e[12;8H登录");
	printf("\e[13;8H返回");
	printf("\e[14;8H忘记密码");

	printf("\e[10;8H\e[7mQQ号:\e[0m");
	fflush(NULL);

	return;
}

//用户登录函数
void login(void)
{
	MsgPack msg; //创建空包
	memset(&msg, 0, MSG_SIZE);

	msg.msgtype = LOGIN; //设置包类型
	Login lg_info;

	char *arr[5] = {"QQ号:", "密码:", "登录", "返回", "忘记密码"};
	int row = 10;

	login_view();

	tcgetattr(0, &oldattr);
	cfmakeraw(&newattr);
	int flg = 0;

	do{
		char key[3]= {0};
		memset(key, 0, 3);
		tcsetattr(0,TCSANOW,&newattr);
		int ret = read(0, key, 3);
		tcsetattr(0,TCSANOW,&oldattr);

		if(ret == 1)
		{
			if(key[0] == 3) //Ctrl+C按键
			{
				close(conn_fd);
				printf("已退出！\n感谢您使用红猪聊天系统，\n欢迎您再次使用！");
				exit(0);
			}
			else if(key[0] == 13) //Enter按键
			{	
				switch( row )
				{	
					case 10: //QQ号码
						printf("\e[10;8HQQ号:        ");
						printf("\e[10;8HQQ号:");
						fflush(NULL);
						GETSTR(lg_info.id, 32);
						strcpy(my_id, lg_info.id); //存储QQ号
						strcpy(msg.from.id, lg_info.id); //存储QQ号
						printf("\e[10;23H");
						fflush(NULL);
						break;

					case 11: //在 密码处 按Enter
						printf("\e[11;8H密码:        ");
						printf("\e[11;8H密码:");
						fflush(NULL);
						GETSTR(lg_info.passwd, 32);
						printf("\e[11;23H");
						fflush(NULL);
						break;

					case 12: //登录		
						printf("\e[7;8H正在登录");
						fflush(NULL);  
						int i;
						for (i = 0; i < 5; i++)
						{
							usleep(200000);
							printf(">");
							fflush(NULL);  
						}
						memmove(&(msg.data), &lg_info, sizeof(Login));
						if (send(conn_fd, &msg, MSG_SIZE, 0) < 0)
						{
							my_err("send", __LINE__);
						}
						if (recv(conn_fd, &msg, MSG_SIZE, 0) < 0)
						{
							my_err("recv", __LINE__);
						}						
						if (login_result(&msg) == 0)	//登录请求 反馈	
						{
							login_succ(&msg);
						}			
						usleep(400000);
						login_view();
						row = 10;
						break;

					case 13: //返回
						return;
						break;

					case 14: //找回密码
						findpwd(NULL);
						login_view();
						row = 10;
						break;

					default:
						break;
				}
				printf("\e[%d;8H\e[7m%s\e[0m", row, arr[row-10]);
				fflush(NULL);
			}
		}
		else if(ret == 3)
		{
			if(key[0] == 27 && key[1] == 91 && key[2] == 65) //Up按键
			{
				if(row > 10)
				{
					printf("\e[%d;8H%s",row,arr[row-10]);
					fflush(NULL);
					row--;
					printf("\e[%d;8H\e[7m%s\e[0m",row,arr[row-10]);
					fflush(NULL);
				}
			}
			else if(key[0] == 27 && key[1] == 91 && key[2] == 66) //Down按键
			{
				if(row < 14)
				{
					printf("\e[%d;8H%s",row,arr[row-10]);
					fflush(NULL);
					row++;
					printf("\e[%d;8H\e[7m%s\e[0m",row,arr[row-10]);
					fflush(NULL);
				}
			}
		}
	} while(1);

	return;
}

//登录请求 反馈
int login_result(MsgPack *msg)
{
	//打印反馈结果
	printf("\e[7;8H%s", msg->data);
	fflush(NULL);    // 清缓
	if (strcmp(msg->data, "Y(^o^)Y 欢迎回来") == 0)
	{
		return 0;
	}
	else
		return -1;
}


//登录成功
void login_succ(MsgPack *msg)
{
	system("clear");
	printf("     \e[35m\e[43m*********************************************************************************************************\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                                聊天记录                                 \e[35m\e[43m*\e[0m\e[34m          ╭︿︿︿╮           \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                                                                         \e[35m\e[43m*\e[0m\e[33m         {/ o  o /}          \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                                                                         \e[35m\e[43m*\e[0m\e[32m          ( (oo) )           \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                                                                         \e[35m\e[43m*\e[0m\e[35m           ︶︶︶            \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                                                                         \e[35m\e[43m*\e[0m\e[31m         红猪QQ出品          \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                                                                         \e[35m\e[43m*\e[0m     当前用户:               \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                                                                         \e[35m\e[43m******************************\e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                                                                         \e[35m\e[43m*\e[0m\e[32m   我的好友   |   我的QQ群   \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                                                                         \e[35m\e[43m*-----------------------------\e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                                                                         \e[35m\e[43m*\e[0m       \e[31m在线好友:\e[0m     \e[31m人\e[0m      \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                                                                         \e[35m\e[43m*\e[0m                             \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                                                                         \e[35m\e[43m*\e[0m       \e[34m昵称      QQ号\e[0m        \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                                                                         \e[35m\e[43m*\e[0m                             \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                                                                         \e[35m\e[43m*\e[0m                             \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                                                                         \e[35m\e[43m*\e[0m                             \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                                                                         \e[35m\e[43m*\e[0m                             \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                                                                         \e[35m\e[43m*\e[0m                             \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                                                                         \e[35m\e[43m*\e[0m                             \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                                                                         \e[35m\e[43m*\e[0m                             \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                                                                         \e[35m\e[43m*\e[0m                             \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                                                                         \e[35m\e[43m*\e[0m                             \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                                                                         \e[35m\e[43m*\e[0m                             \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                                                                         \e[35m\e[43m*\e[0m                             \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                                                                         \e[35m\e[43m*\e[0m                             \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                                                                         \e[35m\e[43m*\e[0m                             \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m***************************************************************************\e[0m                             \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                                 控制窗口                                \e[35m\e[43m*\e[0m                             \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                                                                         \e[35m\e[43m*\e[0m                             \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                                                                         \e[35m\e[43m*\e[0m                             \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                                                                         \e[35m\e[43m*\e[0m                             \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                                                                         \e[35m\e[43m******************************\e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                                                                         \e[35m\e[43m*\e[0m          主菜单             \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                                                                         \e[35m\e[43m*\e[0m\e[34m   注销  添加好友  删除好友  \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m                                                                         \e[35m\e[43m*\e[0m\e[34m   退出  个人资料  文件    \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*********************************************************************************************************\e[0m\n");

	/* 控制：28；40*/
	/* 好友：9；81 */
	/* QQ群：9；96 */
	/* 菜单：33；91*/
	/* 注销：34；84 */
	/* 添友：34；90 */
	/* 删友：34；100*/
	/* 查找：35；84 */
	/* 资料：35；90 */
	/* 帮助：35；100*/
	char *arr1[] = {"控制窗口", "   我的好友   ", "   我的QQ群   ", "主菜单"};
	char *arr2[] = {"注销", "添加好友", "删除好友", "退出", "个人资料", "文件"};
	int menuloca_x[6] = {84, 90, 100, 84, 90, 100};
	int menuloca_y[6] = {34, 34, 34, 35, 35, 35};
	int menu_ch = 0;

	printf("\e[9;81H\e[43m   我的好友   \e[0m");
	printf("\e[28;40H\e[7m控制窗口\e[0m");
	fflush(NULL);  

	ask_frnd_list();
	frndlist_view(); //打印好友列表
	ask_grup_list();

	printf("\e[30;8H#");
	fflush(NULL);  

	int mem_frnd = frndTb->num; //好友总数
	int mem_grup = grupTb->num; //QQ群总数 
	int  row = 30;
	int  col = 8;
	int flg_where = 0;	
	int flg_frnd = 0;
	int flg_grup = 0;
	int flg_menu = 0;
	int chating  = 0; //私聊
	int chatjoin = 0; //群聊

	//登录成功  创建新线程
	pthread_t tid;
	pthread_create(&tid, NULL, recv_handler, NULL); //子线程
	pthread_detach(tid); //线程分离

	int ret;
	char key[3]= {0};
	tcgetattr(0, &oldattr);
	cfmakeraw(&newattr);	
	while(1)
	{	
		mem_frnd = frndTb->num; //好友总数    
		memset(key,0,3);
		tcsetattr(0,TCSANOW,&newattr);
		ret = read(0,key,3);
		tcsetattr(0,TCSANOW,&oldattr);

		if(ret == 1)
		{
			if (key[0] == 27) //ESC 按键
			{
				if ((flg_frnd == 1) || (flg_grup == 1))
					printf("\e[%d;84H\e[31m  \e[0m", row);
				else if(flg_menu == 1)
					printf("\e[%d;%dH\e[34m%s\e[0m", menuloca_y[menu_ch], menuloca_x[menu_ch], arr2[menu_ch]);
				else if (chating == 1)
				{
					printf("\e[3;10H\e[35m您已退出私聊, 选择好友即可再次进入。\e[0m");
					sleep(1);
					printf("\e[3;10H\e[35m                                   \e[0m");
				}
				else if (chatjoin == 1)
				{
					printf("\e[3;10H\e[35m您已退出聊天室, 选择群即可再次进入。\e[0m");
					sleep(1);
					printf("\e[3;10H\e[35m                                   \e[0m");
				}
				printf("\e[30;8H#");
				fflush(NULL); 

				flg_frnd = 0; //退出好友选择
				flg_grup = 0; //退出群选择
				flg_menu = 0; //退出菜单
				chating  = 0; //退出私聊
				chatjoin = 0; //退出群聊
			}
			else if(key[0] == 3) //Ctrl+C按键
			{
				MsgPack close_msg;
				memset(&close_msg, 0, MSG_SIZE);
				close_msg.msgtype = CLOSE;
				strcpy(close_msg.from.id, my_id);
				strcpy(close_msg.from.name, my_name);
				send(conn_fd, &close_msg, MSG_SIZE, 0);			
				close(conn_fd);
				printf("已退出！\n感谢您使用红猪聊天系统，\n欢迎您再次使用！");
				exit(0);
			}
			else if(key[0] == 13) //Enter按键
			{	
				switch( flg_where ) //在那个区域
				{
					case 0: //控制
						if (chating == 1)
						{
							send_frnd_msg(CHATFRND);
						}
						else if (chatjoin == 1)
						{
							send_grup_msg(CHATGRUP);
						}
						else if(flg_answer == 1)
						{
							MsgPack msg;
							bzero(&msg, MSG_SIZE);
							msg.msgtype = ADDFRND;
							GETSTR(msg.data, 10); //应答 
							printf("\e[30;8H#\e[s                                        \e[u");
							fflush(NULL);        
							strcpy(msg.dest.id, destid);
							strcpy(msg.from.id, my_id);
							send(conn_fd, &msg, MSG_SIZE, 0);
							if (strcmp(msg.data, "同意") == 0)
							{                              
								ask_frnd_list();
								frndlist_view(); //打印好友列表
							}
							flg_answer = 0;   
						}
						printf("\e[30;8H#");
						fflush(NULL);  						
						break;

					case 1: //私聊
						if (flg_frnd == 1) //说明已经选择了
						{
							int ret = chatfrnd(row);
							if (ret == 0 || ret == 2)//和好友私聊，好友在线
							{
								chating = 1; //进入私聊
								flg_where = 0; //切换到控制窗口
								printf("\e[9;81H%s", arr1[1]); 
								printf("\e[28;40H\e[7m%s\e[0m", arr1[0]);
								printf("\e[%d;84H\e[31m  \e[0m", row);
								printf("\e[29;8H\e[31m温馨提示: 按Enter编辑消息， Enter发送消息\e[0m");
								printf("\e[30;8H#");
								fflush(NULL);   
							}
						}
						if ((flg_frnd == 0) && (mem_frnd > 0))
						{

							row = 14;
							printf("\e[s\e[%d;84H\e[31m->\e[0m\e[u", 14);
							fflush(NULL);
							flg_frnd = 1;	
						}						
						break;

					case 2: //QQ群
						if (flg_grup == 1) //说明已经选择组菜单
						{
							int ret = chatgrup(row);
							if (ret == 0)
							{
								chatjoin = 1; //群聊
								flg_where = 0; //切换到控制窗口
								printf("\e[9;81H%s", arr1[1]); 
								printf("\e[28;40H\e[7m%s\e[0m", arr1[0]);
								printf("\e[%d;84H\e[31m  \e[0m", row);
								printf("\e[29;8H\e[31m温馨提示: 按Enter编辑消息， Enter发送消息\e[0m");
								printf("\e[30;8H#");
								fflush(NULL);    // 清缓
							}
						}
						if ((flg_grup == 0) && (mem_grup > 0)) //还未选择组菜单
						{
							row = 14;
							printf("\e[s\e[%d;84H\e[31m->\e[0m\e[u", 14);
							fflush(NULL);
							flg_grup = 1;	
						}	
						break;

					case 3:	//菜单
						if (flg_menu == 1)
						{
							switch (menu_ch)
							{						    
								case 0:
									if (1)  //注销
									{
										MsgPack logout_msg;
										memset(&logout_msg, 0, MSG_SIZE);
										logout_msg.msgtype = LOGOUT;
										strcpy(logout_msg.from.id, my_id);
										send(conn_fd, &logout_msg, MSG_SIZE, 0);							                   
										return; //退出到登录界面
									}
									break;
								case 1:
									send_addfrnd();	//添加好友			
									break;
								case 2:
									break;
								case 3:    //退出
									if (1)
									{
										MsgPack close_msg;
										memset(&close_msg, 0, MSG_SIZE);
										close_msg.msgtype = CLOSE;
										strcpy(close_msg.from.id, my_id);
										strcpy(close_msg.from.name, my_name);
										send(conn_fd, &close_msg, MSG_SIZE, 0);			
										close(conn_fd);
										printf("\n已退出登录！\n感谢您使用红猪聊天系统，\n欢迎您再次使用！\n");
										exit(0);
									}			                        
									break;
								case 4:
									break;
								case 5:
									break;  						    	
							}
						}
						if (flg_menu == 0)
						{
							flg_menu = 1;
							menu_ch = 0;
							printf("\e[%d;%dH\e[7m%s\e[0m", menuloca_y[0], menuloca_x[0], arr2[0]);
							fflush(NULL);
						}						
						break;	

					default:
						break;
				}
			}
		}
		else if(ret == 3)
		{
			if(flg_frnd == 1 || flg_menu == 1 || flg_grup == 1)
			{
				if(key[0] == 27 && key[1] == 91 && key[2] == 65) // Up 按键
				{
					if (((flg_frnd == 1) || (flg_grup == 1)) && (row > 14)) 
					{
						printf("\e[%d;84H\e[31m  \e[0m", row);
						row--;
						printf("\e[%d;84H\e[31m->\e[0m", row);
						fflush(NULL);
					}
					else if((flg_menu == 1) && (menu_ch > 0))
					{
						printf("\e[%d;%dH\e[34m%s\e[0m", menuloca_y[menu_ch], menuloca_x[menu_ch], arr2[menu_ch]);
						menu_ch--;
						printf("\e[%d;%dH\e[7m%s\e[0m", menuloca_y[menu_ch], menuloca_x[menu_ch], arr2[menu_ch]);
						fflush(NULL);
					}
				}
				else if (key[0] == 27 && key[1] == 91 && key[2] == 66) // Down 按键
				{
					if (((flg_frnd == 1) && (row < (14 + mem_frnd - 1))) || ((flg_grup == 1) && (row < (14 + mem_grup - 1))))
					{
						printf("\e[%d;84H\e[31m  \e[0m", row);
						row++;
						printf("\e[%d;84H\e[31m->\e[0m", row);
						fflush(NULL);
					}
					else if((flg_menu == 1) && (menu_ch < 5))
					{
						printf("\e[%d;%dH\e[34m%s\e[0m", menuloca_y[menu_ch], menuloca_x[menu_ch], arr2[menu_ch]);
						menu_ch++;
						printf("\e[%d;%dH\e[7m%s\e[0m", menuloca_y[menu_ch], menuloca_x[menu_ch], arr2[menu_ch]);
						fflush(NULL);
					}
				}
			}
			else
			{
				if( key[0] == 27 && key[1] == 91 && key[2] == 67) // Right 按键
				{
					if (flg_where == 0)
					{
						flg_where = 1;
						printf("\e[28;40H%s", arr1[0]);
						printf("\e[9;81H\e[7m%s\e[0m", arr1[1]);
						fflush(NULL);
					}
					else if (flg_where == 1)
					{
						flg_where = 2;
						printf("\e[9;81H%s", arr1[1]);
						printf("\e[9;96H\e[7m%s\e[0m", arr1[2]);
						fflush(NULL);
						gruplist_view();
					}
					else if (flg_where == 2)
					{
						flg_where = 3;
						printf("\e[9;96H%s", arr1[2]);
						printf("\e[33;91H\e[7m%s\e[0m", arr1[3]);
						fflush(NULL);
					}
					else
						continue;
				}
				else if(key[0] == 27 && key[1] == 91 && key[2] == 68) // Left 按键
				{
					if (flg_where == 3)
					{
						flg_where = 2;
						printf("\e[33;91H%s", arr1[3]);
						printf("\e[9;96H\e[7m%s\e[0m", arr1[2]);
						fflush(NULL);
					}
					else if (flg_where == 2)
					{
						flg_where = 1;
						printf("\e[9;96H%s", arr1[2]);
						printf("\e[9;81H\e[7m%s\e[0m", arr1[1]);
						fflush(NULL);
						frndlist_view();
					}
					else if (flg_where == 1)
					{
						flg_where = 0;
						printf("\e[9;81H%s", arr1[1]);
						printf("\e[28;40H\e[7m%s\e[0m", arr1[0]);
						fflush(NULL);
					}
				}
			}
		}
	}
	return;
}

//添加好友
int send_addfrnd(void)
{
	MsgPack addfrnd_msg;
	memset(&addfrnd_msg, 0, MSG_SIZE);
	addfrnd_msg.msgtype = ADDFRND;
	strcpy(addfrnd_msg.from.id, my_id);
	strcpy(addfrnd_msg.from.name, my_name);
	strcpy(addfrnd_msg.data, "请求");

	printf("\e[26;30H 输入您要添加的好友QQ号码    ");
	printf("\e[30;8H#");
	fflush(NULL);    
	GETSTR( addfrnd_msg.dest.id, 32);    
	printf("\e[30;8H#\e[s                                        \e[u");
	fflush(NULL); 
	send(conn_fd, &addfrnd_msg, MSG_SIZE, 0);
	printf("\e[26;30H 已经向 %s 发送请求         ", addfrnd_msg.dest.id);
	fflush(NULL);
	return;
}

//接收加好友请求
int recv_addfrnd(MsgPack *msg)
{
	if (strcmp(msg->data, "请求") == 0)
	{      
		strcpy( destid, msg->from.id); 
		flg_answer = 1;
		printf("\e[26;30H 用户 %s %s请求添加您为好友       ", msg->from.name, msg->from.id);       
		printf("\e[30;8H#【同意】or【拒绝】:");
		fflush(NULL);               
		return;
	}
	else if (strcmp(msg->data, "同意") == 0)
	{    
		printf("\e[26;30H 用户 %s 同意加您为好友       ", msg->from.id);
		ask_frnd_list();
		frndlist_view();
		printf("\e[26;30H 已您为刷新好友列表          "); 
		fflush(NULL);    
	}
	else if (strcmp(msg->data, "拒绝") == 0)
	{
		printf("\e[26;30H 用户 %s 拒绝加您为好友       ", msg->from.id);
		fflush(NULL);       
	}

	return;
}

//发送私人消息
void *send_frnd_msg(Msg_t type)
{
	printf("\e[35;38H Waiting input... \e[0m");
	printf("\e[30;8H#");
	fflush(NULL);

	MsgPack msg;  //打包消息
	memset(&msg, 0, MSG_SIZE);
	msg.msgtype = type;
	strcpy(msg.from.id, my_id);

	strcpy(msg.dest.id, frndid); //添加目的ID	 

	strcpy(msg.from.name, my_name); 
	msg.from.cfd = conn_fd; 

	char buf[1000];			
	bzero(buf, sizeof(buf));
	int ret = read(0, buf, sizeof(buf)); //读取内容
	char buff[1024];
	bzero(buff, sizeof(buff));
	sprintf(buff, "【私】%s: ", my_name);
	strncat(buff, buf, ret);
	memcpy( msg.data, buff, 1024);

	if (chatrow > 25) //留最后一行不打印
	{
		while (chatrow >= 7) //清空消息
		{
			printf("\e[%d;%dH                                                                      ", chatrow--, chatcol);
		}
	}
	printf("\e[s\e[%d;%dH                                 \e[31m【私】ME:%s\e[u\e[0m", chatrow++, chatcol, buf);

	printf("\e[35;38H                 \e[0m");
	printf("\e[35;38H  Send finished  \e[0m");
	printf("\e[30;8H#                                                                       ");
	printf("\e[31;8H                                                                        ");
	printf("\e[32;8H                                                                        ");
	printf("\e[30;8H#");
	fflush(NULL);

	send(conn_fd, &msg, MSG_SIZE, 0);
	return;
}

//发送群组消息
void *send_grup_msg(Msg_t type)
{
	printf("\e[35;38H Waiting input... \e[0m");
	printf("\e[30;8H#");
	fflush(NULL);

	MsgPack msg; //打包消息
	memset(&msg, 0, MSG_SIZE);
	msg.msgtype = type;     
	strcpy(msg.from.id, my_id); 

	strcpy(msg.dest.id, grupid); //添加目的ID		

	strcpy(msg.from.name, my_name); 
	msg.from.cfd = conn_fd; 

	char buf[1000];
	bzero(buf, sizeof(buf));
	int ret = read(0, buf, sizeof(buf)); //读取内容
	char buff[1024];
	bzero(buff,  sizeof(buff));
	sprintf(buff, "【群】%s: ", my_name);
	strncat(buff, buf, ret);
	memcpy( msg.data, buff, 1024);

	if (chatrow > 25) //留最后一行不打印
	{
		while (chatrow >= 7) //清空消息
		{
			printf("\e[%d;%dH                                                                      ", chatrow--, chatcol);
		}
	}
	printf("\e[s\e[%d;%dH                                 \e[31m【群】ME:%s\e[u\e[0m", chatrow++, chatcol, buf);

	printf("\e[35;38H                 \e[0m");
	printf("\e[35;38H  Send finished  \e[0m");
	printf("\e[30;8H#                                                                       ");
	printf("\e[31;8H                                                                        ");
	printf("\e[32;8H                                                                        ");
	printf("\e[30;8H#");
	fflush(NULL);

	send(conn_fd, &msg, MSG_SIZE, 0);
	return;
}

//接收消息
void *recv_frnd_msg(MsgPack *msg)
{
	if (chatrow > 25) //留最后一行不打印
	{
		while (chatrow >= 7) //清空消息
		{
			printf("\e[%d;%dH                                                                      ", chatrow--, chatcol);
		}
	}
	printf("\e[s\e[%d;%dH\e[33m%s\e[u\e[0m", chatrow++, chatcol, msg->data);
	printf("\e[s\e[3;10H\e[35m您正在与 %s 私聊, 按ESC退出私聊\e[u\e[0m", msg->from.name);
	fflush(NULL);
	return;
}

//接收消息
void *recv_grup_msg(MsgPack *msg)
{
	if (chatrow > 25) //留最后一行不打印
	{
		while (chatrow >= 7) //清空消息
		{
			printf("\e[%d;%dH                                                                      ", chatrow--, chatcol);
		}
	}
	printf("\e[s\e[%d;%dH\e[33m%s\e[u\e[0m", chatrow++, chatcol, msg->data);
	if (strcmp(msg->dest.id, "654321") == 0)
		printf("\e[s\e[3;10H\e[35m您正在 %s 聊天室, 按ESC退出群聊\e[u\e[0m", "Y_Y黛丝li");
	if (strcmp(msg->dest.id, "765432") == 0)
		printf("\e[s\e[3;10H\e[35m您正在 %s 聊天室, 按ESC退出群聊\e[u\e[0m", "吃货￥无罪");
	fflush(NULL);
	return;
}


//注册帐号
void regist_view(void)
{
	printf("\e[10;8H您的昵称:\e[s");	
	printf("\e[13;8H您的密码:");
	printf("\e[14;8H确认密码:");
	printf("\e[11;8H密保问题:");
	printf("\e[12;8H密保答案:");
	printf("\e[15;8H立即注册");
	printf("\e[16;8H取消注册\e[u");
	fflush(NULL);

	return;
}

//注册QQ号函数
int regist(void)
{
	Regist st_regist_info;
	memset(&st_regist_info, 0, sizeof(Regist));
	char passwd_1[32];
	char passwd_2[32];
	MsgPack msg;
	msg.msgtype = REGIST;
	char *arr[7] = {"您的昵称:", "您的密码:", "确认密码:", "密保问题:", "密保答案:", "立即注册", "取消注册"};
	int row = 10;

	regist_view();

	tcgetattr(0, &oldattr);
	cfmakeraw(&newattr);
	while(1)
	{
		char key[3]= {0};
		memset(key, 0, 3);
		tcsetattr(0, TCSANOW, &newattr);
		int ret = read(0, key, 3);
		tcsetattr(0, TCSANOW, &oldattr);

		if(ret == 1)
		{
			if(key[0] == 3) //Ctrl+C按键
			{
				close(conn_fd);
				printf("\n已退出！\n感谢您使用红猪聊天系统，\n欢迎您再次使用！\n");
				exit(0);
			}
			else if(key[0] == 13) //Enter按键
			{	
				printf("\e[%d;8H%s\e[s          \e[u", row, arr[row-10]); //清空输入区域
				fflush(NULL);
				switch( row )
				{	
					case 10:
						GETSTR(st_regist_info.name, 32);
						break;

					case 11:
						GETSTR(passwd_1, 32);
						break;

					case 12:
						GETSTR(passwd_2, 32);
						if (strcmp(passwd_1, passwd_2) != 0)
						{
							printf("\e[s\e[7;8H两次密码不一致\e[u\e[0m");
							fflush(NULL);
							break;
						}
						strcpy(st_regist_info.passwd, passwd_2);
						break;

					case 13:
						GETSTR(st_regist_info.question, 32);
						break;

					case 14:
						GETSTR(st_regist_info.answer, 32);
						break;

					case 15:
						memmove(msg.data, &st_regist_info, sizeof(Regist));
						send(conn_fd, &msg, MSG_SIZE, 0); //发送注册信息			            
						char recv_buf[32];
						bzero(recv_buf, sizeof(recv_buf)); //接收注册得到的QQ号
						recv(conn_fd, recv_buf, sizeof(recv_buf), 0);

						menu_view();

						if (strlen(recv_buf) != 0)
						{
							printf("\e[6;8H%s", "恭喜您！注册成功。");
							printf("\e[7;8H    QQ:%s", recv_buf);
							printf("\e[9;8H%s", "请您牢记帐号密码");
							printf("\e[11;8H%s", "5秒后返回登录界面");
							fflush(NULL);
							sleep(5);
							return 0;  
						}			            			   			           
						break;

					case 16:
						return;
						break;

					default:
						break;
				}
				printf("\e[%d;8H%s", row, arr[row-10]); //回设光标位置
				fflush(NULL);
			}
		}
		else if(ret == 3)
		{
			if(key[0] == 27 && key[1] == 91 && key[2] == 65) //Up按键
			{
				if(row > 10)
				{
					printf("\e[%d;8H%s",row,arr[row-10]);
					fflush(NULL);
					row--;
					printf("\e[%d;8H\e[7m%s\e[0m",row,arr[row-10]);
					fflush(NULL);
				}
			}
			else if(key[0] == 27 && key[1] == 91 && key[2] == 66) //Down按键
			{
				if(row < 16)
				{
					printf("\e[%d;8H%s", row, arr[row-10]);
					fflush(NULL);
					row++;
					printf("\e[%d;8H\e[7m%s\e[0m", row, arr[row-10]);
					fflush(NULL);
				}
			}
		}
	}

	return;
}


//私聊
int chatfrnd(int row)
{
	int mem = row - 14; //将(row)暂存在(cfd)中打包过来的，行号(row)减去起始行(14)得到第几个好友
	int i;
	Node *p = NULL;
	for(p = frndTb->head.next, i = 0; 
			p != &(frndTb->head), i < mem; 
			p = p->next, i++); //指向所选好友

	if (((ListAll *)(p->data))->stat == OFFLINE)
	{
		printf("\e[s\e[12;83H\e[35m该好友离线，无法及时回复\e[0m");
		fflush(NULL);
		usleep(1000000);
		printf("\e[12;83H\e[35m                        \e[u\e[0m");
		fflush(NULL);
		strcpy(frndid, ((ListAll *)(p->data))->id);
		return 2;
	}
	else if (((ListAll *)(p->data))->stat == ONLINE)
	{
		printf("\e[s\e[12;83H\e[35m该好友在线，您将与TA私聊\e[0m");
		fflush(NULL);
		usleep(1000000);
		printf("\e[12;83H\e[35m                         \e[0m");
		printf("\e[3;10H\e[35m您正在与 %s 私聊, 按ESC退出私聊\e[u\e[0m", ((ListAll *)(p->data))->name);
		fflush(NULL);

		strcpy(frndid, ((ListAll *)(p->data))->id);
		return 0;
	}
	else
	{
		my_err("unpredict error", __LINE__);
		return 3;
	}
}

//群聊
int chatgrup(int row)
{
	int mem = row - 14; //根据 行 判断好友
	int i;
	Node *p = NULL;
	for(p = grupTb->head.next, i = 0; 
			p != &(grupTb->head), i < mem; 
			p = p->next, i++); //指向所选好友

	if (grupTb->num == 0) return -1;

	printf("\e[s\e[12;83H\e[35m您将进入该群聊天室\e[0m");
	fflush(NULL);
	usleep(500000);
	printf("\e[12;83H\e[35m                   \e[0m");
	strcpy(grupid, (char *)(p->data));
	if (strcmp(grupid, "654321") == 0)
		printf("\e[s\e[3;10H\e[35m您正在 %s 聊天室, 按ESC退出群聊\e[u\e[0m", "Y_Y黛丝li");
	if (strcmp(grupid, "765432") == 0)
		printf("\e[s\e[3;10H\e[35m您正在 %s 聊天室, 按ESC退出群聊\e[u\e[0m", "吃货￥无罪");

	return 0;
}

//请求好友列表
void ask_frnd_list(void)
{
	frndTb = create_empty_table(sizeof(ListAll)); //好友链表
	MsgPack msg; //打包请求包
	ListAll frndlist, *p;
	p = (ListAll *)msg.data;
	msg.msgtype = FRNDLIST;
	strcpy(msg.from.id, my_id);

	send(conn_fd, &msg, MSG_SIZE, 0); //请求好友列表	
	recv(conn_fd, &msg, MSG_SIZE, 0); //接收好友列表

	strcpy(my_name, msg.dest.name); //my_name全局变量了
	printf("\e[s\e[7;96H\e[34m%s\e[u\e[0m", my_name); //打印当前用户姓名

	if (msg.msgsize == 0)
		printf("\e[s\e[%d;86H\e[31m  %s  \e[u\e[0m", 14, "您还没有好友！");

	int i, j;
	for (j = 0; j < 2; j++)
	{
		p = (ListAll *)msg.data;
		for(i = 0; i < msg.msgsize; i++ )
		{
			memcpy(&frndlist, p++, sizeof(ListAll));
			//先 在线 好友
			if ((j == 0) && (frndlist.stat == ONLINE)) 
			{
				//用尾部插入方法保存好友信息
				insert_to_tail(&frndlist, frndTb);
			}
			//再 离线 好友
			if ((j == 1) && (frndlist.stat == OFFLINE))
			{
				//用尾部插入方法保存好友信息
				insert_to_tail(&frndlist, frndTb);
			}
		}
	}

	return;
}

//显示好友列表
void frndlist_view(void)
{
	int row = 30;
	while(row >=14) //清屏
	{
		printf("\e[s\e[%d;82H                           \e[u", row--);
	}

	if (frndTb->num == 0)
	{
		printf("\e[s\e[%d;86H\e[31m  %s  \e[u\e[0m", 14, "您还没有好友！");
		fflush(NULL);
		return;
	}
	int j;
	int online_num = 0; //在线 好友人数
	int offline_num = 0; //离线 好友人数
	ListAll frndlist;
	Node *p = NULL;
	for (j = 0; j < 2; j++)
	{
		for( p = frndTb->head.next; p != &(frndTb->head); p = p->next)
		{
			memcpy(&frndlist, p->data, sizeof(ListAll));

			//先打印 在线 好友
			if ((j == 0) && (frndlist.stat == ONLINE)) 
			{
				printf("\e[s\e[%d;86H\e[31m%s\t%s\e[u\e[0m", 14+online_num, frndlist.name, frndlist.id);
				online_num++;
			}
			//再打印 离线 好友
			else if ((j == 1) && (frndlist.stat == OFFLINE))
			{
				printf("\e[s\e[%d;86H%s\t%s\e[u", (14 + offline_num + online_num), frndlist.name, frndlist.id);
				offline_num++;
			}
		}
	}
	printf("\e[s\e[11;99H\e[35m%d\e[u\e[0m", online_num);//在线人数
	fflush(NULL);

	return;
}


//更新在线好友列表
void stat_upd(MsgPack *msg) 
{
	Stat stat_msg;
	memcpy(&stat_msg, msg->data, sizeof(Stat));

	Node *p = NULL;
	for(p = frndTb->head.next; p != &(frndTb->head); p = p->next) //指向所选好友
	{
		if (strcmp(stat_msg.id, ((ListAll *)(p->data))->id) == 0)
		{
			((ListAll *)(p->data))->stat = stat_msg.stat;
			if (stat_msg.stat == ONLINE)
			{
				ListAll tmp;
				memmove(&tmp, p->data, sizeof(ListAll)); //重新排序，在线在头部
				memmove(p->data, frndTb->head.next->data, sizeof(ListAll));
				memmove(frndTb->head.next->data, &tmp, sizeof(ListAll));
			}
			else
			{
				ListAll tmp;
				memmove(&tmp, p->data, sizeof(ListAll)); //离线好友放到链表尾部
				memmove(p->data, frndTb->head.prev->data, sizeof(ListAll));
				memmove(frndTb->head.prev->data, &tmp, sizeof(ListAll));
			}
			break;
		}
	}
	frndlist_view(); // 排序完成后刷新屏幕

	return;
}

//请求群列表
void ask_grup_list(void)
{
	grupTb = create_empty_table(32); //群链表
	MsgPack msg;
	memset(&msg, 0, MSG_SIZE);
	char *pdata = msg.data;
	msg.msgtype = GRUPLIST;
	strcpy(msg.from.id, my_id);

	send(conn_fd, &msg, MSG_SIZE, 0); //请求群列表	 	
	recv(conn_fd, &msg, MSG_SIZE, 0); //接收群列表

	int i;
	for(i = 0; i < msg.msgsize; i++ )
	{	
		char gruplist[32];
		memmove(gruplist, pdata, 32);

		//用尾部插入方法保存好友信息
		insert_to_tail(gruplist, grupTb);	
		pdata += 32;
	}
	return;
}

//显示群列表
void gruplist_view(void)
{
	int row = 30;
	while(row >=14) //清屏
	{
		printf("\e[%d;82H                           ", row--);
	}

	if (grupTb->num == 0)
	{
		printf("\e[%d;86H\e[31m  %s  \e[0m", 14, "您还没有QQ群！");
		fflush(NULL);
		return;
	}

	int i;
	Node *p =NULL;
	char *gruname[2] = {"Y_Y黛丝li", "吃货￥无罪"};
	for( p = grupTb->head.next, i = 0; p != &(grupTb->head); p = p->next, i++)
	{
		char buf[32];
		memcpy(buf, p->data, 32);

		if (strcmp(buf, "654321") == 0)
			printf("\e[%d;86H\e[31m%s\t%s\e[0m", 14+i, gruname[0], buf);
		if (strcmp(buf, "765432") == 0)	   
			printf("\e[%d;86H\e[31m%s\t%s\e[0m", 14+i, gruname[1], buf);
		fflush(NULL);
	}

	return;
}


//找回密码
void findpwd_view(void)
{	
	printf("\e[10;8H\e[7m您的QQ号:\e[s\e[0m");
	printf("\e[11;8H密保问题:");
	printf("\e[12;8H密保答案:");
	printf("\e[13;8H重置密码:");
	printf("\e[14;8H确认密码:");
	printf("\e[15;8H\e[31m提交申请\e[0m");
	printf("\e[16;8H\e[32m放弃申请\e[u\e[0m");
	fflush(NULL);
	return;
}

//找回密码
void *findpwd(void *msgpack)
{
	FindPwd find_pwd;
	memset(&find_pwd, 0, sizeof(FindPwd));
	char passwd_1[32];
	char passwd_2[32];
	MsgPack msg;
	msg.msgtype = FINDPWD;
	char *arr[7] = {"您的QQ号:", "密保问题:", "密保答案:", "重置密码:", "确认密码:", "提交申请", "放弃申请"};
	int row = 10;

	findpwd_view(); //显示界面

	tcgetattr(0, &oldattr);
	cfmakeraw(&newattr);
	while(1)
	{
		char key[3]= {0};
		memset(key, 0, 3);
		tcsetattr(0, TCSANOW, &newattr);
		int ret = read(0, key, 3);
		tcsetattr(0, TCSANOW, &oldattr);

		if(ret == 1)
		{
			if(key[0] == 3) //Ctrl+C按键
			{
				close(conn_fd);
				printf("\n已退出！\n感谢您使用红猪聊天系统，\n欢迎您再次使用！\n");
				exit(0);
			}
			else if(key[0] == 13) //Enter按键
			{	
				printf("\e[%d;8H%s\e[s       \e[u", row, arr[row-10]);
				fflush(NULL);
				switch( row )
				{	
					case 10:
						GETSTR(find_pwd.id, 32);
						break;

					case 11:
						GETSTR(find_pwd.question, 32);
						break;

					case 12:
						GETSTR(find_pwd.answer, 32);
						break;

					case 13:
						GETSTR(passwd_1, 32);
						break;

					case 14:
						GETSTR(passwd_2, 32);
						if (strcmp(passwd_1, passwd_2) != 0)
						{
							printf("\e[s\e[7;8H 两次密码不一致\e[u\e[0m");
							fflush(NULL);
							break;
						}
						strcpy(find_pwd.passwd, passwd_2);
						break;

					case 15:
						memmove(&msg.data, &find_pwd, sizeof(FindPwd));
						send(conn_fd, &msg, MSG_SIZE, 0); //发送重置信息			            
						char recv_buf[32];
						bzero(recv_buf, sizeof(recv_buf)); //接收反馈信息
						recv(conn_fd, recv_buf, sizeof(recv_buf), 0);

						menu_view();
						printf("\e[6;8H%s", recv_buf);
						if (strcmp(recv_buf, " 恭喜您，重置成功") == 0)
						{	          
							printf("\e[9;8H%s", " 请您牢记 新密码");
							printf("\e[11;8H%s", "5秒后返回登录界面");
							fflush(NULL);
							sleep(5);
							return 0;  
						}
						else 
						{
							int row = 10;
							findpwd_view();
						}			            			   			           
						break;

					case 16:
						return;
						break;

					default:
						break;
				}
				printf("\e[%d;8H%s", row, arr[row-10]);
				fflush(NULL);
			}
		}
		else if(ret == 3)
		{
			if(key[0] == 27 && key[1] == 91 && key[2] == 65) //Up按键
			{
				if(row > 10)
				{
					printf("\e[%d;8H%s",row,arr[row-10]);
					fflush(NULL);
					row--;
					printf("\e[%d;8H\e[7m%s\e[0m",row,arr[row-10]);
					fflush(NULL);
				}
			}
			else if(key[0] == 27 && key[1] == 91 && key[2] == 66) //Down按键
			{
				if(row < 16)
				{
					printf("\e[%d;8H%s", row, arr[row-10]);
					fflush(NULL);
					row++;
					printf("\e[%d;8H\e[7m%s\e[0m", row, arr[row-10]);
					fflush(NULL);
				}
			}
		}
	}

	return;
}


//显示连接服务器界面
void conn_view(void)
{
	system("clear");
	printf("     \e[35m\e[43m*****************\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m   ╭︿︿︿╮红猪\e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m  {/ o  o /}   \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m   ( (00) )    \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m    ︶︶︶     \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m               \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m 链接服务器... \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m               \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m               \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m               \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m               \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m               \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m               \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m               \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m               \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m               \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*\e[0m               \e[35m\e[43m*\e[0m\n");
	printf("     \e[35m\e[43m*****************\e[0m\n");
	usleep(400000); //等待400毫秒

	return;
}

//服务端消息处理
void *recv_handler(void *argv)
{
	//监听服务端
	fd_set fds;
	while(1)	
	{
		FD_ZERO(&fds);
		FD_SET(conn_fd, &fds);

		select(conn_fd+1, &fds, NULL, NULL, NULL);

		if(FD_ISSET(conn_fd,&fds)) //服务端有消息
		{	

			// (一)、获取数据包
			int ret;
			MsgPack msg;
			memset(&msg, 0, MSG_SIZE);
			ret = recv(conn_fd, &msg, MSG_SIZE, 0); //数据包异常处理
			if (ret == 0) //服务器关闭
			{
				printf("服务器挂了, 赶快下线。\n");
				while(1);
				close(conn_fd);     // 1. 关闭连接套接字           
				pthread_exit(NULL); // 2. 关闭该用户线程
			}
			else if(ret < 0) //接收失败
			{
				my_err("client recv msg failed", __LINE__);
			}

			//（二）、数据包解析 (数据包结构 MsgPack 在common.h定义）
			//	printf("test msg  type:%d data: %s\n", msg.msgtype, msg.data); //测试打印数据
			switch(msg.msgtype)
			{	
				case CHATFRND:
					recv_frnd_msg(&msg); //私聊
					break;

				case CHATGRUP:
					recv_grup_msg(&msg); //群聊
					break;

				case STAT:
					stat_upd(&msg); //更新在线好友列表
					break;

				case ADDFRND:
					recv_addfrnd(&msg);
					break;

				default:
					break;
			}
		}
	}
}

//中断处理
void sig(int argv)
{
	printf("\n用户强制退出！\n");
	exit(0);
}


//初始化
int init(void)
{
	signal(SIGINT, sig); 

	conn_view(); //显示连接服务器界面

	//创建一个TCP套接字
	conn_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (conn_fd < 0)
	{
		my_err("socket", __LINE__);
	}

	//初始化服务器端地质结构
	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons(SER_PORT);
	seraddr.sin_addr.s_addr = inet_addr(IP);

	//向服务器发送连接请求
	int conn_count;
	for(conn_count = 0; conn_count < 5; conn_count++)
	{
		if (connect(conn_fd, (struct sockaddr *)&seraddr, sizeof(struct sockaddr_in)) < 0)
		{
			perror("连接服务器失败！ 1秒后自动重连。\n");
			sleep(1);
			continue;
		}
		break;
	}
	if (conn_count >= 5)
	{
		printf("网络故障，已放弃治疗！\n");
		exit(1);
	}

	menu(); //连接成功， 现在进入菜单

	return 0;
}

//主函数
int main()
{
	init();

	exit(0);
}
