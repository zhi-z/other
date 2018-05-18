#include "common.h"
#include "server.h"


#define LISTENQ	20 //端口监听上限

/********************************************************************************************					
 *
 *					#####	函数部分开始	#####
 *
 ********************************************************************************************/


/* 遍历打印好友列表 */
void print_all(Table *tb)
{
	Node *p = NULL;

	if (tb == NULL)
	{
		my_err("print_all", __LINE__);
	}

	printf("\t%-12s\n\n",	"好友列表");

	for (p = tb->head.next; p != &(tb->head); p = p->next)
	{
		printf("\t%-12s\n",	((StuUsr *)(p->data))->name);
	}

	return;
}

/* 显示好友资料 */
void print_one(void *data)
{
	StuUsr *stu_frd;
	stu_frd = (StuUsr *)data;

	printf("\t%s\t%s\n", "昵称:", stu_frd->name);
	printf("\t%s\t%s\n", "QQ号:", stu_frd->id);
	printf("\t%s\t%s\n", "性别:", stu_frd->sex);
	printf("\t%s\t%s\n", "年龄:", stu_frd->age);
	printf("\t%s\t%s\n", "城市:", stu_frd->city);

	return;
}


//用户登录处理
int login(MsgPack *msg, int fd)
{
	int flg = 0;

	Login 	lg_info;
	memcpy(&lg_info, msg->data, sizeof(Login));

	MsgPack msg_to_send;
	memset(&msg_to_send, 0, MSG_SIZE);
	msg_to_send.msgtype = LOGIN;

	//遍历比较帐号密码
	Node *p	= NULL;
	for (p = UsrTable->head.next; p != &(UsrTable->head); p = p->next)
	{
		//校验QQ号
		if (strcmp(lg_info.id, ((StuUsr *)(p->data))->id) == 0)
		{	
			flg = 1;
			//校验密码
			if (strcmp(lg_info.passwd, ((StuUsr *)(p->data))->passwd) == 0)
			{	
				if (((StuUsr *)(p->data))->online == ONLINE) //当前用户已经在线
				{
					strcpy(msg_to_send.data, "=_=!  您已在线");
					break;
				}
				flg = 2;
				//密码校验 正确
				strcpy(msg_to_send.data, "Y(^o^)Y 欢迎回来");

				((StuUsr *)(p->data))->newfd = fd; //存储链接套接字
				((StuUsr *)(p->data))->online = ONLINE; //标记在线

				system("date"); //记录登录时间
				printf("Login\tName:%s\tQQ:%s\n\n", ((StuUsr *)(p->data))->name, ((StuUsr *)(p->data))->id); //服务器打印登录信息
			}						
			else
			{	
				flg = 3;
				//密码校验 错误
				strcpy(msg_to_send.data, "T^T  密码错误");
			}

			break;
		}
	}
	//QQ号校验 错误
	if (flg == 0)
	{
		strcpy(msg_to_send.data,"-_-!  无此帐号");
	}

	write(fd, &msg_to_send, MSG_SIZE);//向客户端反馈
	return flg;
}


//用户注销处理
int logout(MsgPack *msg)
{
	//遍历比较帐号密码
	Node *p	= NULL;
	for (p = UsrTable->head.next; p != &(UsrTable->head); p = p->next)
	{
		//校验QQ号
		if (strcmp(msg->from.id, ((StuUsr *)(p->data))->id) == 0)
		{				
			((StuUsr *)(p->data))->online = OFFLINE; //标记在线

			system("date"); //记录注销时间
			printf("Logout\tName:%s\tQQ:%s\n\n", ((StuUsr *)(p->data))->name, ((StuUsr *)(p->data))->id); //服务器打印登录信息	

			break;
		}
	}
	return;
}


