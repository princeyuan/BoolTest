#ifndef CPP_LIB_FILE_CREATER_H
#define CPP_LIB_FILE_CREATER_H

#include <string>
#include <vector>

enum WriteCppLibFileState
{
	CppLibFileBegin,CppLibFileEnd,CppLibFileBoth,CppLibFileNone
};

class CppLibFileCreater
{
public:

	void run(const std::vector<std::string> &fun_names,
			 const std::vector<std::vector<char> > &all_args,
			 const std::vector<std::string> &expressions,
			 const WriteCppLibFileState &state,
			 const std::string &filename);

private:
	
	void CreateSourceCode(const std::vector<std::string> &fun_names,  
						  const std::vector<std::vector<char> > &all_args,
						  const std::vector<std::string> &expressions,
						  std::string &sourcecode);

	void CreateSourceFile(const std::string &sourcecode,
						  const WriteCppLibFileState &state,
						  const std::string &filename);

	void CreateFunName(const std::string &name,std::string &code);

	void CreateArglist(const std::vector<char> &arguments,std::string &code);

	void CreateFunBody(const std::string &expression,std::string &code);

	void CreateHeadMicro(const std::string &filename,std::string &head_micro);

};

#endif