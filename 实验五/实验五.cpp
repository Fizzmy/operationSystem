#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<iostream>
#include<algorithm>
#include<ctime>
#include<set>
using namespace std;
int n,T;
int op,pos;
int List[10010],tot;
int sortL[10010];
void request(int x)//����x�ŵ� 
{
	printf("ADD %d\n",x);
	List[++tot]=x;
}
void release(int pos)//�ͷŵ�i������ 
{
	printf("DEL %d\n",pos);
	for (int i=pos;i<tot;i++)
		List[i]=List[i+1];
	tot--;
}
int abss(int x){return x<0?-x:x;}//�����ֵ 
void showList()
{
	for (int i=1;i<=tot;i++) printf("%d ",List[i]);
	printf("\n");
}
void getDis()
{
	//SCAN
	for (int i=1;i<=tot;i++)
		sortL[i]=List[i];
	sort(sortL+1,sortL+1+tot);
	int ps=lower_bound(sortL+1,sortL+1+tot,pos)-sortL;//�ҵ���һ��Ҫ���ʵĴŵ� 
	int nps=pos;
	int ans=0;
	for (int i=ps;i<=tot;i++)
		ans+=abss(sortL[i]-nps),nps=sortL[i];
	for (int i=ps-1;i>=1;i--)//ɨ�����ֵ��ʼ��ɨ 
		ans+=abss(nps-sortL[i]),nps=sortL[i];
	printf("SCAN: %.5f\n",1.0*ans/tot);
	
	//FCFS
	ans=0;
	nps=pos;
	for (int i=1;i<=tot;i++)//ģ�� 
		ans+=abss(List[i]-nps),nps=List[i];
	printf("FCFS: %.5f\n",1.0*ans/tot);
	
	//SSTF
	ans=0;
	nps=pos; 
	multiset<int>setL;//���������ظ�Ԫ�ص�set 
	for (int i=1;i<=tot;i++) setL.insert(List[i]);
	for (int i=1;i<=tot;i++)
	{
		auto it=setL.lower_bound(pos);//�ҵ���ǰ�ŵ���ʣ�༯���б���С��������ͱ��������С�� 
		if (it==setL.begin())//�������� 
			ans+=abss(*it-nps),nps=*it,setL.erase(it);
		else if (it==setL.end())//������С 
		{
			it--;
			ans+=abss(*it-nps),nps=*it,setL.erase(it);
		}
		else//��ͨ��� 
		{
			int nw=*it;
			it--;
			int pre=*it;
			if (abss(nw-nps)<=abss(pre-nps)) it++;//ѡ������� 
			ans+=abss(*it-nps),nps=*it,setL.erase(it);
		}
	}
	printf("SSTF: %.5f\n",1.0*ans/tot);
	printf("--------------------------\n");
}
int main()
{
	printf("Please input track number: ");//�������ŵ��� 
	scanf("%d",&T);
	printf("Please input operation time: "); //��������� 
	scanf("%d",&n);
	printf("Please input now position: ");//�����ǰ��ͷλ�� 
	scanf("%d",&pos);
	srand(time(0));
	for (int i=1;i<=n;i++)
	{
		if (tot<=1)//��������С�ڵ���1����һ��һ����������� 
			request(rand()%T+1);
		else
		{
			//�������������ͷţ�����2��1 
			if (rand()%3)
				request(rand()%T+1);
			else release(rand()%tot+1);
		}
		showList();//���������� 
		getDis();//��������㷨��ƽ���Ʊ۾��� 
	}
}

