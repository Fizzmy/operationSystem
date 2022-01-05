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
void addProcess(int sz,char *pos)//������������½��̣�ռ���ڴ��СΪsz���ڴ��׵�ַΪpos 
{
	PROCESSLIST *nwP=new PROCESSLIST;
	nwP->pos=pos;
	nwP->next=process->next;
	nwP->id=tot;
	nwP->len=sz;
	process->next=nwP;
}
bool getMemory(int sz,bool p)//�����СΪsz���ڴ� 
{
	if (!p)//������Ӧ 
	{
		MEMLIST *nw=head;
		while (nw->next!=NULL)
		{
			if (nw->next->len>sz)//�ҵ���һ������������λ�� 
			{
				addProcess(sz,nw->next->pos);//���̱�����½��� 
				nw->next->len=nw->next->len-sz;//�޸Ŀ����ڴ��С 
				nw->next->pos+=sz;//�޸Ŀ����ڴ��׵�ַ 
				used+=sz;//�޸���ʹ���ڴ� 
				return 1;//���ط���ɹ� 
			}
			else if (nw->next->len==sz)//��������������λ�� 
			{
				addProcess(sz,nw->next->pos);//���̱�����½��� 
				MEMLIST *del=nw->next;//ɾ�������ڴ�����Ŀ 
				nw->next=nw->next->next;
				delete del;
				used+=sz;
				return 1;
			}
			nw=nw->next;
		}
		return 0;//���򷵻ط���ʧ�� 
	}
	else//�����Ӧ 
	{
		MEMLIST *nw=head;
		MEMLIST *ch=NULL;
		int maxn=n+1;
		while (nw->next!=NULL)//�ҵ�������������С���б��� 
		{
			if (nw->next->len>=sz && nw->next->len<maxn)
				ch=nw,maxn=nw->next->len;
			nw=nw->next;
		}
		if (ch==NULL) return 0;//û�ҵ����ط���ʧ�� 
		//ִ�к�������Ӧ�㷨һ���Ĳ��� 
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
void releaseMemory(int sz,char *pos)//�ͷ��ڴ棬��СΪsz���׵�ַΪpos 
{
	MEMLIST *nw=head;
	while (nw->next!=NULL)//���������ڴ�� 
	{
		if (nw->next->pos+nw->next->len==pos)//�����ǰ��������ͷŵ��ڴ�֮ǰ 
		{
			nw->next->len+=sz;//��ǰ����ͷŵ��ڴ�ϲ� 
			if (nw->next->next!=NULL)
			{
				if (nw->next->next->pos==nw->next->pos+nw->next->len)//�����һ��������ͷŵ��ڴ�֮�� 
				{
					nw->next->len+=nw->next->next->len;//��һ��͵�ǰ��ϲ���ɾ����һ�� 
					MEMLIST *del=nw->next->next;
					nw->next->next=nw->next->next->next;
					delete del;
				}
			}
			return;
		}
		if (pos+sz==nw->next->pos)//�����ǰ��������ͷŵ��ڴ�֮�� 
		{
			nw->next->pos-=sz;//��ǰ����ͷŵ��ڴ�ϲ� 
			nw->next->len+=sz;
			return;
		}
		if (pos+sz<nw->next->pos)//�����ǰ�����ͷŵ��ڴ�֮���Ҳ����� 
		{
			MEMLIST *ins=new MEMLIST;//�ͷŵ��ڴ浥����Ϊһ�� 
			ins->next=nw->next;
			ins->len=sz;
			ins->pos=pos;
			nw->next=ins;
			return;
		}
		nw=nw->next;
	}
	//����ͷŵ��ڴ������������λ�ö������Ҳ������������������β 
	MEMLIST *ins=new MEMLIST;
	ins->next=NULL;
	ins->len=sz;
	ins->pos=pos;
	nw->next=ins;
	return;
}
void delProcess(int id)//ɾ�����������е�id������ 
{
	PROCESSLIST *nw=process;
	for (int i=1;i<id;i++)//�ҵ���id������ 
		nw=nw->next;
	used-=nw->next->len;
	nump--;
	printf("release memory of process %d , size of memory : %d .\n",nw->next->id,nw->next->len);
	releaseMemory(nw->next->len,nw->next->pos);//�ͷŸý���ռ�õ��ڴ� 
	PROCESSLIST *del=nw->next;
	nw->next=nw->next->next;
	delete del;
}
void show()//������з����� 
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
	printf("Size of memory: ");//�趨�洢����С 
	scanf("%d",&n);
	printf("Operation times: ");//�趨�������� 
	scanf("%d",&m);
	printf("Choose a fit method(0 for FF,1 for BF) : ");//�趨���ȷ���Ϊ������Ӧ���������Ӧ 
	scanf("%d",&method);
	head=new MEMLIST;//��ʼ�����з����� 
	MEMLIST* nw=new MEMLIST;
	nw->next=NULL;nw->pos=mem;nw->len=n;
	head->next=nw;
	process=new PROCESSLIST;process->next=NULL;//��ʼ�����̱� 
}
int main()
{
	init();
	for (int i=1;i<=m;i++)
	{
		while (1)
		{
			tot++;nump++;
			if (req[tot]==0) req[tot]=rand()%(n/5)+1;//��������ڴ��������У�Ϊ��Ч���������ԣ������С���Ʋ������ܴ�С��1/5�� 
			printf("process %d request %d memory:",tot,req[tot]);
			if (!getMemory(req[tot],method)) {printf("FAIL!\n");nump--;tot--;break;}//����ʧ�ܽ����ͷ��ڴ���� 
			else printf("SUCCESS!\n");//������������ڴ� 
			show();//��ʾ���з����� 
		}
		tot+=used;
		printf("Memory utilization : %.2f%\n",100.0*used/n);//����ڴ������� 
		delProcess(rand()%nump+1);//�������Ҫ�ͷ��ڴ�Ľ�����ţ��ͷŸý��̵��ڴ� 
		show();//������з����� 
	}
	printf("%f\n",1.0*tot/m);
}

