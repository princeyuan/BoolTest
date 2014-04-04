#include "VRFCreater.h"

void VRFCreater::Initialize(const std::string &expression)
{
	int exp_length=m_expression.size();

	//for each symbol of boolean expression
	for(int i=0;i<exp_length;i++)
	{
		//if the symbol is boolean variable 
		if(m_expression[i]>='a' && m_expression[i]<='z')
		{
			int list_length=m_varlist.size();
			//check whether the variable has been contained in variable list
			for(int j=0;j<list_length;j++)
			{
				if(m_expression[i]==m_varlist[j])
					break;
			}

			//if has not been contained, add it in
			if(j==list_length)
			{
				m_varlist.push_back(m_expression[i]);
				std::vector<int> temp;
				temp.push_back(i);
				m_varappear.push_back(temp);
			}

			//else, record the current appear location
			else
				m_varappear[j].push_back(i);
		}
	}
}

void VRFCreater::CreateMutantsRun(std::vector<std::string> &expressions)
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
				int appeartimes=m_varappear[i].size();

				//replace all appearence of variable x by variable y
				for(int k=0;k<appeartimes;k++)
				{
					expr[m_varappear[i][k]]=m_varlist[j];
				}

				expressions.push_back(expr);
			}
		}
	}
}