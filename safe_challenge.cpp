#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;

const int MAXN = 1002;
const int MAXM = MAXN*MAXN;
const int INF = 0x7FFFFFFF;

struct Node
{
    int from;
    int to;
    int next;
    int cap;
};

int tol;
int dep[MAXN];
int head[MAXN];
struct Node edge[MAXM];

void init()
{
    tol=0;
    memset(head, -1, sizeof(head));
}

inline int cmp(const void* a, const void* b)
{
    return *(int*)a - *(int*)b;
}

inline void printout(int val)
{
    printf("%d\n", val);
}

inline void addedge(int u,int v,int w)
{
    if(u>=0 && u<MAXN && v>=0 && v<MAXN)
    {
        edge[tol].from=u;
        edge[tol].to=v;
        edge[tol].cap=w;
        edge[tol].next=head[u];
        head[u]=tol++;
        edge[tol].from=v;
        edge[tol].to=u;
        edge[tol].cap=0;
        edge[tol].next=head[v];
        head[v]=tol++;
    }
}

int BFS(int start,int end)
{
    int que[MAXN];
    int front;
    int rear;
    front=rear=0;
    memset(dep,-1,sizeof(dep));
    que[rear++]=start;
    dep[start]=0;

    while(front!=rear)
    {
        int u=que[front++];
        if(front==MAXN)front=0;
        for(int i=head[u];i!=-1;i=edge[i].next)
        {
            int v=edge[i].to;
            if(edge[i].cap>0&&dep[v]==-1)
            {
                dep[v]=dep[u]+1;
                que[rear++]=v;
                if(rear>=MAXN)rear=0;
                if(v==end)return 1;
            }
        }
    }
    return 0;
}

void update_res(int& res, int& u, int& top, int stack[])
{
    int min=INF;
    int loc=0;
    for(int i=0;i<top;i++)
        if(min>edge[stack[i]].cap)
        {
            min=edge[stack[i]].cap;
            loc=i;
        }
    for(int i=0;i<top;i++)
    {
        edge[stack[i]].cap-=min;
        edge[stack[i]^1].cap+=min;
    }
    res+=min;
    top=loc;
    u=edge[stack[top]].from;
}

int minicut(int start,int end)
{
    int res=0;
    int top;
    int stack[MAXN] = {0};
    int cur[MAXN];

    while(BFS(start,end))
    {
        memcpy(cur,head,sizeof(head));
        int u=start;
        top=0;

        while(1)
        {
            if(u==end)
            {
                update_res(res, u, top, stack);
            }
            for(int i=cur[u];i!=-1;cur[u]=i=edge[i].next)
              if(edge[i].cap!=0&&dep[u]+1==dep[edge[i].to])
                 break;
            if(cur[u]!=-1)
            {
                stack[top++]=cur[u];
                u=edge[cur[u]].to;
            }
            else
            {
                if(top==0) 
                    break;
                dep[u]=-1;
                u=edge[stack[--top]].from;
            }
        }
    }
    return res;
}

void dfs(int v, int* visited, int* features, int* feature_num)
{
    visited[v] = 1;
    features[(*feature_num)++] = v;

    for(int i=head[v]; i>0; i=edge[i].next)
    {
        int vs = edge[i].to;
        if(visited[vs]==0 && edge[i].cap > 0)
            dfs(vs, visited, features, feature_num);
    }
}

int main(int, char** argv)
{
    freopen(argv[1], "r", stdin);

    int n;
    char linebuf[500];

    while(scanf("%d", &n) != EOF)
    {
        int s = 0;   // super source node
        int t = n+1; // super target node
        int sum = 0; // sum of positive weight

		init();

        for(int i=1; i<=n; i++)
        {
            char* pstr;

            scanf("\n%[^\n]", linebuf);
            int value = atoi(strtok_r(linebuf, " ", &pstr));

            if(value > 0)
            {
                addedge(s, i, value);
                sum += value;
            }
            if(value < 0)
            {
                addedge(i, t, -value);
            }

            char* tok = strtok_r(nullptr, " ", &pstr);
            while (tok != nullptr)
            {
                addedge(i, atoi(tok), INF);
                tok = strtok_r(nullptr, " ", &pstr);
            }
        }

        int ret = minicut(s, t);
        int visited[MAXN];
        int features[MAXN];   // output features
        int feature_num = 0;  // output feature number

        memset(visited, 0, sizeof(visited));
        dfs(0, visited, features, &feature_num);

        printout(sum-ret);
        qsort(features, feature_num, sizeof(int), cmp);
        for(int i=1; i<feature_num; i++)
            printout(features[i]);
    }

    fclose(stdin);
    return 0;
}

