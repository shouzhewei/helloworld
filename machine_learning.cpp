#include<cstdio>
#include<cstdlib>
#include<cctype>
#include<cstring>
#include<cmath>
#include<map>
#include<iostream>
#include<fstream>

const int TREEWIDTH = 30;
const int WORDLENMAX = 128;
const int MAXNFORREG = 10000;
const int READBUFLEN = 100;

namespace {
struct trienodest
{
    int count;
    struct trienodest *next[TREEWIDTH];
};

struct trienodest gmaleroot={0, {NULL}};
struct trienodest gfemaleroot={0, {NULL}};

double gtrainingmalenum = 0;
double gtrainingfemalenum = 0;
double gtrainingmalestrnum = 0;
double gtrainingfemalestrnum = 0;
double gfact = 0;

double gindex = 212.6;
double gregstep = 0.1;
int gmaxloop = 5000;
bool gregflag = false;
std::map<std::string, bool> gtraindata;

inline int min(int a, int b)
{
    return a > b ? b : a;
}

inline bool isalpha(char ch)
{
    if((ch>='A' && ch<='Z') || (ch>='a' && ch<='z'))
        return true;
    return false;
}

inline void printout(std::string name, std::string gendor)
{
    std::cout << name << "," << gendor << std::endl;
}

inline int insert(const char* word, bool ismale)
{
    struct trienodest *curr;
    struct trienodest *newnode;

    if(ismale)
    {
        curr = &gmaleroot;
        gtrainingmalestrnum++;
    }
    else
    {
        curr = &gfemaleroot;
        gtrainingfemalestrnum++;
    }

    for(int i=0; word[i]!='\0'; i++)
    {
        char ch = word[i] - 'a';
        if(curr->next[ch] == NULL)
        {
            newnode=new trienodest();
            memset(newnode, 0, sizeof(struct trienodest));
            curr->next[ch] = newnode;
        }
        curr = curr->next[ch];
    }
    curr->count++;

    return 0;
}

inline double getp(const char* word, struct trienodest *rootp)
{
    struct trienodest *curr = rootp;

    for(int i=0; word[i] != '\0'; i++)
    {
        char ch = tolower(word[i]) - 'a';
        if(curr->next[ch] == NULL)
            return 0;
        else
            curr = curr->next[ch];
    }

    return curr->count/(gtrainingmalestrnum + gtrainingfemalestrnum * gfact);
}

inline bool predict(const char* name)
{
    int j = 0;
    char newname[100];
    for(int i=0; name[i]!='\0'; i++)
    {
        if(isalpha(name[i]))
            newname[j++] = tolower(name[i]);
    }
    newname[j] = '\0';

    char tmpstr[100];
    double malep = 0;
    double femalep = 0;
    for(int i=strlen(newname)-1; i>=0; i--)
    {
        memset(tmpstr, 0, sizeof(tmpstr));
        malep += getp(strncpy(tmpstr, newname+i, strlen(newname)-i), &gmaleroot) * pow((strlen(newname)-i), gindex);

        memset(tmpstr, 0, sizeof(tmpstr));
        femalep += getp(strncpy(tmpstr, newname+i, strlen(newname)-i), &gfemaleroot) * pow((strlen(newname)-i), gindex);
    }

    if(malep >= femalep)
        return true;
    else
        return false;
}

double regression()
{
    double succcnt = 0;
    std::map<std::string, bool>::iterator it;
    for(it=gtraindata.begin(); it != gtraindata.end(); it++)
    {
        if(predict(it->first.c_str()) == it->second)
            succcnt++;
    }
    return succcnt/gtraindata.size();
}

void runregression()
{
    if(gregflag == false)
        return;

    double maxsuccrate = regression();
    double maxindex = gindex;

    while(gmaxloop--)
    {
        gindex += gregstep;
        double tmpsuccrate = regression();
        if(tmpsuccrate > maxsuccrate)
        {
            maxsuccrate = tmpsuccrate;
            maxindex = gindex;
        }
    }

    gindex = maxindex;
}


void runtraining()
{
    char linebuf[102];

    std::ifstream fin;
    fin.open("/var/www/html/training_dataset.txt");

    while(fin.getline(linebuf, READBUFLEN))
    {
        char* pstr;
        char* name = strtok_r(linebuf, ",", &pstr);
        char* gendor = strtok_r(nullptr, ",", &pstr);

        int j = 0;
        char newname[100];
        for(int i=0; name[i]!='\0'; i++)
        {
            if(isalpha(name[i]))
                newname[j++] = tolower(name[i]);
        }
        newname[j] = '\0';

        char tmpstr[100];
        bool ismale = strcasecmp(gendor, "male")==0;

        if(gtraindata.find(newname) == gtraindata.end())
        {
            gtraindata.insert(std::pair<char*, bool>(newname, ismale));

            if(ismale)
                gtrainingmalenum++;
            else
                gtrainingfemalenum++;

            for(int i=strlen(newname)-1; i>=0; i--)
            {
                memset(tmpstr, 0, sizeof(tmpstr));
                insert(strncpy(tmpstr, newname+i, strlen(newname)-i), ismale);
            }
        }
    }
    fin.close();

    double tmp1 = gtrainingmalenum / gtrainingfemalenum;
    double tmp2 = gtrainingmalestrnum / gtrainingfemalestrnum;
    gfact = tmp1 * tmp2;
}

void runpredict(char* argv)
{
    char line[102];

    std::ifstream fin;
    fin.open(argv);

    memset(line, 0, sizeof(line));
    while(fin.getline(line, READBUFLEN, '\n'))
    {
        if(line[0] != '\0')
        {
            if(line[strlen(line)-1] == '\r')
            {
                line[strlen(line)-1] = '\0';
            }

            if(predict(line))
                printout(std::string(line), "male");
            else
                printout(std::string(line), "female");

            memset(line, 0, sizeof(line));
        }
    }
    fin.close();
}
}

int main(int, char** argv)
{
    runtraining();

    runregression();

    runpredict(argv[1]);

    return 0;
}

