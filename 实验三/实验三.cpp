#include<cstdio>
#include<iostream>
#include<ctime>
#include<cstdlib>
using namespace std;
char mem[100010];
struct MEMLIST{
	int len;
	char *pos;
	MEMLIST *next;
};
struct PROCESSLIST{
	char *pos;
	int id;
	int len;
	PROCESSLIST *next;
};
MEMLIST* head;
int tot,n,m;
int req[100010];
int nump;
PROCESSLIST *process;
int used=0;
void addProcess(int sz,char *pos)//进程链表添加新进程，占用内存大小为sz，内存首地址为pos 
{
	PROCESSLIST *nwP=new PROCESSLIST;
	nwP->pos=pos;
	nwP->next=process->next;
	nwP->id=tot;
	nwP->len=sz;
	process->next=nwP;
}
bool getMemory(int sz,bool p)//请求大小为sz的内存 
{
	if (!p)//最先适应 
	{
		MEMLIST *nw=head;
		while (nw->next!=NULL)
		{
			if (nw->next->len>sz)//找到第一个符合条件的位置 
			{
				addProcess(sz,nw->next->pos);//进程表添加新进程 
				nw->next->len=nw->next->len-sz;//修改空闲内存大小 
				nw->next->pos+=sz;//修改空闲内存首地址 
				used+=sz;//修改已使用内存 
				return 1;//返回分配成功 
			}
			else if (nw->next->len==sz)//如果正好消耗完该位置 
			{
				addProcess(sz,nw->next->pos);//进程表添加新进程 
				MEMLIST *del=nw->next;//删除空闲内存表该条目 
				nw->next=nw->next->next;
				delete del;
				used+=sz;
				return 1;
			}
			nw=nw->next;
		}
		return 0;//否则返回分配失败 
	}
	else//最佳适应 
	{
		MEMLIST *nw=head;
		MEMLIST *ch=NULL;
		int maxn=n+1;
		while (nw->next!=NULL)//找到满足条件的最小空闲表项 
		{
			if (nw->next->len>=sz && nw->next->len<maxn)
				ch=nw,maxn=nw->next->len;
			nw=nw->next;
		}
		if (ch==NULL) return 0;//没找到返回分配失败 
		//执行和最先适应算法一样的操作 
		if (ch->next->len>sz)
		{
			addProcess(sz,ch->next->pos);
			ch->next->len=ch->next->len-sz;
			ch->next->pos+=sz;
			used+=sz;
			return 1;
		}
		else if (ch->next->len==sz)
		{
			addProcess(sz,ch->next->pos);
			MEMLIST *del=ch->next;
			ch->next=ch->next->next;
			delete del;
			used+=sz;
			return 1;
		}
	}
}
void releaseMemory(int sz,char *pos)//释放内存，大小为sz，首地址为pos 
{
	MEMLIST *nw=head;
	while (nw->next!=NULL)//遍历空闲内存表 
	{
		if (nw->next->pos+nw->next->len==pos)//如果当前项紧靠在释放的内存之前 
		{
			nw->next->len+=sz;//当前项和释放的内存合并 
			if (nw->next->next!=NULL)
			{
				if (nw->next->next->pos==nw->next->pos+nw->next->len)//如果下一项紧靠在释放的内存之后 
				{
					nw->next->len+=nw->next->next->len;//下一项和当前项合并，删除下一项 
					MEMLIST *del=nw->next->next;
					nw->next->next=nw->next->next->next;
					delete del;
				}
			}
			return;
		}
		if (pos+sz==nw->next->pos)//如果当前项紧靠在释放的内存之后 
		{
			nw->next->pos-=sz;//当前项和释放的内存合并 
			nw->next->len+=sz;
			return;
		}
		if (pos+sz<nw->next->pos)//如果当前项在释放的内存之后且不紧靠 
		{
			MEMLIST *ins=new MEMLIST;//释放的内存单独作为一项 
			ins->next=nw->next;
			ins->len=sz;
			ins->pos=pos;
			nw->next=ins;
			return;
		}
		nw=nw->next;
	}
	//如果释放的内存比链表的所有项都位置都靠后且不紧靠，将其放入链表尾 
	MEMLIST *ins=new MEMLIST;
	ins->next=NULL;
	ins->len=sz;
	ins->pos=pos;
	nw->next=ins;
	return;
}
void delProcess(int id)//删除进程链表中第id个进程 
{
	PROCESSLIST *nw=process;
	for (int i=1;i<id;i++)//找到第id个进程 
		nw=nw->next;
	used-=nw->next->len;
	nump--;
	printf("release memory of process %d , size of memory : %d .\n",nw->next->id,nw->next->len);
	releaseMemory(nw->next->len,nw->next->pos);//释放该进程占用的内存 
	PROCESSLIST *del=nw->next;
	nw->next=nw->next->next;
	delete del;
}
void show()//输出空闲分区表 
{
	printf("-----------\n");
	MEMLIST *nw=head;
	while (nw->next!=NULL)
	{
		printf("len:%d pos:%d\n",nw->next->len,nw->next->pos-mem);
		nw=nw->next;
	}
	printf("-----------\n");
}
bool method;
void init()
{
	srand(time(0));
	printf("Size of memory: ");//设定存储区大小 
	scanf("%d",&n);
	printf("Operation times: ");//设定操作次数 
	scanf("%d",&m);
	printf("Choose a fit method(0 for FF,1 for BF) : ");//设定调度方法为最先适应还是最佳适应 
	scanf("%d",&method);
	head=new MEMLIST;//初始化空闲分区表 
	MEMLIST* nw=new MEMLIST;
	nw->next=NULL;nw->pos=mem;nw->len=n;
	head->next=nw;
	process=new PROCESSLIST;process->next=NULL;//初始化进程表 
}
int main()
{
	init();
	for (int i=1;i<=m;i++)
	{
		while (1)
		{
			tot++;nump++;
			if (req[tot]==0) req[tot]=rand()%(n/5)+1;//随机生成内存请求序列（为了效果更加明显，请求大小限制不超过总大小的1/5） 
			printf("process %d request %d memory:",tot,req[tot]);
			if (!getMemory(req[tot],method)) {printf("FAIL!\n");nump--;tot--;break;}//请求失败进入释放内存过程 
			else printf("SUCCESS!\n");//否则继续请求内存 
			show();//显示空闲分区表 
		}
		tot+=used;
		printf("Memory utilization : %.2f%\n",100.0*used/n);//输出内存利用率 
		delProcess(rand()%nump+1);//随机生成要释放内存的进程序号，释放该进程的内存 
		show();//输出空闲分区表 
	}
	printf("%f\n",1.0*tot/m);
}

