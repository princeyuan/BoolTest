#include "VNFCreater.h"

void VNFCreater::Initialize(const std::string &expression)
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

void VNFCreater::CreateMutantsRun(std::vector<std::string> &expressions)
{
	int varnumber=m_varlist.size();
	int length=m_expression.size();

	//for each variable x
	for(int i=0;i<varnumber;i++)
	{
		std::string expr;
		int appeartimes=m_varappear[i].size();
		int k=0;

		//for each appearence of variable x
		for(int j=0;j<appeartimes;j++)
		{
			//copy charactors that before x
			for( ;k<m_varappear[i][j]-1;k++)
				expr.push_back(m_expression[k]);

			//if here is not "!x", add a '!' before x
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
		for( ;k<length;k++)
			expr.push_back(m_expression[k]);

		expressions.push_back(expr);
	}
}