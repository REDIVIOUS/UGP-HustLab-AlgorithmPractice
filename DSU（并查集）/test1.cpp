#include<cstdio>
 
const int N = 50005;
int n, m, f[N], rank[N];
 
inline void init(){
    for(int i=0; i<=n; ++i)
        f[i]=i,rank[i]=0;
}
 
int find(int x){
    if(x==f[x])return x;
    int fa=f[x];
    f[x]=find(f[x]);
    rank[x] = (rank[x]+rank[fa])%3;
    return f[x];
}
 
inline bool Union(int x,int y,int D){
    int a=find(x), b=find(y);
    if(a==b){
        if(D==1&&rank[x]!=rank[y]) return false;
        if(D==2){
            if(rank[x]==2&&rank[y]!=1)return false;
            if(rank[x]==1&&rank[y]!=0)return false;
            if(rank[x]==0&&rank[y]!=2)return false;
        }
        return true;
    }
    f[a] = b;
    if(D==2){
        rank[a] = (rank[y]-rank[x]+3+1)%3; //注意加上3，因为可能是负数
    }
    else{
        rank[a] = (rank[y]-rank[x]+3)%3;   // 注意加上3，因为可能是负数
    }
    return true;
}
 
int main(){
    int x,y,d;
    scanf("%d%d",&n,&m);
    init();
    int cnt=0;
    for(int i=0; i<m; ++i){
        scanf("%d%d%d",&d,&x,&y);
        if(x>n || y>n || d==2&&x==y){
            ++cnt; continue;
        }
        if(!Union(x,y,d)) {
            ++cnt;
        }
    }
    printf("%d\n", cnt);
    return 0;
}
