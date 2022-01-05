#include<cstdio>
#include<iostream>
#include<vector>
#include<cstring>
using namespace std;
struct PROCESSLIST{//进程链表 
	int id;
	PROCESSLIST* next;
};
struct RCB{//资源数据结构 
	int id;//资源rid 
	vector<char>name; //资源名称 
	bool status;//状态，0空闲1占用 
	PROCESSLIST *head,*tail;//被本资源阻塞的进程链表，head指向链表头tail指向链表尾 
	RCB(int _=0,bool __=0)//初始化 
	{
		id=_,status=__;
		head=new PROCESSLIST;head->next=NULL;
		tail=head;
	}
}source[1100];
struct IO{//io数据结构 
	int id;//io的id 
	vector<char>name;//io名称 
	bool status;//状态，0空闲1占用 
	PROCESSLIST *head,*tail;//被本io阻塞的进程链表，head指向链表头tail指向链表尾 
	IO(int _=0,bool __=0)//初始化 
	{
		id=_,status=__;
		head=new PROCESSLIST;head->next=NULL;
		tail=head;
	}
}io[1100];
struct SOURCELIST{//资源链表 
	int id; 
	SOURCELIST* next;
};
struct IOLIST{//IO链表 
	int id;
	IOLIST* next;
};
struct PCB{//进程数据结构 
	int id;//进程id 
	vector<char>name; //进程名 
	SOURCELIST *head;//进程占用资源列表 
	IOLIST *headio;//进程占用IO列表 
	int status;//状态码 0就绪1运行2阻塞3摧毁 
	pair<int,int> wait;//进程状态为2时有效，代表进程因什么被阻塞，first表示id，second表示是资源还是io 
	PROCESSLIST *pos;//进程在状态队列的位置 
	int fa;//父进程id 
	vector<int>son;//子进程id 
	int pri;//进程优先级 
	PCB(int _=0,SOURCELIST *__=NULL,IOLIST *___=NULL,int ____=-1,PROCESSLIST *_____=NULL,int ______=-1,int _______=-1)
	{//初始化 
		id=_;
		head=__;
		headio=___;
		status=____;
		pos=_____;
		fa=______;
		pri=_______;
		head=new SOURCELIST;
		head->next=NULL;
		headio=new IOLIST;
		headio->next=NULL;
	};
}process[1100];
struct QUEUE{//优先级队列 
	PROCESSLIST *head,*tail;
}q[110];
int n;
char op[20];
char name[20];
int rt=1,tr[3][2010][62],sz[3],p,tag[3][2010],tot[3];//字典树相关数据结构 
int nowProcess=0;//当前运行的进程 
int tf(char c)//字符转换，压缩字典树空间 
{
	if (c>='A'&&c<='Z') return c-'A';
	else if (c>='a'&&c<='z') return c-'a'+26;
	else if (c>='0'&&c<='9') return c-'0'+52;
	return -1; 
}
int insert(int p,char *s,bool ins=0,bool find=0,bool del=0)
//ins/find/del为1分别表示向字典树中插入/查找/删除字符串，p为0表示进程字典树，1资源字典树，2io字典树 
{
	int nw=rt;
	int n=strlen(s);
	for (int i=0;i<n;i++)
	{
		int nxt=tf(s[i]);
		if (!tr[p][nw][nxt])
			tr[p][nw][nxt]=++sz[p];
		nw=tr[p][nw][nxt]; //按照字符在字典树上移动 
	}
	if (del)//删除 
	{
		if (!tag[p][nw]) return -1;//该位置没有标记，返回错误 
		int id=tag[p][nw];
		tag[p][nw]=0;//标记置0 
		return id;
	}
	if (find)//查找 
	{
		if (!tag[p][nw]) return -1;//该位置没标记，返回错误 
		return tag[p][nw];
	}
	if (ins)//插入 
	{
		if (!tag[p][nw]) return tag[p][nw]=++tot[p];
		return -1;//该位置有标记，返回错误 
	}
}
PROCESSLIST* insertQueue(int pri,int id)//添加进程id到优先级pri的队列，返回插入后id的指针 
{
	PROCESSLIST *nw=new PROCESSLIST;
	nw->next=NULL,nw->id=id;
	q[pri].tail->next=nw;
	q[pri].tail=nw;
	return nw;
}
void deleteQueue(int pri)//从就绪队列pri中删除队头进程 
{
	if (q[pri].head->next==q[pri].tail)//如果删除后队列变为空，进行特殊处理 
	{
		q[pri].head->next=NULL;
		delete q[pri].tail;
		q[pri].tail=q[pri].head; 
		return;
	}
	PROCESSLIST *nxt=q[pri].head->next;
	q[pri].head->next=q[pri].head->next->next;
	delete nxt;
}
void schedule()//调度算法 
{
	int maxid=-1;
	for (int i=n;i>=0;i--)//找到就绪队列中优先级最高的进程 
		if (q[i].head->next!=NULL)
		{
			maxid=q[i].head->next->id;
			break;
		}
	if (maxid==-1) return;//就绪队列没有进程，不需要调度，直接返回 
	if (nowProcess==-1 || process[nowProcess].status!=1)
	//当前没有运行的进程或者当前进程不在运行 
	{
		nowProcess=maxid;//运行就绪队列中优先级最高的进程
		process[nowProcess].status=1;
		process[nowProcess].pos=NULL;
		deleteQueue(process[nowProcess].pri);//从就绪队列中删除该进程 
		return;
	}
	if (process[nowProcess].status==1 && process[maxid].pri>process[nowProcess].pri)
	//当前运行的进程在正常运行且就绪队列中优先级最高的进程大于当前进程 
	{
		//当前进程加入对应就绪队列 
		process[nowProcess].pos=insertQueue(process[nowProcess].pri,nowProcess);
		process[nowProcess].status=0;
		nowProcess=maxid;//运行就绪队列中优先级最高的进程
		process[nowProcess].status=1;
		process[nowProcess].pos=NULL;
		deleteQueue(process[nowProcess].pri);//从就绪队列中删除该进程 
	}
}
void create(char *s,int pri)//创建一个新进程 
{
	int id=insert(0,s,1);//新进程名加入字典树，返回进程id 
	if (id==-1) {printf("ERROR:Process name has been used.\n");return;}
	//如果进程名已存在 返回错误 
	process[nowProcess].son.push_back(id);//添加当前进程子进程 
	process[id]=PCB{id,NULL,NULL,0,NULL,nowProcess,pri};//创建新进程 
	process[id].pos=insertQueue(pri,id);//在对应优先级的就绪队列添加进程，返回进程的位置 
	int len=strlen(s);
	for (int i=0;i<len;i++) process[id].name.push_back(s[i]);//存储进程名 
	schedule();//调度 
}
bool deleteSoucre(int nowProcess,int id)//在进程nowProcess占用资源链表中删除资源id
{
	SOURCELIST *nw=process[nowProcess].head;
	while (nw->next!=NULL)//遍历整个链表 
	{
		if (nw->next->id==id)//找到资源，执行删除操作，返回成功 
		{
			SOURCELIST *p=nw->next;
			nw->next=nw->next->next;
			delete p;
			return 1;
		}
		nw=nw->next;
	}
	//未找到资源，返回失败 
	return 0;
}
int deleteProcess(int id)//取出被本资源阻塞的进程队列中的队头进程 
{
	int nowId=source[id].head->next->id;
	if (source[id].head->next==source[id].tail)//删除进程后队列边为空，特殊处理 
	{
		source[id].head->next=NULL;
		delete source[id].tail;
		source[id].tail=source[id].head; 
		return nowId;
	}
	//正常删除并返回 
	PROCESSLIST *nxt=source[id].head->next;
	source[id].head->next=source[id].head->next->next;
	delete nxt;
	return nowId;
}
void insertSource(int nowProcess,int id)//在进程nowProcess占用资源链表中加入资源id 
{
	SOURCELIST *nw=new SOURCELIST;
	nw->id=id; 
	nw->next=process[nowProcess].head->next;
	process[nowProcess].head->next=nw;
}
void insertProcess(int id,int nowProcess)//在被资源id阻塞的进程队列中加入进程nowProcess 
{
	PROCESSLIST *nw=new PROCESSLIST;
	nw->id=nowProcess;
	nw->next=NULL;
	source[id].tail->next=nw;
	source[id].tail=nw;
}
void request(char *s)//请求资源 
{
	int id=insert(1,s,0,1,0);//字典树中查找资源，返回id 
	if (id==-1)//没有该资源 
	{
		int len=strlen(s);
		id=insert(1,s,1,0,0);//字典树中插入资源，返回id 
		source[id]=RCB{id,0};//初始化新资源 
		for (int i=0;i<len;i++)//记录资源名 
			source[id].name.push_back(s[i]);
	}
	if (source[id].status==0)//当前资源为空闲资源 
	{
		source[id].status=1;//占用当前资源 
		insertSource(nowProcess,id);//在当前进程占用资源链表中加入该资源 
	}
	else
	{
		process[nowProcess].status=2;//当前进程阻塞 
		process[nowProcess].wait=make_pair(id,1);//设置进程的阻塞原因，因资源id而阻塞 
		insertProcess(id,nowProcess);//在被本资源阻塞的进程队列中加入该进程 
		schedule();//调度 
	}
}

