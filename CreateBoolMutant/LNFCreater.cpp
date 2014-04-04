#include "LNFCreater.h"

void LNFCreater::Initialize(const std::string &expression)
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