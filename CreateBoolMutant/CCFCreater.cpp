#include "CCFCreater.h"

void CCFCreater::CreaterInitialize()
{
    RecordVarListAndLengthAndMap(m_expression,m_varlist,m_varlength,m_literal_var_map);
}

void CCFCreater::CreaterCleanStates()
{
    m_varlist.clear();
    m_varlength.clear();
    m_literal_var_map.clear();
}

void CCFCreater::CreateMutantsRun(std::vector<std::string> &expressions)
{
    int varnumber=m_varlist.size();
    int literalnum=m_literal_var_map.size();

    //for each literal x
    for(int i=0;i<literalnum;i++)
    {
        //for each variable y
        for(int k=0;k<varnumber;k++)
        {
            std::string expr1(m_expression);
            std::string expr2(m_expression);

            //replace x by (x+y) and (x+!y)
            if(m_literal_var_map[i].first==0 || m_expression[m_literal_var_map[i].first-1]!='!')
            {
                expr1.insert(expr1.begin()+m_literal_var_map[i].first,'(');
                expr1.insert(m_literal_var_map[i].first+m_varlength[m_literal_var_map[i].second]+1,
                    std::string("*)"));
                expr1.insert(m_literal_var_map[i].first+m_varlength[m_literal_var_map[i].second]+2,
                    m_varlist[k]);

                expr2.insert(expr2.begin()+m_literal_var_map[i].first,'(');
                expr2.insert(m_literal_var_map[i].first+m_varlength[m_literal_var_map[i].second]+1,
                    std::string("*!)"));
                expr2.insert(m_literal_var_map[i].first+m_varlength[m_literal_var_map[i].second]+3,
                    m_varlist[k]);
            }
            //replace !x by (!x+y) and (!x+!y)
            else//if(m_varappear[i][j]-1>=0 && m_expression[m_varappear[i][j]-1]=='!')
            {
                expr1.insert(expr1.begin()+m_literal_var_map[i].first-1,'(');
                expr1.insert(m_literal_var_map[i].first+m_varlength[m_literal_var_map[i].second]+1,
                    std::string("*)"));
                expr1.insert(m_literal_var_map[i].first+m_varlength[m_literal_var_map[i].second]+2,
                    m_varlist[k]);

                expr2.insert(expr2.begin()+m_literal_var_map[i].first-1,'(');
                expr2.insert(m_literal_var_map[i].first+m_varlength[m_literal_var_map[i].second]+1,
                    std::string("*!)"));
                expr2.insert(m_literal_var_map[i].first+m_varlength[m_literal_var_map[i].second]+3,
                    m_varlist[k]);
            }

            expressions.push_back(expr1);
            expressions.push_back(expr2);
        }
    }
}