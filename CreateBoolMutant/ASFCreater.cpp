#include "ASFCreater.h"

#include <iostream>

void ASFCreater::Initialize(const std::string &expression)
{
	for(int i=0;i<m_expression.size();i++)
	{
		if(m_expression[i]=='(')
			m_left_parenthesis.push(i);
		if(m_expression[i]==')')
		{
			if(m_left_parenthesis.empty())
			{
				std::cerr<<"The boolean expression \""<<expression<<"\" has solecism!"<<std::endl;
				throw "Solecism!";
			}
			int pos=m_left_parenthesis.top();
			m_left_parenthesis.pop();
			std::pair<int,int> temp(pos,i);
			m_positions.push_back(temp);
		}
	}

	if(!m_left_parenthesis.empty())
	{
		std::cerr<<"The boolean expression \""<<expression<<"\" has solecism!"<<std::endl;
		throw "Solecism!";
	}
}

void ASFCreater::CreateMutantsRun(std::vector<std::string> &expressions)
{
	for(std::list<std::pair<int,int> >::const_iterator it=m_positions.begin();
		it!=m_positions.end();++it)
	{
		std::string expr(m_expression);
	
		expr.erase(expr.begin()+it->first);
		expr.erase(expr.begin()+it->second-1);

		expressions.push_back(expr);
	}
}