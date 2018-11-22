#include <cstdio>
#include <cstdlib>
#include <cstring>

const int MAXN = 129;
const int NUMS = 10;

namespace {
char a[102];
char b[102];
char c[102];
char variables[129];
char selectedvariables[129];

void formatprint(char* vmap)
{
    int cnt = 0;
    char output[102];

    for(int i=0; i<strlen(a); i++)
        output[cnt++] = vmap[a[i]] + '0';

    output[cnt++] = ' ';
    output[cnt++] = '*';
    output[cnt++] = ' ';

    for(int i=0; i<strlen(b); i++)
        output[cnt++] = vmap[b[i]] + '0';

    output[cnt++] = ' ';
    output[cnt++] = '=';
    output[cnt++] = ' ';

    for(int i=0; i<strlen(c); i++)
        output[cnt++] = vmap[c[i]] + '0';

    output[cnt] = '\0';
    fprintf(stdout, "%s\n", output);
}

int calcvalue(char* str, char* vmap)
{
    int ret = 0;

    for(int i=0; i<strlen(str); i++)
        ret = ret*10 + vmap[str[i]];

    return ret;
}

void calc(char* vmap)
{
    int va = calcvalue(a, vmap);
    int vb = calcvalue(b, vmap);
    int vc = calcvalue(c, vmap);

    if((va * vb == vc) && (vmap[a[0]] != 0 && vmap[b[0]] != 0 && vmap[c[0]] != 0))
    {
        formatprint(vmap);
    }
}

void vloop(int idx, char* vmap, int* used, int cnt)
{
    for(int i=0; i<NUMS; i++)
    {
        if(used[i] == 0)
        {
            used[i] = 1;
            vmap[selectedvariables[idx]] = i;

            if(idx == cnt-1)
            {
                calc(vmap);
            }
            else if(idx == 2 && (vmap[a[strlen(a)-1]] * vmap[b[strlen(b)-1]])%10 !=  vmap[c[strlen(c)-1]])
            {
            }
            else
            {
                vloop(idx+1, vmap, used, cnt);
            }
            used[i] = 0;
        }
    }
}

void sortselectedvariables(int& varcnt, char ch)
{
    int i;
    for(i=0; i<varcnt; i++)
    {
        if(selectedvariables[i] == ch)
            break;
    }
    if(i == varcnt)
    {
        selectedvariables[varcnt++] = ch;
    }
}

}

int main(int, char** argv)
{
    freopen(argv[1], "r", stdin);

    while(scanf("%100s * %100s = %100s", a, b, c)!=EOF)
    {
        memset(variables, 0, sizeof(variables));
        int varcnt=0;
        int depth=0;
        int i = strlen(a) - 1;
        int j = strlen(b) - 1;
        int k = strlen(c) - 1;

        while(true)
        {
            if(i<0 && j<0 && k<0)
                break;
            if(i >= 0)
            {
                sortselectedvariables(varcnt, a[i]);
                i--;
            }
            if(j >= 0)
            {
                sortselectedvariables(varcnt, b[j]);
                j--;
            }
            if(k >= 0)
            {
                sortselectedvariables(varcnt, c[k]);
                k--;
            }
        }

        char vmap[MAXN];
        int used[11];
        memset(vmap, 0, sizeof(vmap));
        memset(used, 0, sizeof(used));

        vloop(0, vmap, used, varcnt);
    }

    fclose(stdin);
    return 0;
}

