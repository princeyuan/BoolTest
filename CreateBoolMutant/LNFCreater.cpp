#include "LNFCreater.h"

void LNFCreater::CreaterInitialize()
{
    std::vector<int> temp_literal_length;
    RecordLiteralAppearAndLength(m_expression,m_literal_appear,temp_literal_length);
}

void LNFCreater::CreaterCleanStates()
{
    m_literal_appear.clear();
}

void LNFCreater::CreateMutantsRun(std::vector<std::string> &expressions)
{
    int literalnum=m_literal_appear.size();

    for(int i=0;i<literalnum;i++)
    {
        std::string expr(m_expression);

        if(m_literal_appear[i]==0 || expr[m_literal_appear[i]-1]!='!')//a... or ...a... or ...a
            expr.insert(expr.begin()+m_literal_appear[i],'!');
        else///!a or ...!a... or ...!a
            expr.erase(expr.begin()+m_literal_appear[i]-1);

        expressions.push_back(expr);
    }
}