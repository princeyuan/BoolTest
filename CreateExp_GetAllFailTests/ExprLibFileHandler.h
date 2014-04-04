#ifndef FILE_HANDER_H
#define FILE_HANDER_H

#include <vector>
#include <string>

//used in main()
void HandleOrigin(const std::string &expr_file,
				  const std::string &cpp_head_file,
				  std::vector<std::vector<char> > &all_argument_list,
				  std::vector<std::string> &fun_names);

void HandleMutant(const std::string &expr_file,
				  const std::string &cpp_head_file,
				  const std::vector<std::vector<char> > &all_argument_list,
				  std::vector<std::vector<std::vector<std::string> > > &fun_names);

//following functions will be used in HandleOrigin()

void WriteOriginCppFile(const std::string &common_name,
						const std::vector<std::string> &expressions,
						const std::vector<std::vector<char> > &all_argument_list,
						const std::string &filename,
						std::vector<std::string> &fun_names);

void WriteMutantCppFile(const std::string &common_name,
						const std::vector<int> &origin_index,
						const std::vector<std::string> &origin_exprs,
						const std::vector<std::vector<std::string> > &mutant_types,
						const std::vector<std::vector<std::vector<std::string> > > &expressions,
						const std::vector<std::vector<char> > &all_argument_list,
						const std::string &filename,
						std::vector<std::vector<std::vector<std::string> > > &fun_names);

void CreateAllArglist(const std::vector<std::string> &expressions,
					  std::vector<std::vector<char> > &all_arglist);

#endif