#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>

const int MAXN = 22;

namespace {
struct data
{
    int cnt;
    int val[MAXN];
};

struct data map[MAXN];

inline bool cmp(const struct data a, const struct data b)
{
    return a.cnt < b.cnt;
}

void dfs(int unidx, int un, int* used, long long* ret)
{
    for(int i=0; i<map[unidx].cnt; i++)
    {
        if(used[map[unidx].val[i]] == 0)
        {
            used[map[unidx].val[i]] = 1;

            if(unidx == un-1)
            {
                (*ret)++;
            }
            else
            {
                dfs(unidx+1, un, used, ret);
            }

            used[map[unidx].val[i]] = 0;
        }
    }
}
}

int main(int, char** argv)
{
    freopen(argv[1], "r", stdin);

    int tn;
    int un;
    while(scanf("%d, %d", &tn, &un)!=EOF)
    {
        char line[202];
        memset(map, 0, sizeof(map));
        for(int i=0; i<un; i++)
        {
            scanf("\n%[^\n]", &line);

            char* p;
            char* tok = strtok_r(line, ",", &p);
            while (tok != nullptr)
            {
                map[i].val[map[i].cnt++] = atoi(tok);
                tok = strtok_r(nullptr, ",", &p);
            }
        }

        std::sort(map, map+un, cmp);

        long long ret=0;
        int used[MAXN];
        memset(used, 0, sizeof(used));
        dfs(0, un, used, &ret);

        fprintf(stdout, "%lld\n", ret);
    }

    fclose(stdin);
    return 0;
}

