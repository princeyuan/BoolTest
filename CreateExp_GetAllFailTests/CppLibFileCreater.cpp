#include "CppLibFileCreater.h"

#include <fstream>
#include <algorithm>

void CppLibFileCreater::run(const std::vector<std::string> &fun_names,
							const std::vector<std::vector<char> > &all_args,
							const std::vector<std::string> &expressions,
							const WriteCppLibFileState &state,
							const std::string &filename)
{
	std::string sourcecode;

	CreateSourceCode(fun_names,all_args,expressions,sourcecode);	
	CreateSourceFile(sourcecode,state,filename);
}

void CppLibFileCreater::CreateSourceCode(const std::vector<std::string> &fun_names,
										 const std::vector<std::vector<char> > &all_args,
										 const std::vector<std::string> &expressions,
										 std::string &sourcecode)
{
	for(int i=0;i<expressions.size();i++)
	{
		std::string temp;

		CreateFunName(fun_names[i],temp);
		CreateArglist(all_args[i],temp);
		CreateFunBody(expressions[i],temp);

		sourcecode.append(temp);
	}
}

void CppLibFileCreater::CreateSourceFile(const std::string &sourcecode,
										 const WriteCppLibFileState &state,
										 const std::string &filename)
{
	std::ofstream outfile;
	if(state==CppLibFileBegin || state==CppLibFileBoth)
		outfile.open(filename.c_str());
	else
		outfile.open(filename.c_str(),std::ios::app);
	if(!outfile)return;

	try
	{
		if(state==CppLibFileBegin || state==CppLibFileBoth)
		{
			std::string head_micro;
			CreateHeadMicro(filename,head_micro);
			outfile<<"#ifndef "<<head_micro<<"\n#define "<<head_micro<<"\n\n";
		}

		for(int i=0;i<sourcecode.size();i++)
		{
			outfile<<sourcecode[i];
		}

		if(state==CppLibFileEnd || state==CppLibFileBoth)
		{
			outfile<<"\n#endif\n";
		}
	}
	catch (...)
	{
		outfile.close();
		return;
	}

	outfile.close();
}

void CppLibFileCreater::CreateFunName(const std::string &name,std::string &code)
{
	code.append("bool ");
	code.append(name);
}

void CppLibFileCreater::CreateArglist(const std::vector<char> &arguments,std::string &code)
{
	code.append("(");
	std::vector<char>::const_iterator it=arguments.begin();
	while(true)
	{
		code.append("bool ");
		code.append(1,*it);
		if((++it)!=arguments.end())
			code.append(",");
		else
			break;
	}
	code.append(")\n");
}

void CppLibFileCreater::CreateFunBody(const std::string &expression,std::string &code)
{
	code.append("{\n\t");
	code.append("return ");
	code.append(expression);
	code.append(";\n}\n");
}

void CppLibFileCreater::CreateHeadMicro(const std::string &filename,std::string &head_micro)
{
	head_micro.clear();

	std::string::const_iterator it_begin=filename.begin();
	for(std::string::const_iterator it=filename.begin();
		it!=filename.end();++it)
	{
		if(*it=='/')
			it_begin=it+1;
	}

	head_micro.assign(it_begin,filename.end());

	for(std::string::iterator it=head_micro.begin();
		it!=head_micro.end();++it)
	{
		if(*it=='.')
			*it='_';
		else if(*it>='a' && *it<='z')
			*it='A'+(*it-'a');
	}
}