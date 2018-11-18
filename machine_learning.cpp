#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cctype>
#include<cmath>
#include<map>
#include<iostream>
#include<fstream>
using namespace std;

const int TREE_WIDTH = 30;
const int WORDLENMAX = 128;
const int MAXN_FOR_REG = 10000;

struct trie_node_st
{
    int count;
    struct trie_node_st *next[TREE_WIDTH];
};

struct trie_node_st g_male_root={0, {NULL}};
struct trie_node_st g_female_root={0, {NULL}};

double g_training_male_num = 0;
double g_training_female_num = 0;
double g_training_male_str_num = 0;
double g_training_female_str_num = 0;
double g_fact = 0;   // For smoothing data

double g_index = 212.6;  // Regression index (7, 212.6)
double g_reg_step = 0.1;
int g_max_loop = 5000;
bool g_reg_flag = false;
map<string, bool> g_train_data;

inline int min(int a, int b)
{
    return a > b ? b : a;
}

inline bool is_alpha(char ch)
{
    if((ch>='A' && ch<='Z') || (ch>='a' && ch<='z'))
        return true;
    return false;
}

inline void printout(string name, string gendor)
{
    cout << name << "," << gendor << endl;
}

inline int insert(const char* word, bool is_male)
{
    struct trie_node_st *curr;
    struct trie_node_st *newnode;

    if(is_male)
    {
        curr = &g_male_root;
        g_training_male_str_num++;
    }
    else
    {
        curr = &g_female_root;
        g_training_female_str_num++;
    }

    for(int i=0; word[i]!='\0'; i++)
    {
        char ch = word[i] - 'a';
        if(curr->next[ch] == NULL)
        {
            newnode=new trie_node_st();
            memset(newnode, 0, sizeof(struct trie_node_st));
            curr->next[ch] = newnode;
        }
        curr = curr->next[ch];
    }
    curr->count++;

    return 0;
}

inline double get_p(const char* word, struct trie_node_st *rootp)
{
    struct trie_node_st *curr = rootp;

    for(int i=0; word[i] != '\0'; i++)
    {
        char ch = tolower(word[i]) - 'a';
        if(curr->next[ch] == NULL)
            return 0;
        else
            curr = curr->next[ch];
    }

    return curr->count/(g_training_male_str_num + g_training_female_str_num * g_fact);
}

inline bool predict(const char* name)
{
    int j = 0;
    char newname[100];
    for(int i=0; name[i]!='\0'; i++)
    {
        if(is_alpha(name[i]))
            newname[j++] = tolower(name[i]);
        else
            continue;
    }
    newname[j] = '\0';

    char tmpstr[100];
    double male_p = 0;
    double female_p = 0;
    for(int i=strlen(newname)-1; i>=0; i--)
    {
        memset(tmpstr, 0, sizeof(tmpstr));
        male_p += get_p(strncpy(tmpstr, newname+i, strlen(newname)-i), &g_male_root) * pow((strlen(newname)-i), g_index);

        memset(tmpstr, 0, sizeof(tmpstr));
        female_p += get_p(strncpy(tmpstr, newname+i, strlen(newname)-i), &g_female_root) * pow((strlen(newname)-i), g_index);
    }

    if(male_p >= female_p)
        return true;
    else
        return false;
}

double regression()
{
    double succ_cnt = 0;
    map<string, bool>::iterator it;
    for(it=g_train_data.begin(); it != g_train_data.end(); it++)
    {
        if(predict(it->first.c_str()) == it->second)
            succ_cnt++;
    }
    return succ_cnt/g_train_data.size();
}

void run_regression()
{
    if(g_reg_flag == false)
        return;

    double max_succ_rate = regression();
    double max_index = g_index;

    while(g_max_loop--)
    {
        g_index += g_reg_step;
        double tmp_succ_rate = regression();
        if(tmp_succ_rate > max_succ_rate)
        {
            max_succ_rate = tmp_succ_rate;
            max_index = g_index;
        }
    }

    g_index = max_index;
}


void run_training()
{
    char linebuf[102];

    ifstream fin;
    fin.open("/var/www/html/training_dataset.txt");

    while(fin.getline(linebuf, 100))
    {
        char* pstr;
        char* name = strtok_r(linebuf, ",", &pstr);
        char* gendor = strtok_r(nullptr, ",", &pstr);

        int j = 0;
        char newname[100];
        for(int i=0; name[i]!='\0'; i++)
        {
            if(is_alpha(name[i]))
                newname[j++] = tolower(name[i]);
            else
                continue;
        }
        newname[j] = '\0';

        char tmpstr[100];
        bool is_male = strcasecmp(gendor, "male")==0;

        if(g_train_data.find(newname) == g_train_data.end())
            g_train_data.insert(pair<char*, bool>(newname, is_male));
        else
            continue;

        if(is_male)
            g_training_male_num++;
        else
            g_training_female_num++;

        for(int i=strlen(newname)-1; i>=0; i--)
        {
            memset(tmpstr, 0, sizeof(tmpstr));
            insert(strncpy(tmpstr, newname+i, strlen(newname)-i), is_male);
        }
    }
    fin.close();

    double tmp1 = g_training_male_num / g_training_female_num;
    double tmp2 = g_training_male_str_num / g_training_female_str_num;
    g_fact = tmp1 * tmp2;
}

void run_predict(char* argv)
{
    char line[102];

    ifstream fin;
    fin.open(argv);

    memset(line, 0, sizeof(line));
    while(fin.getline(line, 100, '\n'))
    {
        if(line[0] == '\0')
            continue;

        if(line[strlen(line)-1] == '\r')
        {
            line[strlen(line)-1] = '\0';
        }

        if(predict(line))
            printout(string(line), "male");
        else
            printout(string(line), "female");

        memset(line, 0, sizeof(line));
    }
    fin.close();
}

int main(int, char** argv)
{
    run_training();

    run_regression();

    run_predict(argv[1]);

    return 0;
}

