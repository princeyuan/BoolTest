#include <list>
#include <time.h>
#include <fstream>
#include <iostream>

#include "RandGen_ZYChen.h"

void main(int argc, char* argv[])
{
	//handle argument
	//e.g.: RandBoolExprGen 20 - 40 10 - 20 dir/RandExpr.txt
	int min_length=atoi(argv[1]);
	int max_length=atoi(argv[3]);
	int min_var_num=atoi(argv[4]);
	int max_var_num=atoi(argv[6]);
	int expr_num=atoi(argv[7]);
	char *out_file_name=argv[8];

	srand(time(NULL));
	std::list<std::string> exprs;

	for(int i=0;i<expr_num;++i)
	{
		int lit_num=min_length+(rand()&(max_length-min_length+1));
		int var_num;
		if(min_var_num+(rand()%(max_var_num-min_var_num+1))<lit_num*2/3)
			var_num=min_var_num+(rand()%(max_var_num-min_var_num+1));
		else
			var_num=lit_num*2/3;

		std::string temp;
		RandBoolExprGen_ZYChen(lit_num,var_num,temp);
		exprs.push_back(temp);
	}

	std::ofstream outfile;
	outfile.open(out_file_name);
	if(!outfile)
	{
		std::cout<<"Error when creating file "<<out_file_name;
		return;
	};
	try
	{
		int i=1;
		for(std::list<std::string>::const_iterator it=exprs.begin();
			it!=exprs.end();++it,++i)
		{
			outfile<<*it<<'\n';
			std::cout<<i<<". "<<*it<<'\n';
		}
	}
	catch (...)
	{
		outfile.close();
		std::cout<<"Error when creating file "<<out_file_name;
		return;
	}
	outfile.close();
}