void release(char *s)//释放资源 
{
	int id=insert(1,s,0,1,0);//字典序中查找资源，返回id 
	if (id==-1){printf("ERROR:Source name does not exist.\n");return;}//未找到资源名，报错 
	if (!deleteSoucre(nowProcess,id)){printf("ERROR:Process does not use this source.\n");return;}
	//在当前进程占用资源链表中删除该资源，若未找到该资源，报错 
	if (source[id].head==source[id].tail)//如果被本资源阻塞的进程队列为空，则设置资源状态为空闲 
		source[id].status=0;
	else
	{
		//否则取出队头进程 
		int idx=deleteProcess(id);
		process[idx].status=0;//设置进程就绪 
		process[idx].pos=insertQueue(process[idx].pri,idx);//进程加入就绪队列 
		process[idx].wait=make_pair(0,0);//清空wait 
		insertSource(idx,id);//在当前进程占用资源链表中加入该资源 
		schedule();//调度 
	}
}
//------------下面是io操作，和资源操作相同--------------// 
void insertIO(int nowProcess,int id)
{
	IOLIST *nw=new IOLIST;
	nw->id=id; 
	nw->next=process[nowProcess].headio->next;
	process[nowProcess].headio->next=nw;
}
void insertIOProcess(int id,int nowProcess)
{
	PROCESSLIST *nw=new PROCESSLIST;
	nw->id=nowProcess;
	nw->next=NULL;
	io[id].tail->next=nw;
	io[id].tail=nw;
}
bool deleteIO(int nowProcess,int id)
{
	IOLIST *nw=process[nowProcess].headio;
	while (nw->next!=NULL)
	{
		if (nw->next->id==id)
		{
			IOLIST *p=nw->next;
			nw->next=nw->next->next;
			delete p;
			return 1;
		}
		nw=nw->next;
	}
	return 0;
}
int deleteIOProcess(int id)
{
	int nowId=io[id].head->next->id;
	if (io[id].head->next==io[id].tail)
	{
		io[id].head->next=NULL;
		delete io[id].tail;
		io[id].tail=io[id].head; 
		return nowId;
	}
	PROCESSLIST *nxt=io[id].head->next;
	io[id].head->next=io[id].head->next->next;
	delete nxt;
	return nowId;
}
void iorequest(char *s)
{
	int id=insert(2,s,0,1,0);
	if (id==-1)
	{
		int len=strlen(s);
		id=insert(2,s,1,0,0);
		io[id]=IO{id,0};
		for (int i=0;i<len;i++)
			io[id].name.push_back(s[i]);
	}
	if (io[id].status==0)
	{
		io[id].status=1;
		insertIO(nowProcess,id);
	}
	else
	{
		process[nowProcess].status=2;
		process[nowProcess].wait=make_pair(id,2);
		insertIOProcess(id,nowProcess);
		schedule();
	}
}

