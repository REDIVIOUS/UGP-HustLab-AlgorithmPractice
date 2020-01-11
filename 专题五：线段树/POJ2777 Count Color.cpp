#include <iostream>
#include <cstdio>
#include <cstring>
#define N 400010
#define left(i) (i<<1)
#define right(i) (i<<1|1)
using namespace std;

int tree[N]; //线段树节点的权值
int flag[N]; //延迟标志
int l,t,o; //区间数量、颜色数量、操作数量
char op; //操作类型
int ans; //当前查询节点的结果（权值）

//更新节点统计信息
void pushup(int i){
    tree[i]=tree[left(i)]|tree[right(i)];
}

//下推标记
void pushdown(int i){ //当前节点、i的左子树和右子树元素数量
    if(flag[i]){ //如果延迟标记置位，则下推标记
        flag[left(i)]=flag[i]; //下推至左子树
        flag[right(i)]=flag[i];//下推至右子树
        //修改子节点的Sum使之与对应的flag相对应
        tree[left(i)]=flag[i];
        tree[right(i)]=flag[i];
        flag[i]=0; //延迟位置0
    }
}


//建立线段树
void build(int i,int l,int r){
    if(l==r){ //到达根节点
        tree[i]=1; //初始化染色为1号
        return;
    }
    int m=(l+r)>>1;
    build(left(i),l,m); //递归建左子树
    build(right(i),m+1,r); //递归建右子树
    pushup(i); //更新自己的信息
}


//区间修改，将L到R的线段都加上c
void update(int i,int l,int r,int L,int R,int c){
    //节点编号，当前节点左右区间，操作区间、增加常量
    if(r<L||l>R){ //完全不在操作区间内
        return;
    }
    if(l<=L&&r>=R){ //如果[l, r]包含[L,R]
        tree[i]=1<<(c-1); //更新区间,标记染色号
        flag[i]=1<<(c-1); //增加延迟更新标记，表示本区间正确，但子区间仍需要根据flag的值来调
        return;
    }
    int m=(L+R)>>1;
    pushdown(i); //如果有延迟更新，这里下推标记
    update(left(i),l,r,L,m,c); //左子树递归修改
    update(right(i),l,r,m+1,R,c); //右子树递归修改
    pushup(i); //更新本节点信息
}

//区间查询，查询l到r有多少种颜色，保存在ans中
void query(int i,int l,int r,int L,int R){ //当前节点编号、询问操作的左右区间、当前节点左右区间
    if(r<L||l>R){ //完全不在操作区间内
        return;
    }
    if(l<=L&&r>=R){ //如果[l, r]包含[L,R]
        ans|=tree[i];
        return;
    }
    int m=(L+R)>>1;
    pushdown(i); //下推标记
    query(left(i),l,r,L,m); //左子树求和
    query(right(i),l,r,m+1,R); //在右子树求和
}

//计算x中有多少个1（多少种颜色）
int cal(int x){
    int total=0;
    while(x){
        if(x&1){
            total++;
        }
        x>>=1;
    }
    return total;
}


int main(){
    scanf("%d %d %d",&l,&t,&o);
    memset(flag,0,sizeof(flag));
    build(1,1,l);
    int i,a,b,c;
    for(i=1;i<=o;i++){
        getchar();
        scanf("%c",&op);
        if(op=='C'){
            scanf("%d%d%d",&a,&b,&c);
            if(a>b){
                swap(a,b);
            }
            update(1,a,b,1,l,c);
        }
        if(op=='P'){
            ans=0;
            scanf("%d%d",&a,&b);
            if(a>b){
                swap(a,b);
            }
            query(1,a,b,1,l);
            int total=cal(ans);
            printf("%d\n",total);
        }
    }
    return 0;
}
