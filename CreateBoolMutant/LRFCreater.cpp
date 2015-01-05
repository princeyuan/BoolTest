#include "LRFCreater.h"

void LRFCreater::CreaterInitialize()
{
    RecordVarListAndLengthAndMap(m_expression,m_varlist,m_varlength,m_literal_var_map);
}

void LRFCreater::CreaterCleanStates()
{
    m_varlist.clear();
    m_varlength.clear();
    m_literal_var_map.clear();
}

void LRFCreater::CreateMutantsRun(std::vector<std::string> &expressions)
{
    int varnumber=m_varlist.size();
    int literalnum=m_literal_var_map.size();

    //for each literal x
    for(int i=0;i<literalnum;i++)
    {
        //for each variable y
        for(int k=0;k<varnumber;k++)
        {
            //y is not x
            if(m_literal_var_map[i].second!=k)
            {
                //replace literal x by y
                std::string expr1(m_expression);
                expr1.replace(m_literal_var_map[i].first,
                    m_varlength[m_literal_var_map[i].second],
                    m_varlist[k]);
                expressions.push_back(expr1);

                //replace literal x by !y
                std::string expr2(m_expression);
                expr2.replace(m_literal_var_map[i].first,
                    m_varlength[m_literal_var_map[i].second],
                    m_varlist[k]);
                //x... or ...x... or ...x
                if(m_literal_var_map[i].first==0 || m_expression[m_literal_var_map[i].first-1]!='!')
                    expr2.insert(expr2.begin()+m_literal_var_map[i].first,'!');
                //!x
                else
                    expr2.erase(expr2.begin()+m_literal_var_map[i].first-1);
                expressions.push_back(expr2);
            }
            //y is x
            else
            {
                //replace literal x by y(=x), get same expression

                //replace literal x by !y(=!x)
                std::string expr3(m_expression);
                //x... or ...x... or ...x
                if(m_literal_var_map[i].first==0 || m_expression[m_literal_var_map[i].first-1]!='!')
                    expr3.insert(expr3.begin()+m_literal_var_map[i].first,'!');
                //!x
                else
                    expr3.erase(expr3.begin()+m_literal_var_map[i].first-1);
                expressions.push_back(expr3);
            }
        }
    }
}