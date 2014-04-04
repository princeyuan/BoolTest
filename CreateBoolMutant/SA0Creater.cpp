#include "SA0Creater.h"

void SA0Creater::Initialize(const std::string &expression)
{
	int exp_length=m_expression.size();

	//for each symbol of boolean expression
	for(int i=0;i<exp_length;i++)
	{
		//if the symbol is boolean variable 
		if(m_expression[i]>='a' && m_expression[i]<='z')
		{
			//record appear location of each literal
			m_literal_appear.push_back(i);
		}
	}
}

void SA0Creater::CreateMutantsRun(std::vector<std::string> &expressions)
{
	int literalnum=m_literal_appear.size();

	for(int i=0;i<literalnum;i++)
	{
		std::string expr(m_expression);

		//a... or ...a... or ...a to 0... or ...0... or ...0
		if(m_literal_appear[i]==0 || expr[m_literal_appear[i]-1]!='!')
		{
			expr[m_literal_appear[i]]='0';
		}
		else //!a... or ...!a... or ...!a to 0... or ...0... or ...0
		{
			expr[m_literal_appear[i]]='0';
			expr.erase(expr.begin()+m_literal_appear[i]-1);
		}

		expressions.push_back(expr);
	}
}