void iorelease(char *s)
{
	int id=insert(2,s,0,1,0);
	if (id==-1){printf("ERROR:IO name does not exist.\n");return;}
	if (!deleteIO(nowProcess,id)){printf("ERROR:Process does not use this IO.\n");return;}
	if (io[id].head==io[id].tail)
		io[id].status=0;
	else
	{
		int idx=deleteIOProcess(id);
		process[idx].status=0;
		process[idx].pos=insertQueue(process[idx].pri,idx);
		process[idx].wait=make_pair(0,0);
		insertIO(idx,id);
		schedule();
	}
}
//------------上面是io操作，和资源操作相同--------------// 
void timeout()//时间片时间到，强制调用timeout 
{
	for (int i=n;i>=0;i--)
	{
		if (q[i].head->next!=NULL)//就绪队列中寻找优先级最高的进程 
		{
			int pre=nowProcess;
			nowProcess=q[i].head->next->id;
			deleteQueue(process[nowProcess].pri);//新进程从就绪队列中删除 
			process[nowProcess].status=1;//更改状态 
			process[nowProcess].pos=NULL;
			process[pre].pos=insertQueue(process[pre].pri,pre);//当前进程加入就绪队列 
			process[pre].status=0;//更改状态 
			return;
		}
	}
}
void destory()//删除进程 
{
	SOURCELIST *nw=process[nowProcess].head;
	while (nw->next!=NULL)//取消占用所有的资源 
	{
		SOURCELIST *pre=nw;
		int id=nw->next->id;
		if (source[id].head==source[id].tail)
			source[id].status=0;
		else
		{
			int idx=deleteProcess(id);
			process[idx].status=0;//设置进程就绪 
			process[idx].pos=insertQueue(process[idx].pri,idx);//进程加入就绪队列 
			process[idx].wait=make_pair(0,0);//清空wait 
			insertSource(idx,id);//在当前进程占用资源链表中加入该资源 
		}
		nw=nw->next;
		delete pre;//释放空间 
	}
	delete nw;
	process[nowProcess].head=NULL; 
	IOLIST *nw2=process[nowProcess].headio;
	while (nw2->next!=NULL)//取消占用所有的IO 
	{
		IOLIST *pre=nw2;
		int id=nw2->next->id;
		if (io[id].head==io[id].tail)
			io[id].status=0;
		else
		{
			int idx=deleteIOProcess(id);
			process[idx].status=0;
			process[idx].pos=insertQueue(process[idx].pri,idx);
			process[idx].wait=make_pair(0,0);
			insertIO(idx,id);
		}
		nw2=nw2->next;
		delete pre;
	}
	delete nw2;
	process[nowProcess].headio=NULL;
	int nlen=0;
	for (auto x:process[nowProcess].name)
		name[nlen++]=x;
	name[nlen]=0;
	insert(0,name,0,0,1); //从字典树中删除该进程名 
	process[nowProcess].status=3;//设置进程状态为删除 
	nowProcess=-1;//当前未运行进程 
	schedule();//调度 
}
void show()//展示当前所有进程的状态 
{
	for (int i=1;i<=tot[0];i++)
	{
		printf("Process ");
		for (auto x:process[i].name) printf("%c",x);
		printf(" is ");
		if (process[i].status==0) printf("waiting\n");
		else if (process[i].status==1) printf("running\n");
		else if (process[i].status==2) printf("blocked\n");
		//else if (process[i].status==3) printf("destoried\n");	
	}
	printf("-----------------------\n");
}
void showPriList()//展示当前所有的就绪队列 
{
	printf("------PRIORITY LIST-------\n");
	for (int i=n;i>=0;i--)
	{
		printf("Priority %d : ",i);
		PROCESSLIST *nw=q[i].head;
		while (nw->next!=NULL)
		{
			printf("%d ",nw->next->id);
			nw=nw->next;
		}
		printf("\n");
	}
}
void showProcess()//展示当前所有的进程信息 
{
	printf("------PROCESS MESSAGE-------\n");
	for (int i=1;i<=tot[0];i++)
	{
		if (process[i].status==3) continue;
		printf("Process ");
		for (auto x:process[i].name) printf("%c",x);
		printf("\n");
		printf("id : %d\n",process[i].id);
		printf("fa : %d\n",process[i].fa);
		if (process[i].status==2)
			printf("pos : %s %d\n",process[i].wait.second?"IO":"Source",process[i].wait.first);
		printf("son : ");for (auto x:process[i].son) printf("%d ",x);printf("\n");
		SOURCELIST *nw=process[i].head;
		printf("SOURCE: ");
		while (nw->next!=NULL)
		{
			printf("%d ",nw->next->id);
			nw=nw->next;
		}
		printf("\n");
		IOLIST *nw2=process[i].headio;
		printf("IO: ");
		while (nw2->next!=NULL)
		{
			printf("%d ",nw2->next->id);
			nw2=nw2->next;
		}
		printf("\n");
	}
}
void showSource()//展示当前所有的资源信息 
{
	printf("------SOURCE MESSAGE-------\n");
	for (int i=1;i<=tot[1];i++)
	{
		printf("id : %d\n",source[i].id);
		printf("name ");for (auto x:source[i].name) printf("%c",x);printf("\n");
		printf("wait list : ");
		PROCESSLIST *nw=source[i].head;
		while (nw->next!=NULL)
		{
			printf("%d ",nw->next->id);
			nw=nw->next;
		}
		printf("\n");
	}
}
void showIO()//展示当前所有的IO信息 
{
	printf("------IO MESSAGE-------\n");
	for (int i=1;i<=tot[2];i++)
	{
		printf("id : %d\n",io[i].id);
		printf("name ");for (auto x:io[i].name) printf("%c",x);printf("\n");
		printf("wait list : ");
		PROCESSLIST *nw=io[i].head;
		while (nw->next!=NULL)
		{
			printf("%d ",nw->next->id);
			nw=nw->next;
		}
		printf("\n");
	}
}
int main()
{
	sz[0]=sz[1]=sz[2]=1;
	process[0]=PCB{0,NULL,NULL,1,NULL,-1,0};//初始化init进程 
	process[0].name.push_back('i');process[0].name.push_back('n');process[0].name.push_back('i');process[0].name.push_back('t');
	printf("Max Number of Priority:");//设置最高优先级 
	scanf("%d\n",&n);
	for (int i=0;i<=n;i++) q[i].head=new PROCESSLIST,q[i].head->next=NULL,q[i].tail=q[i].head;
	while (1)
	{
		int pri;
		scanf("%s",op);
		if (op[0]=='c'&&op[1]=='r')//create操作 创建进程 
		{
			scanf("%s",name);
			scanf("%d",&pri);
			if (pri>n||pri<1)
			{
				printf("ERROR:Wrong priority.\n");
				continue;
			}
			create(name,pri);
		}
		else if (op[0]=='d'&&op[1]=='e')//destory操作 摧毁当前进程 
		{
			destory();
		}
		else if (op[0]=='r'&&op[1]=='e'&&op[2]=='q')//request操作 请求资源 
		{
			scanf("%s",name);
			request(name);
		}
		else if (op[0]=='r'&&op[1]=='e'&&op[2]=='l')//release操作 释放资源 
		{
			scanf("%s",name);
			release(name);
		}
		else if (op[0]=='t'&&op[1]=='i')//tiemeout操作 时间片时间到 强制调度 
		{
			timeout();
		}
		else if (op[0]=='i'&&op[1]=='o'&&op[2]=='r'&&op[3]=='e'&&op[4]=='q')//iorequest操作 请求io 
		{
			scanf("%s",name);
			iorequest(name);
		}
		else if (op[0]=='i'&&op[1]=='o'&&op[2]=='r'&&op[3]=='e'&&op[4]=='l')//iorelease操作 释放io
		{
			scanf("%s",name);
			iorelease(name);
		}
		else if (op[0]=='e'&&op[1]=='x') break; 
		//showPriList();
		//showProcess();
		//showSource();
		//showIO();
		show();
	}
}

