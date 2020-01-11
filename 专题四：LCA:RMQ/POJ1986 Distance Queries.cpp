#include <iostream>
#include <vector>
#define N 100010
#define M 200010
using namespace std;

int root[N]; //并查集，每个节点的父节点
int result[N]; //存储最后的查询结果
int depth[N]; //存储节点的深度
bool vis[N]; //是否可访问
int Next[N]; //下一个要处理的节点
int next_index; //下一个节点的编号
vector<pair<int,int> > List[N]; //邻接链表，第一个元素是连接的点的编号，第二个元素是查询编号
int m,n; //节点个数和边的个数
int u,v,c; //两个节点以及节点之间的距离
int times; //h查询次数
int a,b; //两个查询节点
char loc[2]; //读入方向
//读入存储边的信息
struct tree_edge{
    int v, c, next; //（和u相连的）节点v的值、距离、下一个处理节点
}edge[M];

//查询祖先节点(并查集find操作)
int findroot(int x){
    if(x==root[x]){
        return x;
    }
    else{
        root[x]=findroot(root[x]);
        return root[x];
    }
}

//查询最邻近祖先操作
void LCA(int Root,int u,int dep){
    depth[u] = dep; //记录节点的深度
    for (int i=Next[u];i!=-1;i=edge[i].next){
        int v = edge[i].v, c = edge[i].c;
        //如果是根节点，结束递归
        if (v == Root){
            continue;
        }
        LCA(u,v,dep+c); //递归执行
        //合并并查集
        root[v] = u;
    }
    //标记为可访问
    vis[u] = true;
    //遍历整个图，找出查询的点和邻接的点
    for (int i = 0; i < List[u].size(); i++){
        //v是邻接点，index是查询编号
        int v=List[u][i].first,index=List[u][i].second;
        //如果可以到达
        if (vis[v]){
            //计算结果
            result[index]=depth[u]+depth[v]-2*depth[findroot(v)];
        }
    }
}


int main(){
    //读入节点个数和边的个数
    scanf("%d%d", &n, &m);

    //数据初始化
    next_index = 0;
    memset(Next, -1, sizeof(Next));
    memset(vis, false, sizeof(vis));
    memset(depth, 0, sizeof(depth));
    for (int i = 1; i <= n; i++){ //每个节点的跟节点先初始化为自己
        root[i] = i;
    }

    //读取两个节点、节点值以及方向
    for (int i = 1; i <= m; i++){
        scanf("%d%d%d%s",&u,&v,&c,loc);
        //加入u到v的边和v到u的边
        edge[next_index].v = v;
        edge[next_index].c = c;
        edge[next_index].next = Next[u];
        Next[u] = next_index++;
        //加入v到u的边
        edge[next_index].v = u;
        edge[next_index].c = c;
        edge[next_index].next = Next[v];
        Next[v] = next_index++;
    }

    //查询过程
    scanf("%d", &times); //查询次数
    for (int i = 1; i <= times; i++){
        scanf("%d%d", &a, &b);
        //在图中存入连接节点的
        List[a].push_back(make_pair(b, i));
        List[b].push_back(make_pair(a, i));
    }

    //查询最近公共祖先result
    LCA(-1, 1, 0);

    //输出查询结果
    for (int i = 1; i <= times; i++){
        printf("%d\n", result[i]);
    }
    return 0;
}

