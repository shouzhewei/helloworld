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

double g_index = 7;  // Regression index
double g_reg_step = 0.1;
map<string, bool> g_train_data;

int min(int a, int b)
{
    return a > b ? b : a;
}

bool is_alpha(char ch)
{
    if((ch>='A' && ch<='Z') || (ch>='a' && ch<='z'))
        return true;
    return false;
}

int insert(const char* word, bool is_male)
{
    struct trie_node_st *curr, *newnode;

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
            newnode=(struct trie_node_st*)malloc(sizeof(struct trie_node_st));
            memset(newnode, 0, sizeof(struct trie_node_st));
            curr->next[ch] = newnode;
        }
        curr = curr->next[ch];
    }
    curr->count++;

    return 0;
}

double get_p(const char* word, struct trie_node_st *rootp)
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

/* return true  : male
 *        false : female
*/
bool predict(const char* name)
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
    double male_p = 0, female_p = 0;
    for(int i=strlen(newname)-1; i>=0; i--)
    {
        memset(tmpstr, 0, sizeof(tmpstr));

        //calc p with weight for male and female
        male_p += get_p(strcpy(tmpstr, newname+i), &g_male_root) * pow((strlen(newname)-i), g_index);
        female_p += get_p(strcpy(tmpstr, newname+i), &g_female_root) * pow((strlen(newname)-i), g_index);
    }

    //printf("male_p: %lf female_p: %lf\n", male_p, female_p);
    if(male_p >= female_p)
        return true;
    else
        return false;
}

double regression()
{
    double succ_cnt = 0;
    for(auto it=g_train_data.begin(); it != g_train_data.end(); it++)
    {
        if(predict(it->first.c_str()) == it->second)
            succ_cnt++;
    }
    return succ_cnt/g_train_data.size();
}

void run_regression()
{
    double max_succ_rate = regression();

    while(1)
    {
        g_index += g_reg_step;
        double tmp_succ_rate = regression();
        if(tmp_succ_rate > max_succ_rate)
        {
            max_succ_rate = tmp_succ_rate;
        }
        else
        {
            //printf("max success rate: %lf %lf\n", max_succ_rate, g_index);
            break;
        }
    }
}


void run_training()
{
    char linebuf[102];

    //freopen(argv, "r", stdin);
    ifstream fin;
    fin.open("/var/www/html/training_dataset.txt");

    //freopen("/var/www/html/training_dataset.txt", "r", stdin);
    while(fin.getline(linebuf, 100))
    {
        auto name = strtok(const_cast<char*>(linebuf), ",");
        auto gendor = strtok(nullptr, ",");

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
        //printf("newname: %s\n", newname);

        char tmpstr[100];
        bool is_male = strcasecmp(gendor, "male")==0;

        if(g_train_data.find(newname) == g_train_data.end())
            g_train_data.emplace(newname, is_male);
        else
            continue;

        if(is_male)
            g_training_male_num++;
        else
            g_training_female_num++;

        //sub prefix string as factors
        for(int i=strlen(newname)-1; i>=0; i--)
        {
            memset(tmpstr, 0, sizeof(tmpstr));
            insert(strcpy(tmpstr, newname+i), is_male);
        }
    }
    fin.close();
    //fclose(stdin);
    //printf("filtered train data size: %d\n", g_train_data.size());

    g_fact = (g_training_male_num / g_training_male_num) * (g_training_male_str_num / g_training_female_str_num);
}

void run_predict(char* argv)
{
    char line[102];

    //freopen(argv, "r", stdin);
    ifstream fin;
    fin.open(argv);

    memset(line, 0, sizeof(line));
    while(fin.getline(line, 100, '\n'))
    {
        //getchar();
        if(line[0] == '\0')
            continue;

        //int len=strlen(line);
        //line[len] = '\0';
        if(line[strlen(line)-1] == '\r')
        {
            line[strlen(line)-1] = '\0';
        }

        if(predict(line))
            cout << line << ",male\n";
        else
            cout << line << ",female\n";

        memset(line, 0, sizeof(line));
    }
    fin.close();
    //fclose(stdin);
}

int main(int args, char** argv)
{
    run_training();

    //run_regression();

    run_predict(argv[1]);

    return 0;
}

