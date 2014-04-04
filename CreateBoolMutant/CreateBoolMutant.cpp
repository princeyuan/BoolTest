#include <fstream>
#include <iostream>
#include "AllCreater.h"
#include "../SolutionBase/OriginExprFileIO.h"
#include "../SolutionBase/MutantExprFileIO.h"

void main(int argc, char **argv)
{
	//参数预处理
	//例如: CreateBoolMutant TCAS.txt FaultType.ini TCASMutant.txt
	if(argc!=4 && argc!=5)return;
	char *origin_expr_file=argv[1];//参数1:存放原始布尔表达式的文件
	char *fault_types_file=argv[2];//参数2:指定所需变异类型的文件
	char *mutant_expr_file=argv[3];//参数3:存放所得变异表达式的文件

	bool is_display=false;
	if(argc==5 && strcmp("-d",argv[4])==0)
		is_display=true;

	//FOR DEBUG
	/*char *origin_expr_file="../Temp/TCAS.txt";
	char *fault_types_file="../Temp/FaultType.ini";
	char *mutant_expr_file="../Temp/TCASMutant.txt";*/

	/*---------------------------------------------------------------*/

	//从origin_expr_file文件中读入所有表达式
	std::string expr_common_name;
	std::vector<std::string> expressions;
	ReadOriginExprFile(origin_expr_file,expr_common_name,expressions);

	//从fault_types_file文件中读入指定的变异类型
	std::vector<std::string> types;
	std::ifstream infile2;
	infile2.open(fault_types_file);
	if(!infile2)
	{
		std::cout<<"Error when reading file "<<fault_types_file;
		return;
	}
	try
	{
		char buffer[1024];
		while(infile2.getline(buffer,1024,'\n'))
			types.push_back(buffer);
	}
	catch (...)
	{
		infile2.close();
		std::cout<<"Error when reading file "<<fault_types_file;
		return;
	}
	infile2.close();

	//对每一个表达式，生成所有指定类型的变异表达式
	std::vector<std::vector<std::vector<std::string> > > all_mutants(expressions.size());
	for(int i=0;i<expressions.size();i++)
	{
		AllCreater creater;
		creater.SetMutationType(types);
		creater.CreateGivenClassMutants(expressions[i],all_mutants[i]);

		if(is_display)
		{
			std::cout<<"The "<<i+1<<"-th original expression: "<<expressions[i]<<'\n';
			std::cout<<"The mutants of the "<<i+1<<"-th original expression include:\n";
			for(int j=0;j<all_mutants[i].size();j++)
			{
				std::cout<<"Mutation type = "<<types[j]<<" :\n";
				for(int k=0;k<all_mutants[i][j].size();++k)
					std::cout<<all_mutants[i][j][k]<<'\n';
			}
		}
	}

	//将变异表达式写入输出文件mutant_expr_file
	WriteMutantExprFile(mutant_expr_file,origin_expr_file,expressions,types,all_mutants);
}