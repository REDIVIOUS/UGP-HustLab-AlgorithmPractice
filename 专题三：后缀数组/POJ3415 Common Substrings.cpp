#include <iostream>
#include <cstring>
#include <cstdio>
#define N 200005
using namespace std;

int Rank[N]; //排名数组，序号为i的排名
int sa[N]; //后缀数组，排名为i的序号
int Count[N]; //计数排序数组
int X[N],Y[N],*x,*y,*tmp; //倍增法所用到的数组(x为第一关键字，y为第二关键字)，以及他们的指针
int height[N]; //记录ranki和ranki-1的最长前缀匹配
int k; //组内串数量
long long sum_t,top; //当前已有的匹配数
char str1[N],str2[N]; //输入的两个子串
int cur_height[N],cur_cnt[N];

bool check(int i,int j){ //倍增法首尾两点是否相等
    return y[sa[i-1]]==y[sa[i]]&&y[sa[i-1]+j]==y[sa[i]+j];
}

//倍增法求得sa数组
void suffix(const char *s,int n,int m){
    x=X;
    y=Y;
    //初始化count和sa数组
    for (int i=0;i<m;i++){
        Count[i]=0;
    }
    for (int i=0;i<n;i++){
        Count[x[i]=s[i]]++;
    }
    for (int i=1;i<m;i++){
        Count[i]+=Count[i-1];
    }
    for (int i=n-1;i>=0;i--){
        sa[--Count[x[i]]]=i;
    }
    for (int j=1;j<n;j<<=1)
    {
        int w=0;
        for (int i=n-j;i<n;i++){
            y[w++]=i;
        }
        for (int i=0;i<n;i++){
            if (sa[i]>=j){
                y[w++]=sa[i]-j;
            }
        }

        //更新count数组和sa数组
        for (int i=0;i<m;i++){
            Count[i]=0;
        }
        for (int i=0;i<n;i++){
            Count[x[y[i]]]++;
        }
        for (int i=1;i<m;i++){
            Count[i]+=Count[i-1];
        }
        for (int i=n-1;i>=0;i--){
            sa[--Count[x[y[i]]]]=y[i];
        }

        //把第一关键在保存在Y数组里面
        tmp=x;
        x=y;
        y=tmp;

        //关键字重排
        w=1;
        x[sa[0]]=0;
        for (int i=1;i<n;i++){
            if(check(i,j)){
                x[sa[i]]=w-1;
            }
            else{
                x[sa[i]]=w;
                w++;
            }
        }
        if (w>n) break;
        m=w;
    }
}

//获取height数组
void mkheight(const char *s,int n)
{
    for (int i=1;i<=n;i++){
        Rank[sa[i]]=i;
    }
    height[1]=0; //初始化
    int j=0;
    for (int i=0;i<n;i++)
    {
        if (Rank[i]==0){
            continue;
        }
        if (j){
            j--;
        }
        int t=sa[Rank[i]-1];
        while (i+j<n&&t+j<n&&s[i+j]==s[t+j]){//不越界且当前点相等，前缀长度增加
            j++;
        }
        height[Rank[i]]=j;
    }
}

int main()
{
    while(scanf("%d",&k)&&k){
        scanf("%s%s",str1,str2);
        int len1,len2;
        len1=strlen(str1);
        len2=strlen(str2);
        //将str2拼接到str1上面，中间用$分割
        str1[len1]='$';
        for(int i=len1+1;i<=len1+len2;i++){
            str1[i]=str2[i-len1-1];
        }
        int n=len1+len2+1;
        str1[n]='\0';
        suffix(str1,n+1,300);
        mkheight(str1,n);
        sum_t=top=0;
        long long sum=0;
        for(int i=1;i<=n;i++){
            if(height[i]<k){ //当最长前缀小于k
                sum_t=top=0;
            }
            else{
                int count=0;
                //对B的后缀，找其rank之前的A的后缀进行匹配
                if(sa[i-1]<len1){ //A中的匹配
                    count++;
                    sum_t=sum_t+height[i]-k+1;
                }
                //维护单调栈，保证栈内height递增
                while(height[i]<=cur_height[top-1]&&top>0){
                    top--;
                    sum_t=sum_t-cur_cnt[top]*(cur_height[top]-height[i]);
                    count=count+cur_cnt[top];
                }
                cur_height[top]=height[i];
                cur_cnt[top++]=count;
                if(sa[i]>len1){ //不是A中匹配，sum加上当前已有的匹配数
                    sum=sum+sum_t;
                }
            }
        }
        sum_t=top=0;
        for(int i=1;i<=n;i++){
            if(height[i]<k){ //当最长前缀小于k
                sum_t=top=0;
            }
            else{
                int count=0;
                //对A的后缀，找其rank之前的B的后缀进行匹配
                if(sa[i-1]>len1){ //A中的匹配
                    count++;
                    sum_t=sum_t+height[i]-k+1;
                }
                //维护单调栈，保证栈内height递增
                while(height[i]<=cur_height[top-1]&&top>0){
                    top--;
                    sum_t=sum_t-cur_cnt[top]*(cur_height[top]-height[i]);
                    count=count+cur_cnt[top];
                }
                cur_height[top]=height[i];
                cur_cnt[top++]=count;
                if(sa[i]<len1){ //不是A中匹配，sum加上当前已有的匹配数
                    sum=sum+sum_t;
                }
            }
        }
        //这里一定要记得用lld，用ld一直wrong
        printf("%lld\n",sum);
    }
    return 0;
}

