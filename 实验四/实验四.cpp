#include<cstdio>
#include<iostream>
using namespace std;
int n,m;//n���̸�����m��Դ���� 
int Available[1010];//��������Դ���� 
int Work[1010];//�㷨ʱʹ�ã���ǰ��������Դ���� 
int Max[1010][1010];//���������� 
int Allocation[1010][1010];//������� 
int Need[1010][1010];//������� 
char op[10];
int req[1010];
int x;
bool Finish[1010];
int ans[1010];
bool check()//���м��㷨������� 
{
	for (int i=1;i<=n;i++) Finish[i]=0;
	for (int i=1;i<=m;i++) Work[i]=Available[i];
	int total=0,pre=-1;
	while (total!=pre)//����㷨��λ����µĽ���finish���ٴ�ִ���㷨 
	{
		pre=total;//��¼�㷨��ʼǰfinish�Ľ��̸��� 
		for (int i=1;i<=n;i++)
		{
			if (Finish[i]) continue;
			bool can=1;
			for (int j=1;j<=m;j++)//�жϽ���i�ܷ����� 
				if (Need[i][j]>Work[j]) {can=0;break;}
			if (!can) continue;
			Finish[i]=1;
			for (int j=1;j<=m;j++)//�޸ĵ�ǰ��������Դ���� 
				Work[j]+=Allocation[i][j];
			total++;
			ans[total]=i;//ά����ȫ���� 
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
		scanf("%s%d",op,&x);//��ȡ���� 
		if (x<1||x>n) {printf("ERROR: Invalid process.\n");continue;}
		for (int i=1;i<=m;i++)//��ȡ�������� 
			scanf("%d",&req[i]);
		if (op[2]=='q')//������Դ 
		{
			bool can=1;
			for (int i=1;i<=m;i++)
			{
				if (req[i]>Need[x][i]) {printf("ERROR: Process does not need so many resources.\n");can=0;break;}//���̲���Ҫ��ô����Դ������ 
				if (req[i]>Available[i]) {printf("ERROR: Lack of resources.\n");can=0;break;}//��ǰʣ�����Դ�������䣬���� 
			}
			if (!can) continue;
			for (int i=1;i<=m;i++)//��̽���� 
			{
				Available[i]-=req[i];
				Allocation[x][i]+=req[i];
				Need[x][i]-=req[i];
			}
			if (check())//���ڰ�ȫ״̬ 
			{
				printf("Request accepted.\n");//���շ��� 
				printf("Security sequence: ");//�����ȫ���� 
				for (int i=1;i<=n;i++)
					printf("%d ",ans[i]);
				printf("\n");
			}
			else//�����ڰ�ȫ״̬ 
			{
				printf("Request is not accepted, it will cause a deadlock.\n");//���� 
				for (int i=1;i<=m;i++)//�ָ� 
				{
					Available[i]+=req[i];
					Allocation[x][i]-=req[i];
					Need[x][i]+=req[i];
				}
			}
		}
		else//�ͷ���Դ 
		{
			bool can=1;
			for (int i=1;i<=m;i++)
				if (req[i]>Allocation[x][i]) {printf("ERROR: Process does not have so many resources.\n");can=0;break;}//����ûռ����ô����Դ������ 
			if (!can) continue;
			for (int i=1;i<=m;i++)//�������ݽṹ 
			{
				Available[i]+=req[i];
				Allocation[x][i]-=req[i];
				Need[x][i]+=req[i];
			}
			printf("Release accepted.\n");
		}
	}
}

