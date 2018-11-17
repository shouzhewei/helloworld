#include <cstdio>
#include <cstdlib>
#include <cstring>

const int MAXN = 129;

char a[102];
char b[102];
char c[102];
char variables[129];
char selected_variables[129];

void format_print(char* vmap)
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
    printf("%s\n", output);
}

int calc_value(char* str, char* vmap)
{
    int ret = 0;

    for(int i=0; i<strlen(str); i++)
        ret = ret*10 + vmap[str[i]];

    return ret;
}

void calc(char* vmap)
{
    int va = calc_value(a, vmap);
    int vb = calc_value(b, vmap);
    int vc = calc_value(c, vmap);

    if((va * vb == vc) && (vmap[a[0]] != 0 && vmap[b[0]] != 0 && vmap[c[0]] != 0))
    {
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
            calc(vmap);
        }
        else
        {
            vloop(idx+1, vmap, used, cnt);
        }
        used[i] = 0;
    }
}

int main(int, char** argv)
{
    freopen(argv[1], "r", stdin);

    while(scanf("%100s * %100s = %100s", a, b, c)!=EOF)
    {
        memset(variables, 0, sizeof(variables));
        for (int i=0; a[i]!='\0' || b[i]!='\0' || c[i]!='\0'; i++)
        {
            if(a[i]!='\0')
                variables[a[i]] = 1;
            if(b[i]!='\0')
                variables[b[i]] = 1;
            if(c[i]!='\0')
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
    return 0;
}

