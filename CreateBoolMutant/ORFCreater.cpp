#include "ORFCreater.h"

void ORFCreater::CreaterInitialize()
{
    for(int i=0;i<m_expression.size();i++)
    {
        if(m_expression[i]=='*' || m_expression[i]=='+')
        {
            m_operator_positions.push_back(i);
        }
    }
}

void ORFCreater::CreaterCleanStates()
{
    m_operator_positions.clear();
}

void ORFCreater::CreateMutantsRun(std::vector<std::string> &expressions)
{
    for(std::list<int>::const_iterator it=m_operator_positions.begin();
        it!=m_operator_positions.end();++it)
    {
        std::string expr(m_expression);

        if(expr[*it]=='*')
            expr[*it]='+';
        else
            expr[*it]='*';

        expressions.push_back(expr);
    }
}