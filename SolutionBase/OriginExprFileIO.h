#ifndef ORIGIN_EXPR_FILE_IO_H
#define ORIGIN_EXPR_FILE_IO_H

#include <vector>
#include <string>

void ReadOriginExprFile(const std::string &expr_file,
                        std::string &expr_common_name,
                        std::vector<std::string> &expressions);

#endif