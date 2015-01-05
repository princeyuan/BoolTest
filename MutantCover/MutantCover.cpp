#include <list>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>

#include "../SolutionBase/AssistMethod.h"
//using GetAllFileList(), getBinaryTestSuiteFromFile(), 

void main(int argc, char* argv[])
{
    //Handle arguments
    //e.g.: MutantCover TCAS2 ASF/ALL Int_FailTests/ input_tests.txt
    if(argc<5)return;
    std::string expr_name(argv[1]);
    std::string fail_tests_dir(argv[3]);
    std::string input_tests_file(argv[4]);

    std::vector<std::string> mutant_types;
    if(strncmp(argv[2],"ALL",3)==0)
    {
        char types[10][4]={"MLF","LNF","SA0","SA1","LRF","CCF","CDF","ASF","ORF","ENF"};
        for(int type_index=0;type_index<10;++type_index)
            mutant_types.push_back(std::string(types[type_index]));
    }
    else
        mutant_types.push_back(argv[2]);

    bool is_exist;
    std::string temp_filename=expr_name+std::string("_TempFileList.txt");
    std::ifstream infile_temp(temp_filename.c_str());
    if(!infile_temp)
    {
        is_exist=false; //not exist TempFileList.txt
    }
    else
    {
        is_exist=true;  //exist TempFileList.txt
    }

    std::list<std::string> allfilelist;
    if(is_exist)
    {
        GetAllFileList(temp_filename.c_str(),allfilelist);
    }
    else
    {
        GetAllFileList((fail_tests_dir+std::string("AllFileList.txt")).c_str(),allfilelist);
    }

    //compute kill number
    std::list<std::vector<bool> > input_tests;
    getBinaryTestSuiteFromFile(input_tests_file,input_tests);

    for(int type_index=0;type_index<mutant_types.size();++type_index)
    {
        int kill_number=0;
        int mutant_number=0;
        std::string tag=expr_name+mutant_types[type_index];

        std::list<std::string>::iterator itf=allfilelist.begin();
        while(itf!=allfilelist.end())
        {    
            int i=itf->size()-1;
            for(;i>=0;--i)
                if((*itf)[i]=='/')
                    break;
            std::string real_file_name(itf->begin()+i+1,itf->end());

            if(strncmp(real_file_name.c_str(),tag.c_str(),tag.size())==0)
            {
                ++mutant_number;

                std::list<std::vector<bool> > current_kill_tests;
                getBinaryTestSuiteFromFile(*itf,current_kill_tests);

                std::list<std::vector<bool> >::const_iterator itt=input_tests.begin();
                for(;itt!=input_tests.end();++itt)
                {
                    if(find(current_kill_tests.begin(),current_kill_tests.end(),*itt)!=current_kill_tests.end())
                    {
                        ++kill_number;
                        itf=allfilelist.erase(itf);
                        break;
                    }
                }
                if(itt==input_tests.end())
                {
                    ++itf;
                }
            }
            else
            {
                ++itf;
            }
        }

        std::cout<<" Number of total "<<mutant_types[type_index]<<" mutants of "<<expr_name<<" : "<<mutant_number<<'\n';
        std::cout<<" Number of killed "<<mutant_types[type_index]<<" mutants of "<<expr_name<<" : "<<kill_number<<'\n';
        std::cout<<" Number of left "<<mutant_types[type_index]<<" mutants of "<<expr_name<<" : "<<mutant_number-kill_number<<'\n';
    }

    //save mutants that have not been killed
    if(!allfilelist.empty())
    {
        std::ofstream outfile_list(temp_filename.c_str());
        for(std::list<std::string>::const_iterator it=allfilelist.begin();
            it!=allfilelist.end();++it)
        {
            outfile_list<<*it<<'\n';
        }
        outfile_list.close();
    }
    else
    {
        std::ifstream exist_test(temp_filename.c_str());
        if(exist_test)
        {
            exist_test.close();
            remove(temp_filename.c_str());
        }    
    }
}

//CreateBoolMutant
//CreateExp_GetAllFailTests
//之后，读AllFileList.txt文件，获得非等价变异的数量
//运行时，若不存在TempFileList.txt文件，拷贝AllFileList.txt文件，建立临时文件TempFileList.txt
//运行时，若存在TempFileList.txt文件，且文件为空，删除之
//每次运行时，针对TempFileList.txt文件中描述的非等价变异，计算所给测试用例集杀死变异的数量