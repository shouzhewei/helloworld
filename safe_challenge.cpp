#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;

const int MAXN = 1002;
const int MAXM = MAXN*MAXN;
const int INF = 0x7FFFFFFF;

int head[MAXN]; // edge num for head[i]
int edge_cnt;   // total edge num
int nv;         // total node num

struct edge
{
    int v,next,cap,flow;
}e[MAXM];

int cmp(const void* p1, const void* p2)
{
    return *(int*)p1 - *(int*)p2;
}

void addedge(int u,int v,int cap)
{
    //printf("%d -> %d : %d\n", u, v, cap);
    //printf("%d -> %d : %d\n", v, u, 0);
    e[edge_cnt].v=v;
    e[edge_cnt].cap=cap;
    e[edge_cnt].flow=0;
    e[edge_cnt].next=head[u];
    head[u]=edge_cnt++;

    e[edge_cnt].v=u;
    e[edge_cnt].cap=0;
    e[edge_cnt].flow=0;
    e[edge_cnt].next=head[v];
    head[v]=edge_cnt++;
}

int gap[MAXN],dist[MAXN],pre[MAXN],curedge[MAXN];
int minicut(int s, int t)
{
    int cur_flow, u, tmp, neck, i, max_flow=0;

    memset(gap, 0, sizeof(gap));
    memset(pre, -1, sizeof(pre));
    memset(dist, 0, sizeof(dist));
    memset(curedge, 0, sizeof(curedge));

    for(i=1 ;i<=nv; i++)
    {
        curedge[i] = head[i];
    }
    gap[nv] = nv;
    u = s;

    while(dist[s] < nv)
    {
        if(u == t)
        {
            cur_flow = INF;
            for(i=s; i!=t; i=e[curedge[i]].v)
            {
                if(cur_flow > e[curedge[i]].cap)
                {
                    neck = i;
                    cur_flow = e[curedge[i]].cap;
                }
            }
            for(i=s; i!=t; i=e[curedge[i]].v)
            {
                tmp = curedge[i];
                e[tmp].cap -= cur_flow;
                e[tmp].flow += cur_flow;
                tmp ^= 1;
                e[tmp].cap += cur_flow;
                e[tmp].flow -= cur_flow;
            }
            max_flow += cur_flow;
            u = neck;
        }
        for(i = curedge[u]; i!=-1; i=e[i].next)
        {
            if(e[i].cap > 0 && dist[u] == dist[e[i].v]+1)
                break;
        }
        if(i != -1)
        {
            curedge[u] = i;
            pre[e[i].v] = u;
            u = e[i].v;
        }
        else
        {
            if(0 == --gap[dist[u]])
                break;
            curedge[u] = head[u];
            for(tmp = nv,i=head[u] ; i != -1; i = e[i].next)
            {
                if(e[i].cap > 0)
                    tmp = tmp < dist[e[i].v] ? tmp : dist[e[i].v];
            }
            dist[u] = tmp+1;
            ++gap[dist[u]];
            if(u != s)
                u = pre[u];
        }
    }
    return max_flow;
}

void dfs(int v, int* visited, int* features, int* feature_num)
{
    visited[v] = 1;
    features[(*feature_num)++] = v;

    for(int i = head[v]; i != 0; i = e[i].next)
    {
        int vs = e[i].v;
        if(visited[vs]==0 && e[i].cap)
            dfs(vs, visited, features, feature_num);
    }
}

int main(int args, char** argv)
{
    freopen(argv[1], "r", stdin);

    int n;
    char linebuf[500];

    while(scanf("%d", &n) != EOF)
    {
        int s = 0;   // super source node
        int t = n+1; // super target node
        int sum = 0; // sum of positive weight
        nv = n+2;    // total node number

        edge_cnt=0;
        memset(head,-1,sizeof(head));
        memset(e,0,sizeof(e));

        for(int i=1; i<=n; i++)
        {
            scanf("\n%[^\n]", &linebuf);
            int value = atoi(strtok(linebuf, " "));

            if(value > 0)
            {
                addedge(s, i, value);
                sum += value;
            }
            else
            {
                addedge(i, t, -value);
            }

            auto tok = strtok(nullptr, " ");
            while (tok != nullptr)
            {
                addedge(i, atoi(tok), INF);
                tok = strtok(nullptr, " ");
            }
        }

        int ret = minicut(s, t);
        int visited[MAXN];
        int features[MAXN];   // output features
        int feature_num = 0;  // output feature number

        memset(visited, 0, sizeof(visited));
        dfs(0, visited, features, &feature_num);

        // print out
        printf("%d\n", sum-ret);
        qsort(features, feature_num, sizeof(int), cmp);
        for(int i=1; i<feature_num; i++)
            printf("%d\n", features[i]);
    }

    fclose(stdin);
    return 0;
}

