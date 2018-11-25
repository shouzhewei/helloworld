#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>

const int MAXN = 1005;
const int MAXM = MAXN*MAXN*2 + MAXN*2;
const int INF = 0x7FFFFFFF;

namespace {
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

inline void printout(int val)
{
    fprintf(stdout, "%d\n", val);
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

int updategraph(int& rear, int& u, int& end, int(&que)[MAXN])
{
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

int bfs(int start,int end)
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

        if(updategraph(rear, u, end, que) == 1)
            return 1;
    }
    return 0;
}

void updateres(int& res, int& u, int& top, int(&stack)[MAXN])
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

bool updatepath(int start, int end, int& u, int& res, int& top, int(&stack)[MAXN], int(&cur)[MAXN])
{
    if(u==end)
    {
        updateres(res, u, top, stack);
    }
    for(int i=cur[u];i!=-1;cur[u]=i=edge[i].next)
    {
        if(edge[i].cap!=0&&dep[u]+1==dep[edge[i].to])
            break;
    }
    if(cur[u]!=-1)
    {
        stack[top++]=cur[u];
        u=edge[cur[u]].to;
    }
    else
    {
        if(top==0)
            return false;
        dep[u]=-1;
        u=edge[stack[--top]].from;
    }
    return true;
}

int minicut(int start,int end)
{
    int res=0;
    int top;
    int stack[MAXN] = {0};
    int cur[MAXN];

    while(bfs(start,end))
    {
        memcpy(cur,head,sizeof(head));
        int u=start;
        top=0;

        while(1)
        {
            if(updatepath(start, end, u, res, top, stack, cur) == false)
                break;
        }
    }
    return res;
}

void dfs(int v, int* visited, int* features, int* featurenum)
{
    visited[v] = 1;
    features[(*featurenum)++] = v;

    for(int i=head[v]; i>=0; i=edge[i].next)
    {
        int vs = edge[i].to;
        if(visited[vs]==0 && edge[i].cap > 0)
            dfs(vs, visited, features, featurenum);
    }
}

void runaddedge(int s, int t, int cur, int& sum, char* linebuf)
{
    char* pstr;
    int value = atoi(strtok_r(linebuf, " ", &pstr));

    if(value > 0)
    {
        addedge(s, cur, value);
        sum += value;
    }
    if(value < 0)
    {
        addedge(cur, t, -value);
    }

    if(value != 0)
    {
        char* tok = strtok_r(nullptr, " ", &pstr);
        while (tok != nullptr)
        {
            addedge(cur, atoi(tok), INF);
            tok = strtok_r(nullptr, " ", &pstr);
        }
    }
}
}

int main(int, char** argv)
{
    freopen(argv[1], "r", stdin);

    int n;
    char linebuf[10000];

    while(scanf("%d", &n) != EOF)
    {
        int s = 0;
        int t = n+1;
        int sum = 0;

        init();

        for(int i=1; i<=n; i++)
        {
            scanf("\n%[^\n]", linebuf);
            runaddedge(s, t, i, sum, linebuf);
        }

        int ret = minicut(s, t);
        int visited[MAXN];
        int features[MAXN];
        int featurenum = 0;

        memset(visited, 0, sizeof(visited));
        dfs(0, visited, features, &featurenum);

        if(sum-ret > 0)
        {
            printout(sum-ret);
            std::sort(features, features+featurenum);
            for(int i=1; i<featurenum; i++)
                printout(features[i]);
        }
        else
        {
            printout(0);
        }
    }

    fclose(stdin);
    return 0;
}

