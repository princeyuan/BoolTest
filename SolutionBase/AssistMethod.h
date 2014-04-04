#ifndef BOOL_METHOD_H
#define BOOL_METHOD_H

#include <set>
#include <list>
#include <string>
#include <vector>
#include <fstream>
#include <math.h>

bool next_binary(std::vector<bool> &test);

bool next_Combination(std::vector<int> &combination,int num_elements);

int getOneBinaryTestFromLine(char* str,std::vector<bool> &test);

bool getBinaryTestSuiteFromFile(const std::string &file,std::list<std::vector<bool> > &tests);

char* getOneIntFromLine(char *str,int &num);

int getAllIntsFromLine(char* str,std::vector<int> &nums);

void int2str(int number,std::string &str,int len);

void int2str(int number,std::string &str);

void GetAllFileList(const char *allfilelist_file_name,std::list<std::string> &allfilelist);

void getRealFileName(const std::string &full_name,std::string &real_name,std::string &cur_dir);

#endif