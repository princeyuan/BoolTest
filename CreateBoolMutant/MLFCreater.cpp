#include "MLFCreater.h"

void MLFCreater::CreaterInitialize()
{
    RecordLiteralAppearAndLength(m_expression,m_literal_appear,m_literal_length);
}

void MLFCreater::CreaterCleanStates()
{
    m_literal_appear.clear();
    m_literal_length.clear();
}

void MLFCreater::CreateMutantsRun(std::vector<std::string> &expressions)
{
    int literalnum=m_literal_appear.size();

    std::string::iterator first,last;
    bool cut_left_operator=true;

    for(int i=0;i<literalnum;i++)
    {
        std::string expr(m_expression);
        
        //a
        first=expr.begin()+m_literal_appear[i];
        last=expr.begin()+m_literal_appear[i]+m_literal_length[i];

        //!a
        if(first!=expr.begin() && expr[m_literal_appear[i]-1]=='!')
        {
            --first;
        }

        //(a) or (!a) remove ()
        while(first!=expr.begin() && last!=expr.end() &&
              *(first-1)=='(' && *(last)==')')
        {
            --first;
            ++last;
        }

        if(i==0)//a+... or a*...
        {
            ++last;
        }
        else if(i==literalnum-1)//...+a or ...*a
        {
            --first;
        }
        else if(*(first-1)=='(')//(a+... or (a*...
        {
            ++last;
        }
        else if(*last==')')//...+a) or ...*a)
        {
            --first;
        }
        else if(*(first-1)==*last)//...+a+... or ...*a*...
        {
            ++last=last;
        }
        else if(*(first-1)!=*last)//...+a*... or ...*a+...
        {
            if(cut_left_operator)
            {
                --first;
                cut_left_operator=false;
                //repeat again to mutate for cut_left_operator=false
                --i;
            }
            else
            {
                ++last;
                cut_left_operator=true;
            }
        }

        expr.erase(first,last);
        expressions.push_back(expr);
    }
}