#ifndef MUTANT_EXPR_FILE_IO_H
#define MUTANT_EXPR_FILE_IO_H

#include <vector>
#include <string>

void ReadMutantExprFile(const std::string &expr_file,
						std::string &expr_common_name,
						std::vector<int> &origin_index,
						std::vector<std::string> &origin_exprs,
						std::vector<std::vector<std::string> > &mutant_types,
						std::vector<std::vector<std::vector<std::string> > > &expressions);

void WriteMutantExprFile(const std::string &mutant_expr_file,
						 const std::string &origin_expr_file,
						 const std::vector<std::string> &origin_exprs,
						 const std::vector<std::string> &mutant_types,
						 const std::vector<std::vector<std::vector<std::string> > > &expressions);

#endif