#include <iostream>
#include <utility>
#include <algorithm>
#define lowbit(x) ((x)&(-x)) 
#define N 20005
using namespace std;

//先对音量进行排序，然后音量从小到大处理，保证每次取的音量是最大值
//处理每一个音量的时候，分为坐标值比其小的和比其大的，取其绝对值之和 

pair<int,int> x_axis[N]; //存储每个数组对应的音量
int c[2][N]; //存储树状数组和，2是要存储两种状态（取距离和0，牛数量1） 

bool compare(pair<int,int> a,pair<int,int> b){
	return a.second<b.second;
}

//树状数组的getsum函数：返回前x个整数的和
int getsum(int x,int flag){
	int sum=0;
	for(int i=x;i>0;i=i-lowbit(i)){
		sum=sum+c[flag][i]; //根据c[i]，累计求出和 
	}
	return sum; 
} 

//给第x个整数加上v，更新c[i] 
void update(int x,int add, int flag){
	for(int i=x;i<=N;i=i+lowbit(i)){
		c[flag][i]=c[flag][i]+add; //更新和x号整数有关的所有c[i] 
	}
}

int main(){
	int n;
	cin>>n;
	int x,v,sum=0;
	for(int i=1;i<=n;i++){ //读取 
		cin>>v>>x;
		x_axis[i].first=x;
		x_axis[i].second=v;
	}
	sort(x_axis+1,x_axis+n+1,compare); // 按照音量排序
	int left,right,total=0; //分别记录当前距离在左边和右边的距离之和 
	for(int i=1;i<=n;i++){
		int cx=x_axis[i].first; //当前坐标 
		int cv=x_axis[i].second; //当前 
		total=total+cx; //当前距离之和
		update(cx,1,1); //当前牛计数加一 
		update(cx,cx,0); //当前距离和 
		int leftnum=getsum(cx,1); //左边数量 
		int leftsum=getsum(cx,0); //左边距离和
		left=leftnum*cx-leftsum; //左边距离差 
		right=total-leftsum-(i-leftnum)*cx; //右边距离差
		sum=sum+(left+right)*cv;
	}
	cout<<sum<<endl;
	return 0;
}
