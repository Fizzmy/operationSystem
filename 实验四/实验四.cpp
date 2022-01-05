#include<cstdio>
#include<iostream>
using namespace std;
int n,m;//n进程个数，m资源个数 
int Available[1010];//可利用资源向量 
int Work[1010];//算法时使用，当前可利用资源向量 
int Max[1010][1010];//最大需求矩阵 
int Allocation[1010][1010];//分配矩阵 
int Need[1010][1010];//需求矩阵 
char op[10];
int req[1010];
int x;
bool Finish[1010];
int ans[1010];
bool check()//银行家算法检测死锁 
{
	for (int i=1;i<=n;i++) Finish[i]=0;
	for (int i=1;i<=m;i++) Work[i]=Available[i];
	int total=0,pre=-1;
	while (total!=pre)//如果算法这次会有新的进程finish则再次执行算法 
	{
		pre=total;//记录算法开始前finish的进程个数 
		for (int i=1;i<=n;i++)
		{
			if (Finish[i]) continue;
			bool can=1;
			for (int j=1;j<=m;j++)//判断进程i能否运行 
				if (Need[i][j]>Work[j]) {can=0;break;}
			if (!can) continue;
			Finish[i]=1;
			for (int j=1;j<=m;j++)//修改当前可利用资源向量 
				Work[j]+=Allocation[i][j];
			total++;
			ans[total]=i;//维护安全序列 
		}
	}
	return (total==n);
}
void init()
{
	freopen("data2.txt","r",stdin); 
	scanf("%d%d",&n,&m);
	for (int i=1;i<=m;i++)
		scanf("%d",&Available[i]);
	for (int i=1;i<=n;i++)
		for (int j=1;j<=m;j++)
			scanf("%d",&Max[i][j]);
	for (int i=1;i<=n;i++)
		for (int j=1;j<=m;j++)
			scanf("%d",&Allocation[i][j]);
	for (int i=1;i<=n;i++)
		for (int j=1;j<=m;j++)
			Need[i][j]=Max[i][j]-Allocation[i][j];
}
int main()
{
	init();
	freopen("CON","r",stdin);
	while (1)
	{
		scanf("%s%d",op,&x);//读取操作 
		if (x<1||x>n) {printf("ERROR: Invalid process.\n");continue;}
		for (int i=1;i<=m;i++)//读取操作向量 
			scanf("%d",&req[i]);
		if (op[2]=='q')//请求资源 
		{
			bool can=1;
			for (int i=1;i<=m;i++)
			{
				if (req[i]>Need[x][i]) {printf("ERROR: Process does not need so many resources.\n");can=0;break;}//进程不需要这么多资源，报错 
				if (req[i]>Available[i]) {printf("ERROR: Lack of resources.\n");can=0;break;}//当前剩余的资源不够分配，报错 
			}
			if (!can) continue;
			for (int i=1;i<=m;i++)//试探分配 
			{
				Available[i]-=req[i];
				Allocation[x][i]+=req[i];
				Need[x][i]-=req[i];
			}
			if (check())//处于安全状态 
			{
				printf("Request accepted.\n");//接收分配 
				printf("Security sequence: ");//输出安全序列 
				for (int i=1;i<=n;i++)
					printf("%d ",ans[i]);
				printf("\n");
			}
			else//不处于安全状态 
			{
				printf("Request is not accepted, it will cause a deadlock.\n");//报错 
				for (int i=1;i<=m;i++)//恢复 
				{
					Available[i]+=req[i];
					Allocation[x][i]-=req[i];
					Need[x][i]+=req[i];
				}
			}
		}
		else//释放资源 
		{
			bool can=1;
			for (int i=1;i<=m;i++)
				if (req[i]>Allocation[x][i]) {printf("ERROR: Process does not have so many resources.\n");can=0;break;}//进程没占有这么多资源，报错 
			if (!can) continue;
			for (int i=1;i<=m;i++)//更改数据结构 
			{
				Available[i]+=req[i];
				Allocation[x][i]-=req[i];
				Need[x][i]+=req[i];
			}
			printf("Release accepted.\n");
		}
	}
}

