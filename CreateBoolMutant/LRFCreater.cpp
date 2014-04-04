#include "LRFCreater.h"

void LRFCreater::Initialize(const std::string &expression)
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
			}

			//record appear location of each literal
			m_literal_appear.push_back(i);
		}
	}
}

void LRFCreater::CreateMutantsRun(std::vector<std::string> &expressions)
{
	int literalnum=m_literal_appear.size();
	int varnumber=m_varlist.size();

	//for literal x of expression
	for(int i=0;i<literalnum;i++)
	{
		//for each variable y 
		for(int j=0;j<varnumber;j++)
		{
			//y is not x
			if(m_varlist[j]!=m_expression[m_literal_appear[i]])
			{
				//replace literal x by y
				std::string expr1(m_expression);
				expr1[m_literal_appear[i]]=m_varlist[j];
				expressions.push_back(expr1);

				//replace literal x by !y
				std::string expr2(m_expression);
				expr2[m_literal_appear[i]]=m_varlist[j];
				if(m_literal_appear[i]==0 || expr2[m_literal_appear[i]-1]!='!')//x... or ...x... or ...x
					expr2.insert(expr2.begin()+m_literal_appear[i],'!');
				else//!x
					expr2.erase(expr2.begin()+m_literal_appear[i]-1);
				expressions.push_back(expr2);
			}
			//y is x
			else
			{
				//replace literal x by x (y) means two condition are same
				//replace literal x by !x (!y)
				std::string expr3(m_expression);
				if(m_literal_appear[i]==0 || expr3[m_literal_appear[i]-1]!='!')//x... or ...x... or ...x
					expr3.insert(expr3.begin()+m_literal_appear[i],'!');
				else//!x
					expr3.erase(expr3.begin()+m_literal_appear[i]-1);
				expressions.push_back(expr3);
			}
		}
	}
}