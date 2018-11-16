#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;

const int MAXN = 129;

char a[100];
char b[100];
char c[100];
char variables[129];
char selected_variables[129];

void format_print(char* vmap)
{
    for(int i=0; i<strlen(a); i++)
        printf("%d", vmap[a[i]]);
    printf(" * ");
    for(int i=0; i<strlen(b); i++)
        printf("%d", vmap[b[i]]);
    printf(" = ");
    for(int i=0; i<strlen(c); i++)
        printf("%d", vmap[c[i]]);
    puts("");
}

void calc(char* vmap, int cnt)
{
    int va=0;
    for(int i=0; i<strlen(a); i++)
        va = va*10 + vmap[a[i]];

    int vb=0;
    for(int i=0; i<strlen(b); i++)
        vb = vb*10 + vmap[b[i]];

    int vc=0;
    for(int i=0; i<strlen(c); i++)
        vc = vc*10 + vmap[c[i]];

    if(va * vb == vc)
    {
        if(vmap[a[0]] != 0 && vmap[b[0]] != 0 && vmap[c[0]] != 0)
            format_print(vmap);
    }
}

void vloop(int idx, char* vmap, int* used, int cnt)
{
    for(int i=0; i<10; i++)
    {
        if(used[i])
        {
            continue;
        }
        else
        {
            used[i] = 1;
            vmap[selected_variables[idx]] = i;
        }

        if(idx == cnt-1)
        {
            calc(vmap, cnt);
        }
        else
        {
            vloop(idx+1, vmap, used, cnt);
        }
        used[i] = 0;
    }
}

int main(int args, char** argv)
{
    freopen(argv[1], "r", stdin);
    //freopen("slyar.out", "w", stdout);

    while(scanf("%s * %s = %s", a, b, c)!=EOF)
    {
        //printf("%s %s %s\n", a, b, c);
        memset(variables, 0, sizeof(variables));
        for (int i=0; a[i]!='\0'; i++)
        {
            variables[a[i]] = 1;
        }
        for (int i=0; b[i]!='\0'; i++)
        {
            variables[b[i]] = 1;
        }
        for (int i=0; c[i]!='\0'; i++)
        {
            variables[c[i]] = 1;
        }

        int var_cnt=0;
        for(int i=0; i<MAXN; i++)
        {
            if(variables[i])
                selected_variables[var_cnt++] = i;
        }

        char vmap[MAXN];
        int used[11];
        memset(vmap, 0, sizeof(vmap));
        memset(used, 0, sizeof(used));

        vloop(0, vmap, used, var_cnt);
    }

    fclose(stdin);
    //fclose(stdout);
    return 0;
}

