#include "SA1Creater.h"

void SA1Creater::CreaterInitialize()
{
    RecordLiteralAppearAndLength(m_expression,m_literal_appear,m_literal_length);
}

void SA1Creater::CreaterCleanStates()
{
    m_literal_appear.clear();
    m_literal_length.clear();
}

void SA1Creater::CreateMutantsRun(std::vector<std::string> &mu_expressions)
{
    int literalnum=m_literal_appear.size();

    for(int i=0;i<literalnum;i++)
    {
        std::string expr(m_expression);

        //a... or ...a... or ...a to 1... or ...1... or ...1
        if(m_literal_appear[i]==0 || expr[m_literal_appear[i]-1]!='!')
        {
            expr[m_literal_appear[i]]='1';
            expr.erase(expr.begin()+m_literal_appear[i]+1,expr.begin()+m_literal_appear[i]+m_literal_length[i]);
        }
        else //!a... or ...!a... or ...!a to 1... or ...1... or ...1
        {
            expr[m_literal_appear[i]]='1';
            expr.erase(expr.begin()+m_literal_appear[i]+1,expr.begin()+m_literal_appear[i]+m_literal_length[i]);
            expr.erase(expr.begin()+m_literal_appear[i]-1);
        }

        mu_expressions.push_back(expr);
    }
}