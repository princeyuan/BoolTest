#include "OriginExprFileIO.h"

#include <fstream>
#include <iostream>

void ReadOriginExprFile(const std::string &expr_file,
						std::string &expr_common_name,
						std::vector<std::string> &expressions)
{
	std::ifstream infile;
	infile.open(expr_file.c_str());
	if(!infile)
	{
		std::cout<<"Error when reading file "<<expr_file;
		return;
	}

	std::string::const_iterator it1=expr_file.begin();
	for(std::string::const_iterator it=expr_file.begin();
		it!=expr_file.end();++it)
	{
		if(*it=='/')
			it1=it+1;
	}
	std::string::const_iterator it2=it1;
	for(;it2!=expr_file.end();++it2)
	{
		if(*it2=='.')
			break;
	}
	expr_common_name.assign(it1,it2);

	try
	{
		char buffer[1024];
		while(infile.getline(buffer,1024,'\n'))
		{
			if(strlen(buffer)!=0)
			{
				expressions.push_back(buffer);
			}
		}
	}
	catch (...)
	{
		infile.close();
		std::cout<<"Error when reading file "<<expr_file;
		return;
	}

	infile.close();
}