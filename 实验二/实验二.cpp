#include<cstdio>
#include<iostream>
#include<vector>
#include<cstring>
using namespace std;
struct PROCESSLIST{//�������� 
	int id;
	PROCESSLIST* next;
};
struct RCB{//��Դ���ݽṹ 
	int id;//��Դrid 
	vector<char>name; //��Դ���� 
	bool status;//״̬��0����1ռ�� 
	PROCESSLIST *head,*tail;//������Դ�����Ľ�������headָ������ͷtailָ������β 
	RCB(int _=0,bool __=0)//��ʼ�� 
	{
		id=_,status=__;
		head=new PROCESSLIST;head->next=NULL;
		tail=head;
	}
}source[1100];
struct IO{//io���ݽṹ 
	int id;//io��id 
	vector<char>name;//io���� 
	bool status;//״̬��0����1ռ�� 
	PROCESSLIST *head,*tail;//����io�����Ľ�������headָ������ͷtailָ������β 
	IO(int _=0,bool __=0)//��ʼ�� 
	{
		id=_,status=__;
		head=new PROCESSLIST;head->next=NULL;
		tail=head;
	}
}io[1100];
struct SOURCELIST{//��Դ���� 
	int id; 
	SOURCELIST* next;
};
struct IOLIST{//IO���� 
	int id;
	IOLIST* next;
};
struct PCB{//�������ݽṹ 
	int id;//����id 
	vector<char>name; //������ 
	SOURCELIST *head;//����ռ����Դ�б� 
	IOLIST *headio;//����ռ��IO�б� 
	int status;//״̬�� 0����1����2����3�ݻ� 
	pair<int,int> wait;//����״̬Ϊ2ʱ��Ч�����������ʲô��������first��ʾid��second��ʾ����Դ����io 
	PROCESSLIST *pos;//������״̬���е�λ�� 
	int fa;//������id 
	vector<int>son;//�ӽ���id 
	int pri;//�������ȼ� 
	PCB(int _=0,SOURCELIST *__=NULL,IOLIST *___=NULL,int ____=-1,PROCESSLIST *_____=NULL,int ______=-1,int _______=-1)
	{//��ʼ�� 
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
struct QUEUE{//���ȼ����� 
	PROCESSLIST *head,*tail;
}q[110];
int n;
char op[20];
char name[20];
int rt=1,tr[3][2010][62],sz[3],p,tag[3][2010],tot[3];//�ֵ���������ݽṹ 
int nowProcess=0;//��ǰ���еĽ��� 
int tf(char c)//�ַ�ת����ѹ���ֵ����ռ� 
{
	if (c>='A'&&c<='Z') return c-'A';
	else if (c>='a'&&c<='z') return c-'a'+26;
	else if (c>='0'&&c<='9') return c-'0'+52;
	return -1; 
}
int insert(int p,char *s,bool ins=0,bool find=0,bool del=0)
//ins/find/delΪ1�ֱ��ʾ���ֵ����в���/����/ɾ���ַ�����pΪ0��ʾ�����ֵ�����1��Դ�ֵ�����2io�ֵ��� 
{
	int nw=rt;
	int n=strlen(s);
	for (int i=0;i<n;i++)
	{
		int nxt=tf(s[i]);
		if (!tr[p][nw][nxt])
			tr[p][nw][nxt]=++sz[p];
		nw=tr[p][nw][nxt]; //�����ַ����ֵ������ƶ� 
	}
	if (del)//ɾ�� 
	{
		if (!tag[p][nw]) return -1;//��λ��û�б�ǣ����ش��� 
		int id=tag[p][nw];
		tag[p][nw]=0;//�����0 
		return id;
	}
	if (find)//���� 
	{
		if (!tag[p][nw]) return -1;//��λ��û��ǣ����ش��� 
		return tag[p][nw];
	}
	if (ins)//���� 
	{
		if (!tag[p][nw]) return tag[p][nw]=++tot[p];
		return -1;//��λ���б�ǣ����ش��� 
	}
}
PROCESSLIST* insertQueue(int pri,int id)//��ӽ���id�����ȼ�pri�Ķ��У����ز����id��ָ�� 
{
	PROCESSLIST *nw=new PROCESSLIST;
	nw->next=NULL,nw->id=id;
	q[pri].tail->next=nw;
	q[pri].tail=nw;
	return nw;
}
void deleteQueue(int pri)//�Ӿ�������pri��ɾ����ͷ���� 
{
	if (q[pri].head->next==q[pri].tail)//���ɾ������б�Ϊ�գ��������⴦�� 
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
void schedule()//�����㷨 
{
	int maxid=-1;
	for (int i=n;i>=0;i--)//�ҵ��������������ȼ���ߵĽ��� 
		if (q[i].head->next!=NULL)
		{
			maxid=q[i].head->next->id;
			break;
		}
	if (maxid==-1) return;//��������û�н��̣�����Ҫ���ȣ�ֱ�ӷ��� 
	if (nowProcess==-1 || process[nowProcess].status!=1)
	//��ǰû�����еĽ��̻��ߵ�ǰ���̲������� 
	{
		nowProcess=maxid;//���о������������ȼ���ߵĽ���
		process[nowProcess].status=1;
		process[nowProcess].pos=NULL;
		deleteQueue(process[nowProcess].pri);//�Ӿ���������ɾ���ý��� 
		return;
	}
	if (process[nowProcess].status==1 && process[maxid].pri>process[nowProcess].pri)
	//��ǰ���еĽ��������������Ҿ������������ȼ���ߵĽ��̴��ڵ�ǰ���� 
	{
		//��ǰ���̼����Ӧ�������� 
		process[nowProcess].pos=insertQueue(process[nowProcess].pri,nowProcess);
		process[nowProcess].status=0;
		nowProcess=maxid;//���о������������ȼ���ߵĽ���
		process[nowProcess].status=1;
		process[nowProcess].pos=NULL;
		deleteQueue(process[nowProcess].pri);//�Ӿ���������ɾ���ý��� 
	}
}
void create(char *s,int pri)//����һ���½��� 
{
	int id=insert(0,s,1);//�½����������ֵ��������ؽ���id 
	if (id==-1) {printf("ERROR:Process name has been used.\n");return;}
	//����������Ѵ��� ���ش��� 
	process[nowProcess].son.push_back(id);//��ӵ�ǰ�����ӽ��� 
	process[id]=PCB{id,NULL,NULL,0,NULL,nowProcess,pri};//�����½��� 
	process[id].pos=insertQueue(pri,id);//�ڶ�Ӧ���ȼ��ľ���������ӽ��̣����ؽ��̵�λ�� 
	int len=strlen(s);
	for (int i=0;i<len;i++) process[id].name.push_back(s[i]);//�洢������ 
	schedule();//���� 
}
bool deleteSoucre(int nowProcess,int id)//�ڽ���nowProcessռ����Դ������ɾ����Դid
{
	SOURCELIST *nw=process[nowProcess].head;
	while (nw->next!=NULL)//������������ 
	{
		if (nw->next->id==id)//�ҵ���Դ��ִ��ɾ�����������سɹ� 
		{
			SOURCELIST *p=nw->next;
			nw->next=nw->next->next;
			delete p;
			return 1;
		}
		nw=nw->next;
	}
	//δ�ҵ���Դ������ʧ�� 
	return 0;
}
int deleteProcess(int id)//ȡ��������Դ�����Ľ��̶����еĶ�ͷ���� 
{
	int nowId=source[id].head->next->id;
	if (source[id].head->next==source[id].tail)//ɾ�����̺���б�Ϊ�գ����⴦�� 
	{
		source[id].head->next=NULL;
		delete source[id].tail;
		source[id].tail=source[id].head; 
		return nowId;
	}
	//����ɾ�������� 
	PROCESSLIST *nxt=source[id].head->next;
	source[id].head->next=source[id].head->next->next;
	delete nxt;
	return nowId;
}
void insertSource(int nowProcess,int id)//�ڽ���nowProcessռ����Դ�����м�����Դid 
{
	SOURCELIST *nw=new SOURCELIST;
	nw->id=id; 
	nw->next=process[nowProcess].head->next;
	process[nowProcess].head->next=nw;
}
void insertProcess(int id,int nowProcess)//�ڱ���Դid�����Ľ��̶����м������nowProcess 
{
	PROCESSLIST *nw=new PROCESSLIST;
	nw->id=nowProcess;
	nw->next=NULL;
	source[id].tail->next=nw;
	source[id].tail=nw;
}
void request(char *s)//������Դ 
{
	int id=insert(1,s,0,1,0);//�ֵ����в�����Դ������id 
	if (id==-1)//û�и���Դ 
	{
		int len=strlen(s);
		id=insert(1,s,1,0,0);//�ֵ����в�����Դ������id 
		source[id]=RCB{id,0};//��ʼ������Դ 
		for (int i=0;i<len;i++)//��¼��Դ�� 
			source[id].name.push_back(s[i]);
	}
	if (source[id].status==0)//��ǰ��ԴΪ������Դ 
	{
		source[id].status=1;//ռ�õ�ǰ��Դ 
		insertSource(nowProcess,id);//�ڵ�ǰ����ռ����Դ�����м������Դ 
	}
	else
	{
		process[nowProcess].status=2;//��ǰ�������� 
		process[nowProcess].wait=make_pair(id,1);//���ý��̵�����ԭ������Դid������ 
		insertProcess(id,nowProcess);//�ڱ�����Դ�����Ľ��̶����м���ý��� 
		schedule();//���� 
	}
}

void release(char *s)//�ͷ���Դ 
{
	int id=insert(1,s,0,1,0);//�ֵ����в�����Դ������id 
	if (id==-1){printf("ERROR:Source name does not exist.\n");return;}//δ�ҵ���Դ�������� 
	if (!deleteSoucre(nowProcess,id)){printf("ERROR:Process does not use this source.\n");return;}
	//�ڵ�ǰ����ռ����Դ������ɾ������Դ����δ�ҵ�����Դ������ 
	if (source[id].head==source[id].tail)//���������Դ�����Ľ��̶���Ϊ�գ���������Դ״̬Ϊ���� 
		source[id].status=0;
	else
	{
		//����ȡ����ͷ���� 
		int idx=deleteProcess(id);
		process[idx].status=0;//���ý��̾��� 
		process[idx].pos=insertQueue(process[idx].pri,idx);//���̼���������� 
		process[idx].wait=make_pair(0,0);//���wait 
		insertSource(idx,id);//�ڵ�ǰ����ռ����Դ�����м������Դ 
		schedule();//���� 
	}
}
//------------������io����������Դ������ͬ--------------// 
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
//------------������io����������Դ������ͬ--------------// 
void timeout()//ʱ��Ƭʱ�䵽��ǿ�Ƶ���timeout 
{
	for (int i=n;i>=0;i--)
	{
		if (q[i].head->next!=NULL)//����������Ѱ�����ȼ���ߵĽ��� 
		{
			int pre=nowProcess;
			nowProcess=q[i].head->next->id;
			deleteQueue(process[nowProcess].pri);//�½��̴Ӿ���������ɾ�� 
			process[nowProcess].status=1;//����״̬ 
			process[nowProcess].pos=NULL;
			process[pre].pos=insertQueue(process[pre].pri,pre);//��ǰ���̼���������� 
			process[pre].status=0;//����״̬ 
			return;
		}
	}
}
void destory()//ɾ������ 
{
	SOURCELIST *nw=process[nowProcess].head;
	while (nw->next!=NULL)//ȡ��ռ�����е���Դ 
	{
		SOURCELIST *pre=nw;
		int id=nw->next->id;
		if (source[id].head==source[id].tail)
			source[id].status=0;
		else
		{
			int idx=deleteProcess(id);
			process[idx].status=0;//���ý��̾��� 
			process[idx].pos=insertQueue(process[idx].pri,idx);//���̼���������� 
			process[idx].wait=make_pair(0,0);//���wait 
			insertSource(idx,id);//�ڵ�ǰ����ռ����Դ�����м������Դ 
		}
		nw=nw->next;
		delete pre;//�ͷſռ� 
	}
	delete nw;
	process[nowProcess].head=NULL; 
	IOLIST *nw2=process[nowProcess].headio;
	while (nw2->next!=NULL)//ȡ��ռ�����е�IO 
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
	insert(0,name,0,0,1); //���ֵ�����ɾ���ý����� 
	process[nowProcess].status=3;//���ý���״̬Ϊɾ�� 
	nowProcess=-1;//��ǰδ���н��� 
	schedule();//���� 
}
void show()//չʾ��ǰ���н��̵�״̬ 
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
void showPriList()//չʾ��ǰ���еľ������� 
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
void showProcess()//չʾ��ǰ���еĽ�����Ϣ 
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
void showSource()//չʾ��ǰ���е���Դ��Ϣ 
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
void showIO()//չʾ��ǰ���е�IO��Ϣ 
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
	process[0]=PCB{0,NULL,NULL,1,NULL,-1,0};//��ʼ��init���� 
	process[0].name.push_back('i');process[0].name.push_back('n');process[0].name.push_back('i');process[0].name.push_back('t');
	printf("Max Number of Priority:");//����������ȼ� 
	scanf("%d\n",&n);
	for (int i=0;i<=n;i++) q[i].head=new PROCESSLIST,q[i].head->next=NULL,q[i].tail=q[i].head;
	while (1)
	{
		int pri;
		scanf("%s",op);
		if (op[0]=='c'&&op[1]=='r')//create���� �������� 
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
		else if (op[0]=='d'&&op[1]=='e')//destory���� �ݻٵ�ǰ���� 
		{
			destory();
		}
		else if (op[0]=='r'&&op[1]=='e'&&op[2]=='q')//request���� ������Դ 
		{
			scanf("%s",name);
			request(name);
		}
		else if (op[0]=='r'&&op[1]=='e'&&op[2]=='l')//release���� �ͷ���Դ 
		{
			scanf("%s",name);
			release(name);
		}
		else if (op[0]=='t'&&op[1]=='i')//tiemeout���� ʱ��Ƭʱ�䵽 ǿ�Ƶ��� 
		{
			timeout();
		}
		else if (op[0]=='i'&&op[1]=='o'&&op[2]=='r'&&op[3]=='e'&&op[4]=='q')//iorequest���� ����io 
		{
			scanf("%s",name);
			iorequest(name);
		}
		else if (op[0]=='i'&&op[1]=='o'&&op[2]=='r'&&op[3]=='e'&&op[4]=='l')//iorelease���� �ͷ�io
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