//注册
void *regist(MsgPack *msg, int fd)
{
	Regist reg_info;
	memmove(&reg_info, msg->data, sizeof(Regist)); 
	int num;
	char id[32];
	int flag_exit = 0;
	srand((unsigned int)time(NULL));//设置随机数种子   
	do{
		num = rand() % 999999; //产生6位数的QQ号码    
		sprintf(id, "%d", num); //打印成字符串

		Node *p	= NULL;
		//遍历比较帐号
		for (p = UsrTable->head.next; p != &(UsrTable->head); p = p->next)
		{
			//校验QQ号
			if (strcmp(id, ((StuUsr *)(p->data))->id) == 0)
			{   
				flag_exit = 1;	
				break;
			}
		}	    
	}while (flag_exit == 1);

	StuUsr newusr;
	strcpy(newusr.name, reg_info.name);
	strcpy(newusr.passwd, reg_info.passwd);
	strcpy(newusr.id, id);
	strcpy(newusr.sex, "-");
	strcpy(newusr.age, "-");
	strcpy(newusr.city, "-");
	strcpy(newusr.question, reg_info.question);
	strcpy(newusr.answer, reg_info.answer);
	newusr.newfd = 65535;
	newusr.online = OFFLINE;

	Table *frndlist = create_empty_table(32);
	Table *gruplist = create_empty_table(32);

	newusr.frnd = frndlist;
	newusr.grup = gruplist;

	insert_to_tail(&newusr, UsrTable);

	send(fd, &id, sizeof(id), 0);

	return;
}

//私聊
void *chatfrnd(MsgPack *msg)
{
	int i;	
	Node *p = NULL;

	for (p = UsrTable->head.next; p != &(UsrTable->head); p = p->next)
	{
		if (strcmp(msg->dest.id, ((StuUsr *)(p->data))->id) == 0) //找到目的ID
		{
			if (((StuUsr *)(p->data))->online == ONLINE) //该用户在线
			{
				write(((StuUsr *)(p->data))->newfd, msg, MSG_SIZE);
			}
			return;
		}
	}

	return;
}

//群聊
void *chatgrup(MsgPack *msg)
{
	int i;	
	Node *p = NULL;
	Node *q = NULL;


	for (p = UsrTable->head.next; p != &(UsrTable->head); p = p->next)
	{
		//不发给自己
		if (strcmp( ((StuUsr *)(p->data))->name, msg->from.name) == 0)
		{
			continue;
		}
		for (q = ((StuUsr *)(p->data))->grup->head.next;
				q != &(((StuUsr *)(p->data))->grup->head);
				q = q->next) 							
		{  
			if (strcmp(msg->dest.id, (char *)(q->data)) == 0)
			{		
				if (((StuUsr *)(p->data))->online == ONLINE) //该用户在线
				{					    	
					write(((StuUsr *)(p->data))->newfd, msg, MSG_SIZE);
				}
			}
		}
	}

	return ;
}

//加好友
int addfrnd(MsgPack *msg, int fd) 
{
	if(strcmp(msg->data, "同意") == 0)
	{
		Node *p = NULL; //用户列表指针
		//遍历所有用户帐号
		for (p = UsrTable->head.next; p != &(UsrTable->head); p = p->next)
		{   
			//找到该用户信息节点
			if (strcmp(msg->dest.id, ((StuUsr *)(p->data))->id) == 0) //主动加好友的人
			{			      
				insert_to_tail( msg->from.id, (Table *)(((StuUsr *)(p->data))->frnd) ); //加入好友列表       
				send(((StuUsr *)(p->data))->newfd, msg, MSG_SIZE, 0);          	    
			}
			else if(strcmp(msg->from.id, ((StuUsr *)(p->data))->id) == 0) //被加好友的人
			{		       
				insert_to_tail( msg->dest.id, (Table *)(((StuUsr *)(p->data))->frnd) ); //加入好友列表          
			}
		}    
	}
	else 
	{   
		Node *p = NULL; //用户列表指针
		//遍历所有用户帐号
		for (p = UsrTable->head.next; p != &(UsrTable->head); p = p->next)
		{   
			//找到该用户信息节点
			if (strcmp(msg->dest.id, ((StuUsr *)(p->data))->id) == 0)  
			{	       
				send(((StuUsr *)(p->data))->newfd, msg, MSG_SIZE, 0);
				return 0;		     	    
			}
		}
	}
	return;	
}

