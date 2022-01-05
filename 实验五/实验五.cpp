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
void request(int x)//请求x磁道 
{
	printf("ADD %d\n",x);
	List[++tot]=x;
}
void release(int pos)//释放第i个请求 
{
	printf("DEL %d\n",pos);
	for (int i=pos;i<tot;i++)
		List[i]=List[i+1];
	tot--;
}
int abss(int x){return x<0?-x:x;}//求绝对值 
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
	int ps=lower_bound(sortL+1,sortL+1+tot,pos)-sortL;//找到下一个要访问的磁道 
	int nps=pos;
	int ans=0;
	for (int i=ps;i<=tot;i++)
		ans+=abss(sortL[i]-nps),nps=sortL[i];
	for (int i=ps-1;i>=1;i--)//扫完最大值开始回扫 
		ans+=abss(nps-sortL[i]),nps=sortL[i];
	printf("SCAN: %.5f\n",1.0*ans/tot);
	
	//FCFS
	ans=0;
	nps=pos;
	for (int i=1;i<=tot;i++)//模拟 
		ans+=abss(List[i]-nps),nps=List[i];
	printf("FCFS: %.5f\n",1.0*ans/tot);
	
	//SSTF
	ans=0;
	nps=pos; 
	multiset<int>setL;//加入允许重复元素的set 
	for (int i=1;i<=tot;i++) setL.insert(List[i]);
	for (int i=1;i<=tot;i++)
	{
		auto it=setL.lower_bound(pos);//找到当前磁道在剩余集合中比它小的最大数和比它大的最小数 
		if (it==setL.begin())//都比它大 
			ans+=abss(*it-nps),nps=*it,setL.erase(it);
		else if (it==setL.end())//都比它小 
		{
			it--;
			ans+=abss(*it-nps),nps=*it,setL.erase(it);
		}
		else//普通情况 
		{
			int nw=*it;
			it--;
			int pre=*it;
			if (abss(nw-nps)<=abss(pre-nps)) it++;//选出最近的 
			ans+=abss(*it-nps),nps=*it,setL.erase(it);
		}
	}
	printf("SSTF: %.5f\n",1.0*ans/tot);
	printf("--------------------------\n");
}
int main()
{
	printf("Please input track number: ");//输入最大磁道数 
	scanf("%d",&T);
	printf("Please input operation time: "); //输出操作数 
	scanf("%d",&n);
	printf("Please input now position: ");//输出当前磁头位置 
	scanf("%d",&pos);
	srand(time(0));
	for (int i=1;i<=n;i++)
	{
		if (tot<=1)//请求数量小于等于1，下一次一定是请求操作 
			request(rand()%T+1);
		else
		{
			//否则随机请求或释放，比例2：1 
			if (rand()%3)
				request(rand()%T+1);
			else release(rand()%tot+1);
		}
		showList();//输出请求队列 
		getDis();//求出三种算法的平均移臂距离 
	}
}

