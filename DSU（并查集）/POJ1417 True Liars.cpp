#include <iostream>
#include <string>
#include <cstring>
#define N 1005
using namespace std;

//t t yes
//t e no 
//e t no 
//e e yes
//通过以上分析有，回答yes两者为同类 
int root[N]; //并查集数组 
int relation[N]; //关系数组，0为同类，1不为同类 
int dp[N][N]; //背包容量为j时前i件物品的价值 
int setnum[N]; //每个根节点集合的个数 
int weight[2][N]; //动态规划weight数组（0代表同类，1代表不同类） 
int index[N]; //只记录根节点的索引 
int pre[N]; //dp查询路径用 

//寻找并查集根节点（带有路径压缩） 
int findroot(int v){
	if(v==root[v]){ //找到根节点 
		return v;
	}
	else{
		//更新根节点 
		int u=root[v];
		root[v]=findroot(root[v]); //递归查找
		//向上寻找根节点时，更新关系 
		relation[v]=(relation[v]+relation[u])%2; 
		return root[v]; //返回根节点 		
	} 
}

//合并a和b所在的集合 
void unionroot(int x, int y, int d){
	int rootx=findroot(x);
	int rooty=findroot(y);
	if(rootx==rooty){
		return;
	}
	//没有合并则合并
	root[rooty]=rootx; //向rootx合并
	relation[rooty]=(relation[x]-relation[y]+2+d)%2;
	setnum[rootx]=setnum[rootx]+setnum[rooty]; //rootx接管rooty下的元数个数 
	setnum[rooty]=0; 
}

void KNAP(int block,int p,int tt){
	dp[0][0]=1; //背包边界，和0号同类的有1个 
	for(int i=1;i<=block;i++){ //物品一共有block个 
		for(int j=0;j<=p;j++){ //价值最大允许装入为p，即天使的数量 
			if(j>=weight[0][i]){
				dp[i][j]=dp[i-1][j-weight[0][i]];
			}
			if(j>=weight[1][i]){
				dp[i][j]+=dp[i-1][j-weight[1][i]];
			}
		}
	}
	if(dp[block][p]==1){
		int j=p;
		for(int i=block;i>=1;i--){
			if(dp[i][j]==dp[i-1][j-weight[0][i]]){
				pre[i]=0;
				j=j-weight[0][i];
			}
			else if(dp[i][j]==dp[i-1][j-weight[1][i]]){
				pre[i]=1;
				j=j-weight[1][i];
			}
		}
		for(int i=1;i<=tt;i++){
			if(pre[index[root[i]]]==relation[i]){
				cout<<i<<endl;
			}
		}
		cout<<"end"<<endl; 
	}
	else{
		cout<<"no"<<endl;
	} 
} 
int main(){
	int n,p1,p2,tt,block;
	int x,y;
	string a;
	while(cin>>n>>p1>>p2&&(n||p1||p2)){
		memset(weight,0,sizeof(weight));
        memset(dp,0,sizeof(dp));
        memset(index,0,sizeof(index));
        memset(pre,0,sizeof(pre));
		tt=p1+p2;  //动物总个数 
		for(int i=1;i<=tt;i++){
			root[i]=i; //初始化并查集
			relation[i]=0; //初始化关系数组
			setnum[i]=1; //每个根节点有一个元素 
		}
		//输入元素，完成合并和关系确认，构造并查集 
		for(int i=1;i<=n;i++){
			cin>>x>>y>>a;
			if(a=="yes"){ //表示是同类 
				unionroot(x,y,0);
			}
			else{ //不是同类 
				unionroot(x,y,1); 
			}
		}
		block=0;
		for(int i=1;i<=tt;i++){
			if(setnum[i]!=0){ //如果是一个根节点 
				block+=1; //块的数量加一
				index[i]=block; //记录i的块索引 
			}
		}
		for(int i=1;i<=tt;i++){
			findroot(i);
			//对应块背包重量加一
			//只有根节点对应的序号有价值，分别记录其下和根节点同类、不同类的数量  
			weight[relation[i]][index[root[i]]]+=1;
		}
		//进入背包查询的过程 
		KNAP(block,p1,tt);
	}
	return 0; 	
}