//好友列表
void *frndlist(MsgPack *msg, int fd)
{

	int i = 0;	
	Node *p = NULL; //用户列表指针
	Node *q = NULL; //好友列表指针
	char frnd_id[32][32]; //保存好友QQ号
	ListAll frndlist, *frndp;
	frndp = (ListAll *)(msg->data);

	//遍历所有用户帐号
	for (p = UsrTable->head.next; p != &(UsrTable->head); p = p->next)
	{   
		//找到该用户信息节点
		if (strcmp(msg->from.id, ((StuUsr *)(p->data))->id) == 0)  
		{
			strcpy(msg->dest.name, ((StuUsr *)(p->data))->name);
			//从好友列表中获取好友QQ号
			for (q = ((Table *)(((StuUsr *)(p->data))->frnd))->head.next, i = 0;  
					(q != &(((Table *)(((StuUsr *)(p->data))->frnd))->head)) && (i < 32);
					q = q->next, i++)
			{
				strcpy(frnd_id[i], (char *)(q->data)); //将好友QQ号保存   
			}
		}
	}   

	msg->msgsize = i;
	for (i = 0; i < msg->msgsize; i++) //说明有好友
	{
		//遍历所有用户帐号
		for (p = UsrTable->head.next; p != &(UsrTable->head); p = p->next)
		{
			//找到该好友信息节点
			if (strcmp(frnd_id[i], ((StuUsr *)(p->data))->id) == 0) 
			{
				//若果好友在线
				if (((StuUsr *)(p->data))->online == ONLINE)
				{
					frndlist.stat = ONLINE;
				}	
				else
				{
					frndlist.stat = OFFLINE;
				}
				strcpy(frndlist.id, ((StuUsr *)(p->data))->id);
				strcpy(frndlist.name, ((StuUsr *)(p->data))->name);
				frndlist.cfd = ((StuUsr *)(p->data))->newfd;
				memcpy(frndp, &frndlist, sizeof(ListAll));
				frndp++;
			}
		}
	}

	write(fd, msg, MSG_SIZE);

	return;
}

//群列表
void *gruplist(MsgPack *msg, int fd)
{
	int i = 0;		
	Node *p = NULL; //用户列表指针
	Node *q = NULL; //q群列表指针
	char grup_id[32][32]; //保存群号

	//遍历所有用户帐号
	for (p = UsrTable->head.next; p != &(UsrTable->head); p = p->next)
	{   
		//找到该用户信息节点
		if (strcmp(msg->from.id, ((StuUsr *)(p->data))->id) == 0)  
		{
			//从好友列表中获取好友QQ号
			for (q = ((Table *)(((StuUsr *)(p->data))->grup))->head.next, i = 0;  
					(q != &(((Table *)(((StuUsr *)(p->data))->grup))->head)) && (i < 32);
					q = q->next, i++)
			{
				strcpy(grup_id[i], (char *)(q->data)); //将好友QQ号保存   
			}
		}
	}   
	char *pdata = msg->data;
	msg->msgsize = i;
	for (i = 0; i < msg->msgsize; i++) //说明有群
	{
		memcpy(pdata, grup_id[i], 32);
		pdata += 32;
	}
	write(fd, msg, MSG_SIZE);

	return;
}



//离线处理自身
int  offline(int conn_fd, char *get_id)
{
	Node *p = NULL;

	//遍历比较帐号密码
	for (p = UsrTable->head.next; p != &(UsrTable->head); p = p->next)
	{
		if ((conn_fd - ((StuUsr *)(p->data))->newfd) == 0)
		{	
			strcpy(get_id, ((StuUsr *)(p->data))->id); //获取该用户id
			((StuUsr *)(p->data))->newfd = 65535; //设置未使用套接字
			((StuUsr *)(p->data))->online = OFFLINE; //标记离线

			system("date"); //记录登录时间
			printf("Offline\tName:%s\tQQ:%s\n\n", ((StuUsr *)(p->data))->name, ((StuUsr *)(p->data))->id); //服务器打印离线信息

			return 0; //找到用户并成功处理离线
		}
	}

	return -1; //未找到用户
}

