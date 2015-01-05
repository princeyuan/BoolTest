#ifndef CODE_FILE_CREATER_H
#define CODE_FILE_CREATER_H

#include <string>
#include <vector>

enum WriteCodeFileState
{
    CodeFileBegin,CodeFileEnd,CodeFileBoth,CodeFileNone
};

class CodeFileCreater
{
public:

    void run(const std::vector<std::string> &fun_names,
             const std::vector<std::vector<std::string> > &all_args,
             const std::vector<std::string> &code_expressions,
             const WriteCodeFileState &state,
             const std::string &filename);

private:
    
    void CreateSourceCode(const std::vector<std::string> &fun_names,  
                          const std::vector<std::vector<std::string> > &all_args,
                          const std::vector<std::string> &code_expressions,
                          std::string &sourcecode);

    void CreateSourceFile(const std::string &sourcecode,
                          const WriteCodeFileState &state,
                          const std::string &filename);

    void CreateFunName(const std::string &name,
                       std::string &code);

    void CreateArglist(const std::vector<std::string> &arguments,
                       std::string &code);

    void CreateFunBody(const std::string &expression,
                       std::string &code);

    void CreateHeadMicro(const std::string &filename,
                         std::string &head_micro);

};

#endif