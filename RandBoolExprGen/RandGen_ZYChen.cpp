#include "RandGen_ZYChen.h"

#include <time.h>
#include <vector>
#include <string>
#include <iostream>

void RandBoolExprGen_ZYChen(int lit_num,int var_num,std::string &expr)
{
	expr.clear();
	std::vector<std::string> expr_temp(lit_num*2-1);

	int i,r;

	//set conditions
	for(i=0;i<lit_num;++i)
	{
		r=rand();
		if(r%2==0)
		{
			expr_temp[i*2].push_back('!');
		}

		r=rand();
		expr_temp[i*2].push_back('a'+r%var_num);
	}

	//set operator '+' or '*'
	for(i=0;i<lit_num-1;++i)
	{
		r=rand();
		if(r%2==0)
		{
			expr_temp[i*2+1].push_back('+');
		}
		else
		{
			expr_temp[i*2+1].push_back('*');
		}
	}

	//set association operator "()"
	int step=1;
	int length=lit_num;
	int probability_reciprocal=10;
	while(expr_temp.size() >= (step*2+3))
	{
		std::vector<int> sub_pos;

		for(i=0;i<length-step;++i)
		{
			r=rand();
			if(r%probability_reciprocal==0)
			{
				sub_pos.push_back(i);
			}
		}

		for(int i=1;i<sub_pos.size();++i)
		{
			if(sub_pos[i]-sub_pos[i-1]<2)
			{
				sub_pos.erase(sub_pos.begin()+i-1,sub_pos.begin()+i);
			}
		}

		if(!sub_pos.empty())
		{
			for(int i=sub_pos.size()-1;i>=0;--i)
			{
				std::string sub_expr;

				if(rand()%2==0)
				{
					sub_expr=std::string("!(");
				}
				else
				{
					sub_expr=std::string("(");
				}
				sub_expr=sub_expr+expr_temp[sub_pos[i]*2];
				sub_expr=sub_expr+expr_temp[sub_pos[i]*2+1];
				sub_expr=sub_expr+expr_temp[sub_pos[i]*2+2];
				sub_expr=sub_expr+std::string(")");

				expr_temp.insert(expr_temp.begin()+(sub_pos[i]*2),sub_expr);
				expr_temp.erase(expr_temp.begin()+(sub_pos[i]*2)+1,expr_temp.begin()+(sub_pos[i]*2)+4);

				length=length-1;
			}

			probability_reciprocal+=10;
		}
		else
		{
			++step;
			//probability_reciprocal=10;
		}	
	}

	for(i=0;i<expr_temp.size();++i)
	{
		expr=expr+expr_temp[i];
	}
}
