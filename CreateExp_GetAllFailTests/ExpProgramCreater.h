#ifndef PROGRAM_CREATER_H
#define PROGRAM_CREATER_H

#include <vector>
#include <string>

class ExpProgramCreater
{
public:

	void CreateProgram(const std::vector<std::string> &origin_fun_names,
					   const std::vector<std::vector<std::vector<std::string> > > &mutant_fun_names,
					   const std::vector<std::vector<char> > &all_argument_list,
					   const char *origin_expr_cpp_file,
					   const char *mutant_expr_cpp_file,
					   const char *program_src_file);
};

#endif