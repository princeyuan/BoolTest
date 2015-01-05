#include <list>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "ATF_ProgramCreater.h"

void ExtractFunction(const char *code_file,
                     std::vector<std::string> &fun_names,
                     std::vector<int> &argument_number_list);


void CountArgsNumberFromFunSignature(const std::string &function_signature,
                                     int &argument_number);

void ClassifyMutantFunNames(const std::vector<std::string> &origin_fun_names,
                            const std::vector<std::string> &mutant_fun_names,
                            std::vector<std::vector<std::string> > &classified_fun_names);

int main(int argc, char* argv[])
{
    //帮助CreateProgram /?
    if(argc==2 && strcmp("/?",argv[1])==0)
    {
        std::cout<<"CreateProgram codefile_origin codefile_mutant program_file [-WO]|[-W]|[-LO]|[-L]\n";
        std::cout<<"\tcodefile_origin: code file contains origin boolean expression codes\n";
        std::cout<<"\tcodefile_mutant: code file contains mutant boolean expression codes\n";
        std::cout<<"\tprogram_file: C program to get all fail test cases\n";
        std::cout<<"\t[-WO]|[-W]|[-LO]|[-L]: 'W' means Windows, 'L' means Linux, 'O' means storing oracle\n";
        return 0;
    }
    
    //正常使用, 例如: CreateProgram TCAS.h TCASMu.h GetAllFailTests_TCAS.cpp
    //参数预处理
    if(argc<4)
    {
        std::cout<<"Error arguments!";
        return 0;
    }
    char *origin_code_file=argv[1]; //参数1:存放原始布尔表达式代码的文件
    char *mutant_code_file=argv[2]; //参数2:存放变异布尔表达式代码的文件
    char *program_file=argv[3];     //参数3:生成的程序文件

    bool is_win_sys=true;
    bool is_oracle=false;
    if(argc==5)
    {
        if(strcmp("-LO",argv[4])==0 || strcmp("-L",argv[4])==0)
            is_win_sys=false;
        if(strcmp("-WO",argv[4])==0 || strcmp("-LO",argv[4])==0)
            is_oracle=true;
    }

    /*---------------------------------------------------------------*/

    std::vector<std::string> origin_fun_names;
    std::vector<std::string> mutant_fun_names;
    std::vector<int> origin_argument_number_list;
    std::vector<int> mutant_argument_number_list;//no use, only 
    std::vector<std::vector<std::string> > classified_mutant_fun_names;

    try
    {
        ExtractFunction(origin_code_file,origin_fun_names,origin_argument_number_list);
    }
    catch(...)
    {
        std::cout<<"Error when reading file "<<origin_code_file;
        return 0;
    }

    try
    {
        ExtractFunction(mutant_code_file,mutant_fun_names,mutant_argument_number_list);
    }
    catch(...)
    {
        std::cout<<"Error when reading file "<<mutant_code_file;
        return 0;
    }

    ClassifyMutantFunNames(origin_fun_names,mutant_fun_names,classified_mutant_fun_names);

    try
    {
        GAFT_ProgramCreater pro_creater(is_win_sys);

        if(!is_oracle)
            pro_creater.run(origin_fun_names,
                            classified_mutant_fun_names,
                            origin_argument_number_list,
                            origin_code_file,
                            mutant_code_file,
                            program_file);
        else
            pro_creater.run_o(origin_fun_names,
                              classified_mutant_fun_names,
                              origin_argument_number_list,
                              origin_code_file,
                              mutant_code_file,
                              program_file);
    }
    catch(...)
    {
        std::cout<<"Error when creating file "<<program_file;
        return 0;
    }

    return 0;
}

void ExtractFunction(const char *code_file,
                     std::vector<std::string> &fun_names,
                     std::vector<int> &argument_number_list)
{
    std::ifstream infile;
    infile.open(code_file);
    if(!infile)
    {
        throw "Fail to Open File!";
    }

    try
    {
        char buffer[1024];
        std::list<int> temp_arg_num_list;
        std::list<std::string> temp_fun_names;
        while(infile.getline(buffer,1024,'\n'))
        {
            if(strlen(buffer)!=0 && strncmp("bool",buffer,4)==0)
            {
                //get function name
                std::string fun_signature(buffer);
                temp_fun_names.push_back(fun_signature.substr(5, //since strlen("bool ") is 5 
                                                              fun_signature.find('(')-5));

                //get number of arguments
                int arg_number=0;
                CountArgsNumberFromFunSignature(fun_signature,arg_number);
                temp_arg_num_list.push_back(arg_number);
            }
        }
        fun_names.assign(temp_fun_names.begin(),temp_fun_names.end());
        argument_number_list.assign(temp_arg_num_list.begin(),temp_arg_num_list.end());
    }
    catch(...)
    {
        infile.close();
        throw "Fail to Read File!";
    }
}

void CountArgsNumberFromFunSignature(const std::string &function_signature,
                                     int &argument_number)
{
    int cur=0,count=0;
    int find_pos;
    while((find_pos=function_signature.find("bool ",cur))!=function_signature.npos)
    {
        ++count;
        cur=find_pos+5;
    }
    argument_number=count-1;
}

void ClassifyMutantFunNames(const std::vector<std::string> &origin_fun_names,
                            const std::vector<std::string> &mutant_fun_names,
                            std::vector<std::vector<std::string> > &classified_fun_names)
{
    classified_fun_names.resize(origin_fun_names.size());

    std::vector<std::string>::const_iterator it_head=mutant_fun_names.begin();
    std::vector<std::string>::const_iterator it_tail=mutant_fun_names.begin();

    for(int i=0;i<origin_fun_names.size();++i)
    {
        while(it_tail!=mutant_fun_names.end())
        {
            int pos=it_tail->find(origin_fun_names[i]);
            if(pos!=0)
            {
                classified_fun_names[i].assign(it_head,it_tail);
                it_head=it_tail;
                break;
            }
            else
            {
                ++it_tail;
            }
        }

        if(it_tail==mutant_fun_names.end())
        {
            classified_fun_names[i].assign(it_head,it_tail);
        }
    }
}
