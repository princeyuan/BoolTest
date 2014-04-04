#include "CCFCreater.h"

void CCFCreater::Initialize(const std::string &expression)
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

void CCFCreater::CreateMutantsRun(std::vector<std::string> &expressions)
{
	int literalnum=m_literal_appear.size();
	int varnumber=m_varlist.size();

	//for literal x of expression
	for(int i=0;i<literalnum;i++)
	{
		//for each variable y //that is not x
		for(int j=0;j<varnumber;j++)
		{
			//if(m_varlist[j]!=m_expression[m_literal_appear[i]])
			//{
				std::string expr(m_expression);
				std::string expr2(m_expression);

				//replace literal x by (x*y) and (x*!y)
				if(m_literal_appear[i]==0 || expr[m_literal_appear[i]-1]!='!')
				{
					expr.insert(expr.begin()+m_literal_appear[i],'(');
					expr.insert(expr.begin()+m_literal_appear[i]+2,3,'*');
					expr[m_literal_appear[i]+3]=m_varlist[j];
					expr[m_literal_appear[i]+4]=')';

					expr2.insert(expr2.begin()+m_literal_appear[i],'(');
					expr2.insert(expr2.begin()+m_literal_appear[i]+2,4,'*');
					expr2[m_literal_appear[i]+3]='!';
					expr2[m_literal_appear[i]+4]=m_varlist[j];
					expr2[m_literal_appear[i]+5]=')';
				}
				//replace literal !x by (!x*y) and (!x*!y)
				else//if(m_literal_appear[i]-1>=0 && expr[m_literal_appear[i]-1]=='!')
				{
					expr.insert(expr.begin()+m_literal_appear[i]-1,'(');
					expr.insert(expr.begin()+m_literal_appear[i]+2,3,'*');
					expr[m_literal_appear[i]+3]=m_varlist[j];
					expr[m_literal_appear[i]+4]=')';

					expr2.insert(expr2.begin()+m_literal_appear[i]-1,'(');
					expr2.insert(expr2.begin()+m_literal_appear[i]+2,4,'*');
					expr2[m_literal_appear[i]+3]='!';
					expr2[m_literal_appear[i]+4]=m_varlist[j];
					expr2[m_literal_appear[i]+5]=')';
				}

				expressions.push_back(expr);
				expressions.push_back(expr2);
			//}
		}
	}
}