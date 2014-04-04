#include "ExprLibFileHandler.h"
#include "ExpProgramCreater.h"

void main(int argc, char **argv)
{
	//参数预处理
	//例如: CreateExp_GetAllFailTests TCAS.txt TCASMutant.txt TCAS.h TCASMutant.h exper.cpp
	if(argc!=6)return;
	char *origin_expr_file=argv[1];		//参数1:存放原始布尔表达式的文件
	char *mutant_expr_file=argv[2];		//参数2:存放变异布尔表达式的文件

	char *origin_expr_cpp_file=argv[3];	//参数3:存放原始布尔表达式的.h文件
	char *mutant_expr_cpp_file=argv[4];	//参数4:存放变异布尔表达式的.h文件

	char *program_src_file=argv[5];		//参数5:生成实验运行的.cpp文件

	//FOR DEBUG
	/*char *origin_expr_file="../Temp/TCAS.txt";
	char *mutant_expr_file="../Temp/TCASMutant.txt";
	char *origin_expr_cpp_file="../Temp/TCAS.h";
	char *mutant_expr_cpp_file="../Temp/TCASMutant.h";
	char *program_src_file="../Temp/AFT_exp.cpp";*/

	/*---------------------------------------------------------------*/

	std::vector<std::string> origin_fun_names;
	std::vector<std::vector<std::vector<std::string> > > mutant_fun_names;
	std::vector<std::vector<char> > all_argument_list;

	//处理原始布尔表达式, 并生成.h文件
	HandleOrigin(origin_expr_file,origin_expr_cpp_file,all_argument_list,origin_fun_names);

	//处理变异布尔表达式, 并生成.h文件
	HandleMutant(mutant_expr_file,mutant_expr_cpp_file,all_argument_list,mutant_fun_names);

	//生成实验程序源代码
	ExpProgramCreater prog_creater;
	prog_creater.CreateProgram(origin_fun_names,mutant_fun_names,all_argument_list,
							   origin_expr_cpp_file,mutant_expr_cpp_file,
							   program_src_file);
}