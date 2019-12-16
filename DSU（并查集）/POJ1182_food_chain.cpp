#include <iostream>
#define N 50010
using namespace std;

//声明并查集数组root，表示当前点和根节点是否判断过关系。 
//声明关系数组relation，0为两者同类，1为根节点吃该节点，2为该节点吃根节点 
int root[N];
int relation[N];
int sum=0; //错误次数 
int n,k; //动物数量，话数量 
int d,x,y; //说法，x动物，y动物 

//寻找并查集根节点（带有路径压缩） 
int findroot(int v){
	if(v==root[v]){ //找到根节点 
		return v;
	}
	else{
		//向上寻找根节点时，更新关系 
		relation[v]=(relation[v]+relation[root[v]]+3)%3; 
		//更新根节点 
		int R=findroot(root[v]); //递归查找 
		root[v]=R; //返回根节点
		return R; 		
	} 
}

//合并a和b所在的集合 
bool unionroot(int x, int y, int d){
	int rootx=findroot(x);
	int rooty=findroot(y);
	//如果已经合并，更新错误关系 
	if(rootx==rooty){
		//如果x和y为同类，比较根节点 
		if(d==1&&relation[x]!=relation[y]){
			return false;
		}
		//如果x吃y，比较根节点 
		if(d==2){
			if(relation[x]==0&&relation[y]!=2){
				return false;
			}
			else if(relation[x]==1&&relation[y]!=0){
				return false;
			}
			else if(relation[x]==2&&relation[y]!=1){
				return false;
			}
		}
		return true;
	}
	//没有合并则合并
	root[rooty]=rootx; //向rooty合并
	relation[rooty]=(relation[x]-relation[y]+3+d)%3;
	return true;
}

int main(){
	cin>>n>>k;
	//初始化并查集和关系数组 
	for(int i=1;i<=n;i++){
		root[i]=i;
		relation[i]=0; 
	} 
	for(int i=0;i<k;i++){
		cin>>d>>x>>y;
		//x吃y或者x、y比N大 
		if(x>n||y>n||(d==2&&x==y)){
			sum=sum+1;
			continue; 
		}
		if(unionroot(x,y,d)==false){
			sum=sum+1;
		} 
	}
	cout<<sum<<endl;
	return 0;
} 
