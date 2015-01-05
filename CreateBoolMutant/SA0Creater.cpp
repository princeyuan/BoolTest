#include "SA0Creater.h"

void SA0Creater::CreaterInitialize()
{
    RecordLiteralAppearAndLength(m_expression,m_literal_appear,m_literal_length);
}

void SA0Creater::CreaterCleanStates()
{
    m_literal_appear.clear();
    m_literal_length.clear();
}

void SA0Creater::CreateMutantsRun(std::vector<std::string> &mu_expressions)
{
    int literalnum=m_literal_appear.size();

    for(int i=0;i<literalnum;i++)
    {
        std::string expr(m_expression);

        //a... or ...a... or ...a to 0... or ...0... or ...0
        if(m_literal_appear[i]==0 || expr[m_literal_appear[i]-1]!='!')
        {
            expr[m_literal_appear[i]]='0';
            expr.erase(expr.begin()+m_literal_appear[i]+1,expr.begin()+m_literal_appear[i]+m_literal_length[i]);
        }
        else //!a... or ...!a... or ...!a to 0... or ...0... or ...0
        {
            expr[m_literal_appear[i]]='0';
            expr.erase(expr.begin()+m_literal_appear[i]+1,expr.begin()+m_literal_appear[i]+m_literal_length[i]);
            expr.erase(expr.begin()+m_literal_appear[i]-1);
        }

        mu_expressions.push_back(expr);
    }
}