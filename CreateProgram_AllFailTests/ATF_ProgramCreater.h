#ifndef GAFT_PROGRAM_CREATER_H
#define GAFT_PROGRAM_CREATER_H

#include <vector>
#include <string>

class GAFT_ProgramCreater
{
public:

    GAFT_ProgramCreater(bool is_win_sys=true)
        :m_is_win_sys(is_win_sys)
    {};

    void run(const std::vector<std::string> &origin_fun_names,
             const std::vector<std::vector<std::string> > &mutant_fun_names,
             const std::vector<int> &argument_number_list,
             const char *origin_code_file,
             const char *mutant_code_file,
             const char *program_src_file);

    void run_o(const std::vector<std::string> &origin_fun_names,
             const std::vector<std::vector<std::string> > &mutant_fun_names,
             const std::vector<int> &argument_number_list,
             const char *origin_code_file,
             const char *mutant_code_file,
             const char *program_src_file);

private:

    void GAFT_ProgramCreater::ExtractPureFilenameWithExtension(
        const std::string &filename,
        std::string &pure_filename);

private:

    bool m_is_win_sys;
};

#endif