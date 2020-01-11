#include <iostream>
#include <algorithm>
#include <queue>
#define INF 999999999
#define maxn 30000
#define maxm 150010
using namespace std;

int A,B,C; //孩子A认为孩子B不会比他多处C块糖
int n,m; //n个孩子，m次条件
int node; //松弛操作的当下节点
int NeiNode[maxn]; //邻居节点
int dis[maxn]; //从某个节点（程序制定）到i号节点的最短距离
bool vis[maxn]; //bool数组，表示该节点是否被访问过
//为了构造B-A<=C创造的结构体
//结构体元素表示松弛节点，松弛距离，下一个传播节点
struct edge{
    int curr,dis,next;
}loose[maxm]; //松弛操作数组
//构造松弛节点，以便求最短路
struct Qnode
{
    int curr,dis; //当前松弛节点以及松弛距离
    Qnode(int _curr, int _dis): curr(_curr), dis(_dis){}
    bool operator < (const Qnode& node) const //重载，距离大小对节点排序
    {
        return dis > node.dis;
    }
};

void dij(int s)
{
    int first,last,curr; //上一次节点、下一次节点、当前节点
    //初始化dis数组和vis数组
    //dis开始设为无穷大，vis开始设置为没有访问
    for (int i=0;i<n;++i)
    {
        dis[i]=INF;
        vis[i]=false;
    }
    //将s（源节点）的dis设置为0，可访问性vis设置为true
    dis[s]=0;
    vis[s]=true;
    //将松弛节点建立一个优先队列进行操作
    priority_queue<Qnode> loose_q;
    loose_q.push(Qnode(s, 0));
    //首先将上一次节点设置为源节点s
    first=s;
    //遍历整个dis数组，更新其中的值
    for (int i = 1; i < n; ++i)
    {
        last=NeiNode[first]; //下一个节点设置为上一个节点的邻居节点
        while (last!=-1)
        {
            curr=loose[last].curr; //当前节点先赋值为上一次的当前节点
            if (!vis[curr]&&dis[first]+loose[last].dis<dis[curr]) //松弛操作 B<=A+C
            {
                //加上松弛的量
                dis[curr] = dis[first] + loose[last].dis;
                loose_q.push(Qnode(curr,dis[curr]));
            }
            last=loose[last].next; //后节点为松弛的下一个节点
        }
        while (!loose_q.empty() && vis[loose_q.top().curr]){
            loose_q.pop();
        }
        //更新前节点
        first=loose_q.top().curr;
        //设置已经访问
        vis[first] = true;
        loose_q.pop();
    }
}

int main()
{
    //输入孩子的个数n和操作数m
    scanf("%d%d",&n,&m);
    //当前操作节点从0开始
    node=0;
    //初始化操作节点的邻居节点为-1（即无穷）
    for (int i=0;i<n;++i) {
        NeiNode[i] = -1;
    }
    //读入边的信息
    for (int i=0;i<m;++i)
    {
        scanf("%d%d%d",&A,&B,&C);
        //A和B从0开始编号，故先都要减去1
        --A;
        --B;
        //构造B-A<=C
        loose[node].curr=B;  //松弛对象B
        loose[node].dis=C;  //松弛距离为C
        loose[node].next=NeiNode[A]; //下一个节点为A的邻居节点
        NeiNode[A]=node; //转A的邻居节点作为现在节点
        ++node; //处理下一个
    }
    //求最短路
    dij(0);
    printf("%d\n", dis[n-1]); //求从1到n的最短路
    return 0;
}
