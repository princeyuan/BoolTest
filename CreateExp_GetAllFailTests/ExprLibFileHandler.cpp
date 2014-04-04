#include "ExprLibFileHandler.h"

#include <fstream>
#include <iostream>
#include <algorithm>
#include "BoolTranslater.h"
#include "CppLibFileCreater.h"
#include "../SolutionBase/AssistMethod.h"
#include "../SolutionBase/OriginExprFileIO.h"
#include "../SolutionBase/MutantExprFileIO.h"

void HandleOrigin(const std::string &expr_file,
				  const std::string &cpp_head_file,
				  std::vector<std::vector<char> > &all_argument_list,
				  std::vector<std::string> &fun_names)
{
	std::string common_name;
	std::vector<std::string> expressions;

	ReadOriginExprFile(expr_file,common_name,expressions);

	CreateAllArglist(expressions,all_argument_list);

	WriteOriginCppFile(common_name,expressions,all_argument_list,
					   cpp_head_file,fun_names);
}

void HandleMutant(const std::string &expr_file,
				  const std::string &cpp_head_file,
				  const std::vector<std::vector<char> > &all_argument_list,
				  std::vector<std::vector<std::vector<std::string> > > &fun_names)
{
	std::string common_name;
	std::vector<int> origin_index;
	std::vector<std::string> origin_exprs;
	std::vector<std::vector<std::string> > mutant_types;
	std::vector<std::vector<std::vector<std::string> > > expressions;

	ReadMutantExprFile(expr_file,common_name,origin_index,origin_exprs,mutant_types,expressions);

	WriteMutantCppFile(common_name,origin_index,origin_exprs,mutant_types,expressions,all_argument_list,
					   cpp_head_file,fun_names);
}

void WriteOriginCppFile(const std::string &name,
						const std::vector<std::string> &expressions,
						const std::vector<std::vector<char> > &all_argument_list,
						const std::string &filename,
						std::vector<std::string> &fun_names)
{
	//转化为cpp格式的表达式
	BoolTranslater tranlater;
	std::vector<std::string> all_cpp_exprs(expressions.size());
	for(int i=0;i<expressions.size();i++)
	{
		tranlater.run(expressions[i],all_cpp_exprs[i]);
	}

	//批量生成函数名
	fun_names.clear();
	fun_names.resize(expressions.size(),name);
	for(int i=0;i<expressions.size();i++)
	{
		std::string temp;
		int2str(i+1,temp);
		fun_names[i].append(temp);
	}

	//转化为cpp函数并写入指定的一个文件
	CppLibFileCreater lib_creater;
	lib_creater.run(fun_names,all_argument_list,all_cpp_exprs,CppLibFileBoth,filename);
}

void WriteMutantCppFile(const std::string &common_name,
						const std::vector<int> &origin_index,
						const std::vector<std::string> &origin_exprs,
						const std::vector<std::vector<std::string> > &mutant_types,
						const std::vector<std::vector<std::vector<std::string> > > &expressions,
						const std::vector<std::vector<char> > &all_argument_list,
						const std::string &filename,
						std::vector<std::vector<std::vector<std::string> > > &fun_names)
{
	//转化为cpp格式的表达式
	BoolTranslater tranlater;
	std::vector<std::vector<std::vector<std::string> > > all_cpp_mutants(expressions.size());

	for(int i=0;i<expressions.size();i++)
	{
		all_cpp_mutants[i].resize(expressions[i].size());
		for(int j=0;j<expressions[i].size();j++)
		{
			all_cpp_mutants[i][j].resize(expressions[i][j].size());
			for(int k=0;k<expressions[i][j].size();k++)
			{
				tranlater.run(expressions[i][j][k],all_cpp_mutants[i][j][k]);
			}
		}
	}

	//批量生成函数名
	fun_names.resize(expressions.size());
	for(int i=0;i<expressions.size();i++)
	{
		std::string temp1,current_common_name(common_name);
		int2str(origin_index[i],temp1);
		current_common_name.append(temp1);

		fun_names[i].resize(expressions[i].size());
		for(int j=0;j<expressions[i].size();++j)
		{
			int len2=0;
			for(int num=expressions[i][j].size();num!=0;num=num/10)++len2;

			fun_names[i][j].resize(expressions[i][j].size());
			for(int k=0;k<expressions[i][j].size();++k)
			{
				fun_names[i][j][k].append(current_common_name);
				fun_names[i][j][k].append(mutant_types[i][j]);
				std::string temp2;
				int2str(k+1,temp2,len2);
				fun_names[i][j][k].append(temp2);
			}
		}
	}

	//转化为cpp函数并写入指定的一个文件
	CppLibFileCreater lib_creater;
	for(int i=0;i<all_cpp_mutants.size();++i)
	{
		for(int j=0;j<all_cpp_mutants[i].size();++j)
		{
			std::vector<std::vector<char> > args_temp(expressions[i][j].size(),all_argument_list[i]);

			//以指定方式写入文件
			if((i==0 && j==0) && !(i==all_cpp_mutants.size()-1 && j==all_cpp_mutants[i].size()-1))
			{
				lib_creater.run(fun_names[i][j],args_temp,all_cpp_mutants[i][j],CppLibFileBegin,filename);
			}
			else if(!(i==0 && j==0) && (i==all_cpp_mutants.size()-1 && j==all_cpp_mutants[i].size()-1))
			{
				lib_creater.run(fun_names[i][j],args_temp,all_cpp_mutants[i][j],CppLibFileEnd,filename);
			}
			else if((i==0 && j==0) && (i==all_cpp_mutants.size()-1 && j==all_cpp_mutants[i].size()-1))
			{
				lib_creater.run(fun_names[i][j],args_temp,all_cpp_mutants[i][j],CppLibFileBoth,filename);
			}
			else //!(i==0 && j==0) && !(i==all_cpp_mutants.size()-1 && j==all_cpp_mutants[i].size()-1)
			{
				lib_creater.run(fun_names[i][j],args_temp,all_cpp_mutants[i][j],CppLibFileNone,filename);
			}
		}
	}
}
void CreateAllArglist(const std::vector<std::string> &expressions,
					  std::vector<std::vector<char> > &all_arglist)
{
	all_arglist.resize(expressions.size());
	for(int i=0;i<expressions.size();++i)
	{
		for(int j=0;j<expressions[i].size();++j)
		{
			if(expressions[i][j]>='a' && expressions[i][j]<='z')
			{
				for(int k=0;k<all_arglist[i].size();++k)
					if(expressions[i][j]==all_arglist[i][k])
						break;
				if(k==all_arglist[i].size())
					all_arglist[i].push_back(expressions[i][j]);
			}
		}
		sort(all_arglist[i].begin(),all_arglist[i].end());
	}
}