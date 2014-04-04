#include "MLFCreater.h"

void MLFCreater::Initialize(const std::string &expression)
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

void MLFCreater::CreateMutantsRun(std::vector<std::string> &expressions)
{
	int literalnum=m_literal_appear.size();

	std::string::iterator first,last;
	bool cut_left_operator=true;

	for(int i=0;i<literalnum;i++)
	{
		std::string expr(m_expression);
		
		//a
		first=expr.begin()+m_literal_appear[i];
		last=expr.begin()+m_literal_appear[i]+1;
		//!a
		if(first!=expr.begin() && expr[m_literal_appear[i]-1]=='!')
		{
			first=first-1;
		}

		//(a) or (!a) remove ()
		while(first!=expr.begin() && last!=expr.end() &&
			  *(first-1)=='(' && *(last+1)==')')
		{
			first=first-1;
			last=last+1;
		}

		if(i==0)//a+...
		{
			last=last+1;
		}
		else if(i==literalnum-1)//...+a
		{
			first=first-1;
		}
		else if(*(first-1)=='(')//(a+...
		{
			last=last+1;
		}
		else if(*last==')')//...+a)
		{
			first=first-1;
		}
		else if(*(first-1)==*last)//...+a+... or ...*a*...
		{
			last=last+1;
		}
		else if(*(first-1)!=*last)//...+a*...
		{
			if(cut_left_operator)
			{
				first=first-1;
				cut_left_operator=false;
				--i;
			}
			else
			{
				last=last+1;
				cut_left_operator=true;
			}
		}

		expr.erase(first,last);
		expressions.push_back(expr);
	}
}