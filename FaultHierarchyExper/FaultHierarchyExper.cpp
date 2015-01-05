#include <direct.h>

#include "Exper_AnyTwo.h"
#include "Exper_SelfTest.h"
#include "Exper_KeyReduce.h"
#include "Exper_GivenTwoGroup.h"

void main(int argc, char* argv[])
{
    if(argc<7)return;

    //Handle arguments 2 - 6
    //e.g.: FaultHierarchyExper -x TCAS 1 - 20 Int/
    std::string common_expr_name(argv[2]);
    if(0!=strcmp(argv[4],"-"))return;
    int expr_index_begin=atoi(argv[3]);
    int expr_index_end=atoi(argv[5]);
    std::string tests_file_dir(argv[6]);

    //
    if(argc==7 && !strcmp(argv[1],"-t"))
    {
        Exper_SelfTest(common_expr_name,expr_index_begin,expr_index_end,tests_file_dir);
    }
    else if((argc==7 || argc==8) && !strcmp(argv[1],"-a"))
    {
        if(argc==7)
        {
            if(_mkdir("Output"))
            {
                std::cout<<"Error when create direct : "<<"Output"<<'\n';
                return;
            }
            Exper_AnyTwo(common_expr_name,expr_index_begin,expr_index_end,tests_file_dir,"Output/");
        }
        else if(argc==8)
        {
            if(_mkdir(argv[7]))
            {
                std::cout<<"Error when create direct : "<<argv[7]<<'\n';
                return;
            }
            Exper_AnyTwo(common_expr_name,expr_index_begin,expr_index_end,tests_file_dir,argv[7]);
        }
    }
    else if((argc==7 || argc==8) && !strcmp(argv[1],"-r"))
    {
        if(argc==7)
        {
            if(_mkdir("Output"))
            {
                std::cout<<"Error when create direct : "<<"Output"<<'\n';
                return;
            }
            Exper_KeyReduce(common_expr_name,expr_index_begin,expr_index_end,tests_file_dir,"Output/");
        }
        if(argc==8)
        {
            if(_mkdir(argv[7]))
            {
                std::cout<<"Error when create direct : "<<argv[7]<<'\n';
                return;
            }
            Exper_KeyReduce(common_expr_name,expr_index_begin,expr_index_end,tests_file_dir,argv[7]);
        }
    }
    else if(argc>=10 && !strcmp(argv[1],"-g"))
    {
        Exper_GivenTwoGroup(common_expr_name,expr_index_begin,expr_index_end,tests_file_dir,argc-7,argv+7);
    }
}