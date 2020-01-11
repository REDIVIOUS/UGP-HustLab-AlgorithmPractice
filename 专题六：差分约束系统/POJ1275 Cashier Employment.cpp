#include<iostream>
#include<queue>
#define N 100
using namespace std;

int R[N]; //接受输入（每个时间段需要的人数）
int time_sub[N][N]; //time_sub[i][j]：从j时间到i时间的人数
int start[N]; //从时间i（i从0到24）开始上班的人数
int dis[N]; //源点到标号i的最短路径长度
int visited_time[N]; //一个节点被更新过的次数
bool ifvis[N][N]; //ifvis[i][j]表示i到j是否有路
bool loose_point[N]; //松弛点判断数组
queue<int> Q; //节点队列


//运用SPFA判断是否存在负环
bool SPFA(int x){
    //以24号节点（最后一个小时作为源点）
    dis[24]=0;
    visited_time[24]=1;
    //满足查分约束约束条件
    //条件1:当i大于等于8的时候，它减去它前八小时的人数应该大于R[i]
    for(int i=8;i<=24;i++){
        time_sub[i][i-8]=-R[i];
    }
    //条件2:当i小于8的时候，为条件1反过来m，即为它+16人数剪去它应该小于x-R[i]
    for(int i=1;i<=7;i++) {
        time_sub[i][i+16]=x-R[i];
    }
    //读入每个时间段开始上班的应聘者数量
    for(int i=1;i<=24;i++) {
        time_sub[i-1][i]=start[i];
    }
    //条件3：24小时内雇佣的人数应该大于等于x个人
    time_sub[24][0]=-x;
    
    //源节点进入队列
    Q.push(24);
    while(!Q.empty()){
        int curr=Q.front(); //当前松弛点
        loose_point[curr]=0;
        Q.pop();
        for(int i=0;i<=24;i++){
            if(ifvis[curr][i]&&dis[i]>dis[curr]+time_sub[curr][i]){
                //更新最短距离
                dis[i]=dis[curr]+time_sub[curr][i];
                visited_time[i]++;
                //更换当前松弛点，进入队列
                if(!loose_point[i]){
                    loose_point[i]=1;
                    Q.push(i);
                }
                //如果一个节点的更新次数超过了25次，证明有负环存在，返回false
                if(visited_time[i]>25) {
                    return false;
                }
            }
        }
    }
    return (dis[0]==-x);
}

//isvis数组初始化
void init(){
    //对应上述的几个查分约束条件，将ifvis函数的这些值的初值都置为1
    for(int i=8;i<=24;i++){
        ifvis[i][i-8]=1;
    }
    for(int i=1;i<=7;i++){
        ifvis[i][i+16]=1;
    }
    for(int i=1;i<=24;i++){
       ifvis[i][i-1]=ifvis[i-1][i]=1;
    }
    ifvis[24][0]=1;
}

int main()
{
    int Tsize; //测试的样例数量
    int start_time; //每一个应聘者开始的时间
    int n; //应聘者的数目
    
    //初始化
    init();
    
    //读入测试的次数并循环
    scanf("%d",&Tsize);
    for(int j=1;j<=Tsize;j++){
        //读入每个时间段所需要的收银员的数目
        for(int i=1;i<=24;i++) {
            scanf("%d",&R[i]);
        }
        
        //读入应聘者的数目
        scanf("%d",&n);
        memset(start,0,sizeof(start));
        
        //读入每个应聘者的开始时间（这里是从1到24）
        //结果存在W数组中，数组的值代表该时间开始的人的数目
        for(int i=1;i<=n;i++){
            scanf("%d",&start_time);
            start[start_time+1]++;
        }
        
        //下面进入二分查找阶段
        //l、r和mid表示二分的左、右和中间节点
        int l=1,r=n,mid=(l+r)>>1;
        bool flag=false; //flag表示是否有结果
        while(l<=r){ //二分查找
            mid=(l+r)>>1;
            //调用SPFA函数之前。必要的数组初始化
            memset(dis,9999999,sizeof(dis)); //dis数组初始化为无穷
            memset(visited_time,0,sizeof(visited_time));
            memset(time_sub,0,sizeof(time_sub));
            memset(loose_point,0,sizeof(loose_point));
            if(SPFA(mid)){
                //在左边继续查找
                flag=true;
                r=mid-1;
            }
            else {
                //在右边继续查找
                l=mid+1;
            }
        }
        
        //根据flag的值决定是否存在最小方案
        if(!flag) {
            printf("No Solution\n");
        }
        else {
            printf("%d\n",l);
        }
    }
}