// 上线、下线 通知好友
int  off_on_noti(char *stat_id, LgFlag flg)
{
	int i = 0;
	Node *p = NULL; //用户列表指针
	Node *q = NULL; //好友列表指针
	char frnd_id[32][32]; //保存好友QQ号
	MsgPack msg;
	msg.msgtype = STAT;
	Stat stat_msg;
	strcpy(stat_msg.id, stat_id); //封装离线QQ号码
	stat_msg.stat = flg; //封装状态
	memcpy(msg.data, &stat_msg, sizeof(Stat));

	//遍历所有用户帐号
	for (p = UsrTable->head.next; p != &(UsrTable->head); p = p->next)
	{
		//找到该用户信息节点
		if (strcmp(stat_id, ((StuUsr *)(p->data))->id) == 0) 
		{
			//从好友列表中获取好友QQ号
			for (q = ((StuUsr *)(p->data))->frnd->head.next, i = 0; 
					(q != &(((StuUsr *)(p->data))->frnd->head)) && (i < 32);
					q = q->next, i++)
			{
				strcpy(frnd_id[i], q->data); //将好友QQ号保存	
			}
		}
	}

	msg.msgsize = i;
	for (i = 0; i < msg.msgsize; i++) //说明有好友
	{
		//遍历所有用户帐号
		for (p = UsrTable->head.next; p != &(UsrTable->head); p = p->next)
		{
			//找到该好友信息节点
			if (strcmp(frnd_id[i], ((StuUsr *)(p->data))->id) == 0) //注意 i--
			{
				//若果好友在线
				if (((StuUsr *)(p->data))->online == ONLINE)
				{
					int frnd_fd = ((StuUsr *)(p->data))->newfd;
					write(frnd_fd, &msg, MSG_SIZE); //向该好友发送离线通知包
				}
			}
		}
	}

	return 0; //异常处理
}

//找回密码
int findpwd(MsgPack *msg, int fd)
{
	Node *p = NULL; //用户列表指针
	FindPwd *p_find = (FindPwd*)(msg->data);

	//遍历所有用户帐号
	for (p = UsrTable->head.next; p != &(UsrTable->head); p = p->next)
	{   
		//找到该用户信息节点
		if (strcmp(p_find->id, ((StuUsr *)(p->data))->id) == 0)  
		{
			//对比问题
			if (strcmp(p_find->question, ((StuUsr *)(p->data))->question) == 0)  
			{
				//对比答案
				if (strcmp(p_find->answer, ((StuUsr *)(p->data))->answer) == 0)  
				{
					strcpy(((StuUsr *)(p->data))->passwd, p_find->passwd); //更改密码
					send(fd, "恭喜您，重置成功", sizeof("恭喜您，重置成功"), 0);
					return 0;
				}
			}
		}
	}  

	send(fd, "很遗憾，重置失败", sizeof("很遗憾，重置失败"), 0);

	return -1; 
}



