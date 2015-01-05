#include "ENFCreater.h"

void ENFCreater::CreaterInitialize()
{
    for(int i=0;i<m_expression.size();i++)
    {
        if(m_expression[i]=='(')
            m_left_brackets_positions.push_back(i);
    }
}

void ENFCreater::CreaterCleanStates()
{
    m_left_brackets_positions.clear();
}

void ENFCreater::CreateMutantsRun(std::vector<std::string> &expressions)
{
    for(std::list<int>::const_iterator it=m_left_brackets_positions.begin();
        it!=m_left_brackets_positions.end();++it)
    {
        std::string expr(m_expression);

        if(*it==0 || expr[(*it)-1]!='!')//(...)... or ...(...) to !(...)... or ...!(...)
            expr.insert(expr.begin()+(*it),'!');
        else//!(...) or ...!(...) to (...)... or ...(...)
            expr.erase(expr.begin()+(*it)-1);

        expressions.push_back(expr);
    }
}