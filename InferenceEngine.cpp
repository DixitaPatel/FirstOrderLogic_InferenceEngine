#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <cstdlib>
#include <numeric>
using namespace std;

typedef struct Predicate{
    vector<string> param_list;
}Predicate;

typedef struct Clause{
    vector<Predicate> predicate_list;
}Clause;

typedef struct KB{
    vector<Clause> clause_list;
}KB;

vector<string> formtoken(string input_string, string delimiters)
{

    char * pch;
    vector<string> para;
    char *str = new char[input_string.length() + 1];
    strcpy(str, input_string.c_str());
    pch = strtok (str,delimiters.c_str());
    string c="";
    while (pch != NULL)
    {
        para.push_back(pch+c);
        pch = strtok (NULL, delimiters.c_str());
    }
    return para;
}

bool compare_predicate(vector<string> pred1,vector<string> pred2)
{
    bool status = false;
    if(pred1.size()== pred2.size())
    {
            string neg;
            if(pred1[0].find_first_of("~") == 0)
                pred1[0].erase(pred1[0].begin()+0);
            else
                pred1[0].insert(0,"~");
            neg = pred1[0];
            if(neg.compare(pred2[0])==0)
            {
                for(unsigned int i = 1;i < pred1.size();i++)
                {
                    if(pred1[i].length()>1 && pred2[i].length()==1)
                    {
                        status = true;
                    }
                    if(pred1[i].compare(pred2[i])==0)
                    {
                        status = true;
                    }
                    if(pred1[i].compare(pred2[i])!=0 && ((pred1[i].length()==1 && pred2[i].length()==1)|| (pred1[i].length()==1 && pred2[i].length()>1)))
                    {
                        status = false;
                        break;
                    }
                }
            }
    }
    return status;
}

vector<pair<string,string> > unify(vector<string> pred1,vector<string> pred2)
{
    vector<pair<string,string> > mapping;
    for(unsigned int i = 1;i < pred1.size();i++)
    {
        if(pred1[i].length()>1 && pred2[i].length()==1)
        {
            mapping.push_back(make_pair(pred2[i],pred1[i]));
        }
    }
    return mapping;
}

void replace_variable(vector<string> &pred_tobe_unified,vector<pair<string,string> > mapping)
{
    for (unsigned int i=0;i < mapping.size(); i++ )
    {
        string var = mapping[i].first;
        string cons = mapping[i].second;
        for(unsigned int j=1; j < pred_tobe_unified.size(); j++)
        {
            if(pred_tobe_unified[j] == var)
                pred_tobe_unified[j] = cons;
        }

    }
}

Clause string_to_clause(string input)
{
    Clause answer;
    Predicate temp;
    vector<string> temp_vec_1, temp_vec_2;
    temp_vec_1 = formtoken(input, " |");

    for(unsigned int i = 0; i < temp_vec_1.size(); i++)
    {
        temp_vec_2 = formtoken(temp_vec_1[i], " (,)");
        temp.param_list = temp_vec_2;
        answer.predicate_list.push_back(temp);
    }

    return answer;
}

void printtokens(vector<string> para)
{
    for(unsigned int i=0;i<para.size();i++)
        cout << para[i] << " ";
}

void printClause(Clause input)
{
    for(unsigned i = 0; i < input.predicate_list.size(); i++)
    {
        printtokens(input.predicate_list[i].param_list);
    }
}
void printKB(KB input)
{
    for(unsigned int i = 0;i < input.clause_list.size();i++){
        printClause(input.clause_list[i]);
        cout << endl;
    }

}
bool termianteCondi(KB input)
{
    bool result;
    for(unsigned int i = 0;i < input.clause_list.size();i++)
    {
        for(unsigned int j = i+1;j < input.clause_list.size();j++)
        {
            result = compare_predicate(input.clause_list[i].predicate_list[0].param_list,input.clause_list[j].predicate_list[0].param_list);
            printtokens(input.clause_list[i].predicate_list[0].param_list);
            printtokens(input.clause_list[j].predicate_list[0].param_list);
            if(result == true)
                return true;
        }

    }
    return false;
}

int main()
{
    int no_of_queries, no_of_rules;
    string line,l;
    vector<string> queries;
    KB kb_1, kb_n;
    ifstream fd;
    ofstream outfile;
    outfile.open("output.txt");
    fd.open("input.txt");
    getline(fd,line);
    no_of_queries = atoi(line.c_str());
    for(int i = 0; i < no_of_queries; i++)
    {
        getline(fd,line);
        queries.push_back(line);
    }
    getline(fd,line);
    no_of_rules = atoi(line.c_str());
    for(int i = 0;i < no_of_rules;i++)
    {
        getline(fd,line);
        Clause temp = string_to_clause(line);
        if(temp.predicate_list.size() == 1)
            kb_1.clause_list.push_back(temp);
        else
            kb_n.clause_list.push_back(temp);
    }

    KB kb_1_curr, kb_n_curr;

    vector<pair<string,string> > mapping;
    for(int i = 0; i < no_of_queries; i++)
    {
        kb_1_curr = kb_1;
        kb_n_curr = kb_n;
        string current_query = queries[i];
        Clause query_clause;
        Predicate temp_pred;
        temp_pred.param_list = formtoken(current_query, " (,)");
        temp_pred.param_list[0] = temp_pred.param_list[0].insert(0,"~");
        query_clause.predicate_list.push_back(temp_pred);

        kb_1_curr.clause_list.push_back(query_clause);
        bool stat;
        vector<string> p;
        Clause temp;
        bool result = false;
        for(unsigned int i = 0;i < kb_1_curr.clause_list.size();i++)
        {
            for(unsigned int j = 0;j < kb_n_curr.clause_list.size();j++)
            {
                stat = false;
                for(unsigned int k = 0;k < kb_n_curr.clause_list[j].predicate_list.size();k++)
                {
                    stat = compare_predicate(kb_1_curr.clause_list[i].predicate_list[0].param_list,kb_n_curr.clause_list[j].predicate_list[k].param_list);
                    if(stat == true)
                    {
                        mapping = unify(kb_1_curr.clause_list[i].predicate_list[0].param_list,kb_n_curr.clause_list[j].predicate_list[k].param_list);
                        temp = kb_n_curr.clause_list[j];
                        temp.predicate_list.erase(temp.predicate_list.begin() + k);
                        for(unsigned int i = 0; i < temp.predicate_list.size();i++)
                        {
                            replace_variable(temp.predicate_list[i].param_list,mapping);
                        }
                        if(temp.predicate_list.size() == 1)
                                kb_1_curr.clause_list.push_back(temp);
                        else
                                kb_n_curr.clause_list.push_back(temp);

                        break;
                    }
                    result = termianteCondi(kb_1_curr);
                    if(result == true)
                        break;
                }
                if(result == true)
                    break;
            }
            if(result == true)
                break;
        }
        if(result == 0)
            outfile << "False";
        else
            outfile << "True";
     }
return 0;
}

