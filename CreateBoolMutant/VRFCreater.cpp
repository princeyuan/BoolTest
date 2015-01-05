#include "VRFCreater.h"

void VRFCreater::CreaterInitialize()
{
    RecordVarListAndAppearAndLength(m_expression,m_varlist,m_varappear,m_varlength);
}

void VRFCreater::CreaterCleanStates()
{
    m_varlist.clear();
    m_varappear.clear();
    m_varlength.clear();
}

void VRFCreater::CreateMutantsRun(std::vector<std::string> &mu_expressions)
{
    int varnumber=m_varlist.size();

    //for each variable x of expression
    for(int i=0;i<varnumber;i++)
    {
        //for each variable y that is not x
        for(int j=0;j<varnumber;j++)
        {
            if(i!=j)
            {
                std::string expr(m_expression);

                //replace all appearence of variable x by variable y
                for(int k=0;k<m_varappear[i].size();k++)
                {
                    int offset=m_varlength[j]-m_varlength[i];
                    expr.replace(m_varappear[i][k]+(offset*k),m_varlength[i],m_varlist[j]);
                }

                mu_expressions.push_back(expr);
            }
        }
    }
}