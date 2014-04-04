#include "ORFCreater.h"

void ORFCreater::Initialize(const std::string &expression)
{
	for(int i=0;i<expression.size();i++)
	{
		if(m_expression[i]=='*' || m_expression[i]=='+')
		{
			m_operator_appear.push_back(i);
		}
	}
}

void ORFCreater::CreateMutantsRun(std::vector<std::string> &expressions)
{
	for(std::list<int>::const_iterator it=m_operator_appear.begin();
		it!=m_operator_appear.end();++it)
	{
		std::string expr(m_expression);

		if(expr[*it]=='*')
			expr[*it]='+';
		else
			expr[*it]='*';

		expressions.push_back(expr);
	}
}