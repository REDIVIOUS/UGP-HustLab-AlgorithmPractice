#include <iostream>
#include <cstring>
#include <cstdio>
#define N 200005
using namespace std;

int Rank[N]; //排名数组，序号为i的排名
int sa[N]; //后缀数组，排名为i的序号
int Count[N]; //计数排序数组
int X[N],Y[N],*x,*y,*tmp; //倍增法所用到的数组(x为第一关键字，y为第二关键字)，以及他们的指针
char sinput[1005]; //接收输入的字符串
int Index[N]; //字符串序号
int s[N]; //拼接输入字符串
int height[N]; //记录ranki和ranki-1的最长前缀匹配
bool havedone[1005]; //遍历符合条件的最长匹配（0为没有遍历、1为遍历了）
int commlen; //公共长度
int commnum; //公共个数
int commnumtmp; //公共个数（temp）
int groupnum;
int n,q,m; //字串总长、串的数量、count数组的大小
int commstr[N]; //公共子串

bool check(int i,int j){ //倍增法首尾两点是否相等
    return y[sa[i-1]]==y[sa[i]]&&y[sa[i-1]+j]==y[sa[i]+j];
}

//倍增法求得sa数组
void suffix(int m){
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
    for (int j=1;j<=n;j<<=1)
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
void mkheight()
{
    for (int i=0;i<n;i++){
        Rank[sa[i]]=i;
    }
    height[0]=0; //初始化
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

//计算half的二分条件下，满足条件的字串个数和其在拼接串的位置
bool ifhalf(int half)
{
    //初始化
    int c=0,startpos=N;commnumtmp=0; //计数器（字串长度）、字串在拼接串的起始位置、符合条件子串个数
    memset(havedone,0,sizeof(havedone)); //初始化havedone数组（表示是否访问了某个字符串的某个点）
    for (int i=1;i<n;i++)
        //对于所有最长前缀匹配超过half的节点和前序节点进行遍历
        if (height[i]>=half)
        {
            if (!havedone[Index[sa[i]]]){
                havedone[Index[sa[i]]]=1;
                c++; //字串长度增加
            }
            startpos=min(startpos,sa[i]);
            if (!havedone[Index[sa[i-1]]]){
                havedone[Index[sa[i-1]]]=1;
                c++;
            }
            startpos=min(startpos,sa[i-1]);
        }
        else
        {
            if (c>q/2) commstr[++commnumtmp]=startpos; //如果当前字串长度大于其一般，添加一条输出字串
            startpos=N;
            c=0;
            memset(havedone,0,sizeof(havedone));
        }
    if (c>q/2){
        commstr[++commnumtmp]=startpos;
    }
    if (commnumtmp){ //如果可以输出的字串个数大于0，则输出真
        return 1;
    }
    return 0;
}

int main()
{
    while(scanf("%d",&q)&&q)
    {
        if (q==1){ //若只有一个串，就输出该串
            scanf("%s",sinput);
            printf("%s\n",sinput);
            continue;
        }
        n=-1; //拼接串中每一个字符的索引
        groupnum=0; //串号码
        //拼接字串成为一个大串，用特殊符号分隔
        for (int i=0;i<q;i++)
        {
            scanf("%s",sinput);
            int size_s=strlen(sinput);
            for (int j=0;j<size_s;j++){
                s[++n]=sinput[j];
                Index[n]=i;
            }
            s[++n]='$'+(groupnum++); //结束符，每一个串不一样
        }
        suffix(320); //输入count数组（ascii码个数）的大小，这里给出320
        mkheight(); //生成height数组
        //二分法寻找子串
        int left=1,right=n,commlen=0;
        while (left<=right)
        {
            int half=(left+right)>>1; //取当前两个断点的中点作为half
            //寻找当前y条件下的可输出字串数
            if (ifhalf(half)){
                commlen=half;
                commnum=commnumtmp;
                left=half+1;
            }
            else right=half-1;
        }
        //如果没有符合条件的，输出问号
        if (!commlen) printf("?\n\n");
        else
        {
            //遍历每一个记录的k满足条件的可输出串
            for (int i=1;i<=commnum;i++)
            {
                int end=commstr[i]+commlen;
                for (int j=commstr[i];j<end;j++){
                    printf("%c",s[j]);
                }
                printf("\n");
            }
            printf("\n");
        }
    }
    return 0;
}
