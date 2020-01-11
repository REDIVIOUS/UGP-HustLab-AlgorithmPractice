#include <iostream>
#include <algorithm>
#define M 300010 //这个题目一定注意大小开4倍，否则runtime error
#define ll long long
using namespace std;

int N; //操作次数
int A,B,H; //i号建筑的参数：[A,B] with H
ll Area; //最终面积
ll hash_a[M]; //离散化数组

//线段树结构体，存放每一个线段的左、右、中间节点以及线段高度
struct tree {
    int l,r,m,h;
} tree[M];

//存储每一个建筑的起始点、终点、高度
struct build {
    int a,b,h;
} building[M];

//将建筑离散化成一个一个的节点
struct axis {
    int x,i;
    //重载"<"符号
    bool operator < (const struct axis & other)const {
        return x < other.x;
    }
} x_axis[M];

//建立线段树
void build(int l, int r, int i) {
    //给线段树的左右节点和中间节点赋值
    tree[i].l = l;
    tree[i].r = r;
    tree[i].m = (l+r)/2;
    tree[i].h = 0;
    //到达跟节点之前，左右递归建树
    if(l+1!= r) {
        build(l,tree[i].m,i<<1); //左边递归建树
        build(tree[i].m,r,(i<<1)+1); //右递归建树
    }
}

//更新函数
void update(int l, int r, int h, int i) {
    if(tree[i].l==l&&tree[i].r==r ) { //当跟新区间就是当前区间的时候
        if(tree[i].h<h||tree[i].h == 0){ //当高度小于更新高度或者原本高度为0
            tree[i].h = h; //更新高度
        }
    } else if(tree[i].l<tree[i].r) { //当前节点左节点值小于右节点值（即没有越界的时候）
        //更新左边的情况
        if(r<=tree[i].m){
            update(l,r,h,i<<1);
        }
        //更新右边的情况
        else if(l >= tree[i].m){
            update(l,r,h,(i<<1)+1);
        }
        //否则左右递归更新
        else {
            update(l,tree[i].m,h,i<<1);
            update(tree[i].m,r,h,(i<<1)+1);
        }
    }
}

//计算总面积
ll cal(int h, int i) {
    //取大的h
    if(h>tree[i].h)
        tree[i].h=h;
    //递归终点,计算矩形面积
    if(tree[i].l+1==tree[i].r) {
        ll rightnode=tree[i].r;
        ll leftnode=tree[i].l;
        ll height=tree[i].h;
        ll len=hash_a[rightnode]-hash_a[leftnode];
        ll area=height*len;
        return area;
    }
    //否则在左右子树计算相加
    else{
        ll area=cal(tree[i].h,i<<1) + cal(tree[i].h,(i<<1)+1);
        return area;
    }
}

int main() {
    int i;
    scanf("%d",&N);
    for(i=0; i<N; i++) {
        scanf("%d%d%d",&A,&B,&H); //输入当前的建筑的参数
        if(A>B) { //如果A的值大于B的值
            swap(A,B);
        }
        //存放当前线段信息在building数组中
        building[i].a = A;
        building[i].b = B;
        building[i].h = H;
    
        //将输入离散化成一个一个节点
        //左边节点
        x_axis[i<<1].x = building[i].a;
        x_axis[i<<1].i = -(i+1);
        //右边节点
        x_axis[(i<<1)+1].x = building[i].b;
        x_axis[(i<<1)+1].i = i+1;
        }
    
        //离散化节点排序
        sort(x_axis,x_axis+(N<<1));
        int j=1;
        int now=x_axis[0].x;
        for(i=0; i<2*N; i++) {
            if(x_axis[i].x!=now) {
                j++;
                now=x_axis[i].x;
            }
            if(x_axis[i].i < 0) {
                hash_a[j] = building[-x_axis[i].i -1].a;
                building[-x_axis[i].i -1].a=j;
            } else {
                hash_a[j] = building[x_axis[i].i -1].b;
                building[x_axis[i].i -1].b=j;
            }
        }
        //建立线段树
        build(1,j,1);
        //用输入信息更新线段树
        for(i=0; i<N; i++) {
            update(building[i].a, building[i].b, building[i].h, 1);
        }
        //调用cal函数计算总面积
        Area= cal(0,1);
        printf("%lld\n",Area);
    return 0;
}
