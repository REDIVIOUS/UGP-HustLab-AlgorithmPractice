#include<iostream>
#define N 905
using namespace std;

int root[N]; //祖先节点z数组
int Count[N]; //统计每个祖先的次数
bool vis[N]; //是否被访问过
int n; //节点的数量（输入）
int m; //输入的点对的组数
int u,v,k; //输入的祖先节点、祖先节点对应的子节点、子节点数量
int a,b; //点对的两个节点

//接收输入函数，构建root数组
void Root(){
    memset(root,0,sizeof(root)); //root数组初始化
    for(int i=1;i<=n;i++){
        //输入祖先节点和祖先节点对应的子节点数
        scanf("%d:(%d)",&u,&k);
        for(int j=1;j<=k;j++){
            scanf("%d",&v);
            root[v] = u; //v节点的祖先设置为u
        }
    }
}

//求最近公共祖先
void LCA(){
    //输入点对组数
    scanf("%d",&m);
    memset(Count,0,sizeof(Count)); //初始化Count数组
    for(int i=1;i<=m;i++){
        //查询资料发现，这里过滤掉前后两个括号仅需要加上前后两个%*1s进行缓冲即可
        scanf("%*1s%d%d%*1s",&a,&b);
        memset(vis,false,sizeof(vis)); //初始化vis数组
        while(a) { //标记画关键祖先
            vis[a] = true;
            a=root[a];
        }
        while(vis[b] == false) { //逆序求最近祖先
            b=root[b];
        }
        Count[b]++;
    }
}

int main()
{
    while(~scanf("%d",&n)){
        Root();
        LCA();
        //遍历每个节点，如果有祖先节点有被访问的次数，则输出
        for(int i=1;i<=n;i++){
            if(Count[i]){
                printf("%d:%d\n",i,Count[i]);
            }
        }
    }
    return 0;
}
