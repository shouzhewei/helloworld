#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;

const int MAXN = 105;

int main(int args, char** argv)
{
    freopen(argv[1], "r", stdin);

    int n, m;
    int map[MAXN][MAXN];
    while(scanf("%d, %d", &m, &n)!=EOF)
    {
        memset(map, 0, sizeof(map));

        for(int i=1; i<=n; i++)
        {
            int j, val;
            for(j=1; j<m; j++)
            {
                scanf("%d,", &val);
                map[i][j] = max(map[i-1][j] + val, map[i][j-1] + val);
            }
            scanf("%d", &val);
            map[i][j] = max(map[i-1][j] + val, map[i][j-1] + val);
        }

        printf("%d\n", map[n][m]);
    }

    fclose(stdin);
    return 0;
}

