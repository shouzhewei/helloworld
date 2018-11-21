#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

const int MAXN = 22;

namespace {
void dfs(int unidx, int un, int map[][MAXN], int* used, int* ret)
{
    for(int i=1; i<=map[unidx][0]; i++)
    {
        if(used[map[unidx][i]] == 0 )
        {
            used[map[unidx][i]] = 1;

            if(unidx == un-1)
            {
                (*ret)++;
            }
            else
            {
                dfs(unidx+1, un, map, used, ret);
            }

            used[map[unidx][i]] = 0;
        }
    }
}
}

int main(int, char** argv)
{
    freopen(argv[1], "r", stdin);

    int tn;
    int un;
    int map[MAXN][MAXN];
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
                map[i][++map[i][0]] = atoi(tok);
                tok = strtok_r(nullptr, ",", &p);
            }
        }

        int ret=0;
        int used[MAXN];
        memset(used, 0, sizeof(used));
        dfs(0, un, map, used, &ret);

        fprintf(stdout, "%d\n", ret);
    }

    fclose(stdin);
    return 0;
}

