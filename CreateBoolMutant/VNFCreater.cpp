#include "VNFCreater.h"

void VNFCreater::CreaterInitialize()
{
    std::vector<int> temp_varlength;
    std::vector<std::string> temp_varlist;
    RecordVarListAndAppearAndLength(m_expression,temp_varlist,m_varappear,temp_varlength);
}

void VNFCreater::CreaterCleanStates()
{
    m_varappear.clear();
}

void VNFCreater::CreateMutantsRun(std::vector<std::string> &mu_expressions)
{
    //for each variable x
    for(int i=0;i<m_varappear.size();i++)
    {
        std::string expr;
        int appeartimes=m_varappear[i].size();
        int k=0;

        //for each appearence of variable x
        for(int j=0;j<m_varappear[i].size();j++)
        {
            //copy charactors that before x
            for( ;k<m_varappear[i][j]-1;k++)
                expr.push_back(m_expression[k]);

            //if here is not "!x", add '!' before x
            if(m_expression[k]!='!')
            {
                //if this x is the first literal
                if(k==0)
                    expr.push_back('!');
                else
                {
                    expr.push_back(m_expression[k]);
                    expr.push_back('!');
                    k++;
                }
            }
            //if here is "!x", delete the '!'
            else
                k++;
        }

        //copy all other charactors that after the last x
        for( ;k<m_expression.size();k++)
            expr.push_back(m_expression[k]);

        mu_expressions.push_back(expr);
    }
}