#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;

const int MAXN = 22;

void dfs(int un_idx, int un, int map[][MAXN], int* used, int* ret)
{
    for(int i=1; i<=map[un_idx][0]; i++)
    {
        if(used[map[un_idx][i]])
            continue;
        else
        {
            used[map[un_idx][i]] = 1;
        }

        if(un_idx == un-1)
        {
            (*ret)++;
        }
        else
        {
            dfs(un_idx+1, un, map, used, ret);
        }

        used[map[un_idx][i]] = 0;
    }
}

int main(int args, char** argv)
{
    freopen(argv[1], "r", stdin);
    //freopen("output", "w", stdout);

    int tn, un;
    int map[MAXN][MAXN];
    while(scanf("%d, %d", &tn, &un)!=EOF)
    {
        //printf("%d %d\n", tn, un);
        char line[200];
        memset(map, 0, sizeof(map));
        for(int i=0; i<un; i++)
        {
            scanf("\n%[^\n]", &line);
            //getchar();
            auto tok = strtok(line, ",");
            while (tok != nullptr)
            {
                map[i][++map[i][0]] = atoi(tok);
                tok = strtok(nullptr, ",");
            }
            //char ch;
            //while(1)
            //{
            //    scanf("%d", &map[i][++map[i][0]]);
            //    ch = getchar();
            //    if(ch == '\n')
            //        break;
            //}
        }
        /*
        for(int i=0; i<un; i++)
        {
            for(int j=0; j<=map[i][0]; j++)
                printf("%d ", map[i][j]);
            puts("");
        }
        */

        int ret=0;
        int used[MAXN];
        memset(used, 0, sizeof(used));
        dfs(0, un, map, used, &ret);

        printf("%d\n", ret);
    }

    fclose(stdin);
    //fclose(stdout);
    return 0;
}

