#include <cstdio>
#include <cstring>

const int MAXN = 102;

namespace {
int map[MAXN][MAXN];

inline int max(int a, int b)
{
    return a > b ? a : b;
}
}

int main(int, char** argv)
{
    freopen(argv[1], "r", stdin);

    int n;
    int m;
    int j;
    int val;

    while(scanf("%d, %d", &m, &n)!=EOF)
    {
        for(int i=1; i<=n; i++)
        {
            for(j=1; j<m; j++)
            {
                scanf("%d,", &val);
                map[i][j] = max(map[i-1][j], map[i][j-1]) + val;
            }
            scanf("%d", &val);
            map[i][j] = max(map[i-1][j], map[i][j-1]) + val;
        }

        fprintf(stdout, "%d\n", map[n][m]);
    }

    fclose(stdin);
    return 0;
}