//新连接客户端 处理子线程
void *new_cli(void *args)
{
	int ret;
	int flg_login_succ = 0;
	int conn_fd = *((int *)args);
	MsgPack msg; 
	while(1)
	{
		// (一)、获取数据包
		memset(&msg, 0, MSG_SIZE);
		ret = recv(conn_fd, &msg, MSG_SIZE, 0); //数据包异常处理
		if (ret == 0) //客户端关闭
		{
			printf("客户端 %d 挂了\n", conn_fd);
			close(conn_fd); // 1. 关闭连接套接字
			if (flg_login_succ == 1)
			{
				char get_id[32];
				int ret_o = offline(conn_fd, get_id); // 2. 设置用户为离线状态
				if (ret_o == 0)  //如果用户存在，并成功处理其离线
				{					
					off_on_noti(get_id, OFFLINE); // 3. 通知其好友该客户已离线
				}
			}
			// 4. 关闭该用户线程
			pthread_exit(NULL);
		}
		else if (ret < 0) //接收失败
		{
			my_err("server read msg failed", __LINE__);											
			pthread_exit((void *)0);
		}

		//（二）、数据包解析 (数据包结构 MsgPack 在common.h定义）
		printf("test msg   type: %d data: %s\n", msg.msgtype, msg.data); //测试打印数据包

		msg.from.cfd = conn_fd;		
		//解析数据包的请求类型
		switch (msg.msgtype)
		{
			case LOGIN:     //登录
				printf("lg:%d\n",LOGIN);
				if (login(&msg, conn_fd) == 2) 
				{
					flg_login_succ = 1;
					off_on_noti(msg.from.id, ONLINE);
				}
				break;

			case LOGOUT:   //注销
				printf("lout:%d\n",LOGOUT);
				logout(&msg); // 处理自己离线 *不关闭conn_fd* 
				off_on_noti(msg.from.id, OFFLINE); //通知好友
				break;

			case REGIST:    //注册
				printf("rg:%d\n",REGIST);
				regist(&msg, conn_fd);
				break;

			case CHATFRND:  //私聊
				printf("cf:%d\n",CHATFRND);
				chatfrnd(&msg); 
				break;

			case CHATGRUP:  //群聊
				printf("cg:%d\n",CHATGRUP);
				chatgrup(&msg); 
				break;

			case FRNDLIST:  //获取所有好友列表
				printf("fl:%d\n",FRNDLIST);
				frndlist(&msg, conn_fd); 
				break;

			case GRUPLIST:  //获取群列表
				printf("gl:%d\n",GRUPLIST);
				gruplist(&msg, conn_fd); 
				break;

			case ADDFRND:    //加好友
				printf("add:%d\n",ONLINE);
				addfrnd(&msg, conn_fd); 
				break;

			case FINDPWD:   //找回密码
				printf("fp:%d\n",FINDPWD);
				findpwd(&msg, conn_fd); 
				break;

			case CLOSE:   
				printf("close:%d\n",CLOSE);
				close(conn_fd);
				logout(&msg); //用户退出
				off_on_noti(msg.from.id, OFFLINE);
				printf("%s 已退出登录\n", msg.from.name);
				pthread_exit((void *)0);				
				break;

			default:
				printf("default\n");
				break;
		}			
	}
}


//启动服务器
int init(void)
{
	int		sock_fd, conn_fd;
	int		optval;
	int 		ret;
	pid_t		pid;
	socklen_t	cli_len;
	struct sockaddr_in cli_addr, serv_addr;

	//创建一个TCP套接字
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd < 0)
	{
		my_err("Server创建套接字失败", __LINE__);
	}

	//设置该套接字可以重新绑定端口（多路复用）
	optval = 1;
	if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&optval, sizeof(int)) < 0)
	{
		my_err("Server设置端口复用失败", __LINE__);
	}

	//初始化服务器端地址结构
	memset(&serv_addr, 0, sizeof(struct sockaddr_in));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SER_PORT);
	serv_addr.sin_addr.s_addr = inet_addr(IP);

	//将套接字绑定到本地端口
	if (bind(sock_fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in)) < 0)
	{
		my_err("Server套接字绑定端口失败", __LINE__);
	}

	//将套接字转化为监听套接字
	if (listen(sock_fd, LISTENQ) < 0)
	{
		my_err("Server套接字转为监听失败", __LINE__);
	}

	cli_len = sizeof(struct sockaddr_in);
	while (1)
	{
		//通过accept接收客户端的链接状态请求，返回链接套接字用于收发数据。
		conn_fd = accept(sock_fd, (struct sockaddr *)&cli_addr, &cli_len);
		printf("conn_fd:%d\n", conn_fd);
		if (conn_fd < 0)
		{
			my_err("Server无链接请求", __LINE__);
		}

		printf("accept a new client: %d   IP:%s",conn_fd, inet_ntoa(cli_addr.sin_addr));
		system("date");
		printf("\n");

		// 为 新连接客户端 创建线程 
		pthread_t new_tid;
		pthread_create(&new_tid, NULL, new_cli, (void *)&conn_fd); 
		pthread_detach(new_tid);	   
	}

	return 0;
}

