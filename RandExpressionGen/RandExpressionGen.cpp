#include <list>
#include <string>
#include <vector>
#include <time.h>
#include <fstream>
#include <iostream>

void RandGenerateAnExpression(int length,int var_num,std::string &expr);

int main(int argc, char* argv[])
{
    //Handle argument
    //E.G.: RandBoolExprGen 20 - 40 10 - 20 20

    int min_length;
    int max_length;
    int min_var_num;
    int max_var_num;
    int expr_num;

    if(argc==3)
    {
        min_length=max_length=atoi(argv[1]);
        min_var_num=max_var_num=atoi(argv[2]);
        expr_num=1;
    }
    else if(argc==4)
    {
        min_length=max_length=atoi(argv[1]);
        min_var_num=max_var_num=atoi(argv[2]);
        expr_num=atoi(argv[3]);
    }
    else if(argc==5)
    {
        if(strcmp("-",argv[2])==0)
        {
            min_length=atoi(argv[1]);
            max_length=atoi(argv[3]);
            min_var_num=max_var_num=atoi(argv[4]);
            expr_num=1;
        }
        else if(strcmp("-",argv[3])==0)
        {
            min_length=max_length=atoi(argv[1]);
            min_var_num=atoi(argv[2]);
            max_var_num=atoi(argv[4]);
            expr_num=1;
        }
        else
            return 0;
    }
    else if(argc==6)
    {
        if(strcmp("-",argv[2])==0)
        {
            min_length=atoi(argv[1]);
            max_length=atoi(argv[3]);
            min_var_num=max_var_num=atoi(argv[4]);
            expr_num=atoi(argv[5]);
        }
        else if(strcmp("-",argv[3])==0)
        {
            min_length=max_length=atoi(argv[1]);
            min_var_num=atoi(argv[2]);
            max_var_num=atoi(argv[4]);
            expr_num=atoi(argv[5]);
        }
        else
            return 0;
    }
    else if(argc==7)
    {
        min_length=atoi(argv[1]);
        max_length=atoi(argv[3]);
        min_var_num=atoi(argv[4]);
        max_var_num=atoi(argv[6]);
        expr_num=1;
    }
    else if(argc==8)
    {
        min_length=atoi(argv[1]);
        max_length=atoi(argv[3]);
        min_var_num=atoi(argv[4]);
        max_var_num=atoi(argv[6]);
        expr_num=atoi(argv[7]);
    }
    else
        return 0;

    srand(time(NULL));
    std::list<std::string> exprs;

    for(int i=0;i<expr_num;++i)
    {
        int lit_num=min_length+(rand()&(max_length-min_length+1));
        int var_num=min_var_num+(rand()%(max_var_num-min_var_num+1));
        if(var_num>lit_num)//var_num不大于lit_num
            var_num=lit_num;
        if(var_num>234)//var_num最大不超过26*9=234
            var_num=234;

        std::string temp;
        RandGenerateAnExpression(lit_num,var_num,temp);
        exprs.push_back(temp);
    }

    for(std::list<std::string>::const_iterator it=exprs.begin();
        it!=exprs.end();++it)
    {
        std::cout<<*it<<'\n';
    }

    return 0;
}

void RandGenerateAnExpression(int lit_num,int var_num,std::string &expr)
{
    if(lit_num<var_num || var_num>26*9)return;

    expr.clear();
    std::vector<std::string> expr_temp(lit_num*2-1);

    int i;

    //set conditions negtive or not
    bool *is_neg=new bool[lit_num];
    for(i=0;i<lit_num;++i)
    {
        if(rand()%2==0)
            is_neg[i]=true;//Means there is '!' in i-th condition
        else
            is_neg[i]=false;
    }

    //build variable list
    std::vector<std::string> vars(var_num);
    if(var_num<=26)
    {
        for(i=0;i<var_num;++i)
        {
            vars[i]='a'+i;
        }
    }
    else
    {
        for(i=0;i<var_num;++i)
        {
            vars[i]='a'+(i%26);
            vars[i].append(std::string(1,'1'+(i/26)));
        }
    }

    //set literature
    //first step: each variable appear at least once
    for(i=0;i<var_num;++i)
    {
        int index=rand()%lit_num;
        while(!expr_temp[index*2].empty())
            index=rand()%lit_num;

        if(is_neg[index])
            expr_temp[index*2]='!';

        expr_temp[index*2].append(vars[i]);
    }
    //second step: fix other literature
    for(i=0;i<lit_num;++i)
    {
        if(expr_temp[i*2].empty())
        {
            if(is_neg[i])
                expr_temp[i*2]='!';

            expr_temp[i*2].append(vars[rand()%var_num]);
        }
    }

    //set operator '+' or '*'
    for(i=0;i<lit_num-1;++i)
    {
        if(rand()%2==0)
        {
            expr_temp[i*2+1]='+';
        }
        else
        {
            expr_temp[i*2+1]='*';
        }
    }

    //set association operator "()"
    int step=1;
    int length=lit_num;
    int probability_reciprocal=10;
    while(expr_temp.size() >= (step*2+3))
    {
        std::vector<int> sub_pos;

        for(i=0;i<length-step;++i)
        {
            if(rand()%probability_reciprocal==0)
            {
                sub_pos.push_back(i);
            }
        }

        for(i=1;i<sub_pos.size();++i)
        {
            if(sub_pos[i]-sub_pos[i-1]<2)
            {
                sub_pos.erase(sub_pos.begin()+i-1,sub_pos.begin()+i);
            }
        }

        if(!sub_pos.empty())
        {
            for(i=sub_pos.size()-1;i>=0;--i)
            {
                std::string sub_expr;

                if(rand()%2==0)
                {
                    sub_expr=std::string("!(");
                }
                else
                {
                    sub_expr=std::string("(");
                }
                sub_expr=sub_expr+expr_temp[sub_pos[i]*2];
                sub_expr=sub_expr+expr_temp[sub_pos[i]*2+1];
                sub_expr=sub_expr+expr_temp[sub_pos[i]*2+2];
                sub_expr=sub_expr+std::string(")");

                expr_temp.insert(expr_temp.begin()+(sub_pos[i]*2),sub_expr);
                expr_temp.erase(expr_temp.begin()+(sub_pos[i]*2)+1,expr_temp.begin()+(sub_pos[i]*2)+4);

                length=length-1;
            }

            probability_reciprocal+=10;
        }
        else
        {
            ++step;
            //probability_reciprocal=10;
        }    
    }

    for(i=0;i<expr_temp.size();++i)
    {
        expr=expr+expr_temp[i];
    }
}