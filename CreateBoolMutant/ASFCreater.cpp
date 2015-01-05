#include "ASFCreater.h"

#include <iostream>

void ASFCreater::CreaterInitialize()
{
    std::stack<int> left_brackets_pos;
    
    for(int i=0;i<m_expression.size();i++)
    {
        if(m_expression[i]=='(')
        {
            left_brackets_pos.push(i);
        }
        if(m_expression[i]==')')
        {
            if(left_brackets_pos.empty())
            {
                throw "Boolean Expression Solecism!";
            }
            int pos=left_brackets_pos.top();
            left_brackets_pos.pop();
            std::pair<int,int> temp(pos,i);
            m_brackets_positions.push_back(temp);
        }
    }

    if(!left_brackets_pos.empty())
    {
        m_brackets_positions.clear();
        throw "Boolean Expression Solecism!";
    }
}

void ASFCreater::CreaterCleanStates()
{
    m_brackets_positions.clear();
}

void ASFCreater::CreateMutantsRun(std::vector<std::string> &mu_expressions)
{
    for(std::list<std::pair<int,int> >::const_iterator it=m_brackets_positions.begin();
        it!=m_brackets_positions.end();++it)
    {
        std::string expr(m_expression);
    
        expr.erase(expr.begin()+it->first);
        expr.erase(expr.begin()+it->second-1);

        mu_expressions.push_back(expr);
    }
}
