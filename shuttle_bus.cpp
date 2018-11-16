#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;

const int MAXN = 102;

void dp(int m, int n, int map[][MAXN])
{
    int val[MAXN][MAXN];
    memset(val, 0, sizeof(val));

    for(int i=0; i<n; i++)
        val[i][0] = map[i][0];
    for(int i=0; i<m; i++)
        val[0][i] = map[0][i];

    for(int i=1; i<=n; i++)
        for(int j=1; j<=m; j++)
        {
            val[i][j] = max(val[i-1][j] + map[i][j], val[i][j-1] + map[i][j]);
        }

    /*
    for(int i=0; i<=n; i++)
    {
        for(int j=0; j<=m; j++)
            printf("%d ", val[i][j]);
        puts("");
    }
    */

    printf("%d\n", val[n][m]);
}

int main(int args, char** argv)
{
    freopen(argv[1], "r", stdin);
    //freopen("output", "w", stdout);

    int n, m;
    int map[MAXN][MAXN];
    while(scanf("%d, %d", &m, &n)!=EOF)
    {
        for(int i=1; i<=n; i++)
        {
            int j;
            for(j=1; j<m; j++)
                scanf("%d, ", &map[i][j]);
            scanf("%d", &map[i][j]);
        }

        dp(m, n, map);
    }

    fclose(stdin);
    //fclose(stdout);
    return 0;
}