//载入用户信息
int dload_list(void)
{
	UsrTable = create_empty_table(sizeof(StuUsr));
	Table *FrnTb1 = create_empty_table(32);
	Table *GruTb1 = create_empty_table(32);
	Table *FrnTb2 = create_empty_table(32);
	Table *GruTb2 = create_empty_table(32);
	Table *FrnTb3 = create_empty_table(32);
	Table *GruTb3 = create_empty_table(32);
	Table *FrnTb4 = create_empty_table(32);
	Table *GruTb4 = create_empty_table(32);

	char *f1[3] = {"777777", "999999", "666666"}; //88
	char *f2[2] = {"888888", "999999"}; //77
	char *f3[2] = {"888888", "777777"}; //99
	char *f4[1] = {"888888"}; //66
	char *g1[2] = {"654321", "765432"};
	char *g2[2] = {"654321", "765432"};
	char *g3[2] = {"654321", "765432"};
	char *g4[1] = {"765432"};

	insert_to_tail(f1[0], FrnTb1);
	insert_to_tail(f1[1], FrnTb1);
	insert_to_tail(f1[2], FrnTb1);
	insert_to_tail(g1[0], GruTb1);
	insert_to_tail(g1[1], GruTb1);
	insert_to_tail(f2[0], FrnTb2);
	insert_to_tail(f2[1], FrnTb2);
	insert_to_tail(g2[0], GruTb2);
	insert_to_tail(g2[1], GruTb2);
	insert_to_tail(f3[0], FrnTb3);
	insert_to_tail(f3[1], FrnTb3);
	insert_to_tail(g3[0], GruTb3);
	insert_to_tail(g3[1], GruTb3);
	insert_to_tail(f4[0], FrnTb4);
	insert_to_tail(g4[0], GruTb4);

	StuUsr stu_usr[4] = {
		{"沉鱼落雁", "123456", "666666", "- ", "13", "湖南", "你好吗", "我不好", FrnTb4,  GruTb4, 65535, OFFLINE},
		{"倾城倾国", "123456", "777777", "女", "12", "南宁", "你好吗", "我不好", FrnTb2,  GruTb2, 65535, OFFLINE},
		{"如花似玉", "123456", "888888", "女", "20", "上海", "你好吗", "我不好", FrnTb1,  GruTb1, 65535, OFFLINE},
		{"闭月羞花", "123456", "999999", "男", "51", "武汉", "你好吗", "我不好", FrnTb3,  GruTb3, 65535, OFFLINE}
	};

	insert_to_tail(&stu_usr[0], UsrTable);
	insert_to_tail(&stu_usr[1], UsrTable);
	insert_to_tail(&stu_usr[2], UsrTable);
	insert_to_tail(&stu_usr[3], UsrTable);

	print_all(UsrTable);

	return;
}

//守护进程
void deamon(void)
{ 
	unsigned int i;
	struct rlimit rli;

	umask(0000);

	if (fork() == 0)
	{
		while(getppid() != 1)
		{
		}
	}
	else
	{
		exit(0);    
	}

	setsid();
	chdir("/");

	getrlimit(RLIMIT_NOFILE, &rli);

	for (i = 0; i < rli.rlim_max; i++)
	{
		close(i);
	}

	open("/dev/null", O_RDONLY);
	dup(0);
	dup(0);   
}


//主函数
int main(int argc, char *argv[])
{

	//载入用户信息
	if ( dload_list() < 0)
	{
		my_err("dload_list", __LINE__);
	}

	//开启守护进程
	// deamon();

	//启动服务器
	if ( init() < 0)
	{
		my_err("init", __LINE__);
	}

	return 0;
}




