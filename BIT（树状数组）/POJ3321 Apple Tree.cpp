#include <cstdio>
#include <vector>
#define lowbit(x) ((x)&(-x)) 
#define N 100010
using namespace std; 

int in[N],out[N]; //入序号和出序号 
int c[N],flag[N];  //树状数组和，是否有苹果标志 
vector<int> mylist[N]; //邻接表 
int count; 

//树状数组的getsum函数：返回前x个整数的和
int getsum(int x){
	int sum=0;
	for(int i=x;i>0;i=i-lowbit(i)){
		sum=sum+c[i]; //根据c[i]，累计求出和 
	}
	return sum; 
} 

//给第x个整数加上v，更新c[i] 
void update(int x,int add){
	for(int i=x;i<=N;i=i+lowbit(i)){
		c[i]=c[i]+add; //更新和x号整数有关的所有c[i] 
	}
}

//用dfs遍历,求出编号和入编号
void DFS(int x){
	in[x]=count;
	int size_x=mylist[x].size();
	for(int i=0;i<size_x;i++){ //遍历x的所有邻居节点 
		count=count+1;
		DFS(mylist[x][i]); 
	}
	out[x]=count;
}
 
int main(){
	int n;
	scanf("%d",&n);
	int u,v;
	for(int i=1;i<=n-1;i++){
		scanf("%d %d",&u,&v);
		mylist[u].push_back(v); //边的连接关系输入邻接表 
	}
	count=1;
	DFS(1); //调用DFS初始化in和out数组
	for(int i=1;i<=n;i++){ //初始化所有的节点 
		flag[i]=1;
		update(i,1); //树状数组每一个节点加1 
	} 
	int m;
	char type;
	int num;
	scanf("%d",&m);
	for(int i=0;i<m;i++){
		scanf("%s %d",&type,&num);
		if(type=='Q'){ //如果是查询，则输出个数 
			printf("%d\n",getsum(out[num])-getsum(in[num]-1));
		}
		else if(type=='C'){ //如果是更改，则调用update更改 
			if(flag[num]==1){ //如果该树上有苹果 
				flag[num]=0; //改为没有苹果 
				update(in[num],-1);
			}
			else{ //本身没有苹果 
				flag[num]=1; //变为有苹果状态 
				update(in[num],1); 
			}
		} 
	}
	return 0;
}
