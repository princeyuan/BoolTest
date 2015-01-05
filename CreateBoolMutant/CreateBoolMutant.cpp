#include <fstream>
#include <iostream>
#include "MuCreaterBuilder.h"
//#include "../SolutionBase/OriginExprFileIO.h"
//#include "../SolutionBase/MutantExprFileIO.h"
#include "ExpressionFileIO.h"

int main(int argc, char **argv)
{
    //帮助CreateBoolMutant /?
    if(argc==2 && strcmp("/?",argv[1])==0)
    {
        std::cout<<"CreateBoolMutant file_origin_expressions file_mutant_types file_mutant_expressions [-disp]\n";
        std::cout<<"\tfile_origin_expressions: file contains origin boolean expressions, one expression per line\n";
        std::cout<<"\tfile_mutant_types: file contains mutant types, one mutant type per line\n";
        std::cout<<"\tfile_mutant_expressions: output file contains mutant boolean expressions\n";
        std::cout<<"\t[-disp]: display context of output file\n";
        return 0;
    }
    
    //正常使用, 例如: CreateBoolMutant TCAS.txt FaultType.ini TCASMutant.txt
    //参数预处理
    if(argc!=4 && argc!=5)
    {
        std::cout<<"Error arguments!";
        return 0;
    }
    char *origin_expr_file=argv[1];//参数1:存放原始布尔表达式的文件
    char *fault_types_file=argv[2];//参数2:指定所需变异类型的文件
    char *mutant_expr_file=argv[3];//参数3:存放所得变异表达式的文件

    bool is_display=false;
    if(argc==5 && strcmp("-disp",argv[4])==0)//参数4:决定是否在屏幕上输出
        is_display=true;

    /*---------------------------------------------------------------*/

    //从origin_expr_file文件中读入所有表达式
    std::vector<std::string> expressions;
    try
    {
        ReadOriginExprFile(origin_expr_file,expressions);
    }
    catch(...)
    {
        std::cout<<"Error when reading file "<<origin_expr_file;
        return 0;
    }

    //从fault_types_file文件中读入指定的变异类型
    std::vector<std::string> mutant_types;
    std::ifstream infile2;
    infile2.open(fault_types_file);
    if(!infile2)
    {
        std::cout<<"Error when reading file "<<fault_types_file<<'\n';
        return 0;
    }
    try
    {
        char buffer[8];
        while(infile2.getline(buffer,8,'\n'))
            mutant_types.push_back(buffer);
    }
    catch(...)//接受读取原始布尔表达式文件时抛出的"Fail to Open File!"或"Fail to Read File!"异常
    {
        infile2.close();
        std::cout<<"Error when reading file "<<fault_types_file<<'\n';
        return 0;
    }
    infile2.close();

    //根据指定的变异类型创建相应的变异生成器
    MutantCreaterBuilder builder;
    std::vector<MutantCreater *> mutant_creaters(mutant_types.size());
    for(int i=0;i<mutant_creaters.size();i++)
    {
        mutant_creaters[i]=builder.Build(mutant_types[i]);
    }

    std::vector<std::vector<std::vector<std::string> > > all_mutants(expressions.size());
    //对每一个表达式, 生成所有指定类型的变异表达式
    for(int i=0;i<expressions.size();i++)
    {
        try{
            all_mutants[i].resize(mutant_creaters.size());
            //该循环每次生成一种指定类型的变异表达式
            for(int j=0;j<mutant_creaters.size();j++)
            {
                if(mutant_creaters[j]!=NULL)
                {
                    mutant_creaters[j]->CreateMutants(expressions[i],all_mutants[i][j]);
                }
            }

            if(is_display)
            {
                std::cout<<"The "<<i+1<<"-th original expression: "<<expressions[i]<<'\n';
                std::cout<<"The mutants of the "<<i+1<<"-th original expression include:\n";
                for(int j=0;j<all_mutants[i].size();j++)
                {
                    std::cout<<"Mutation type = "<<mutant_types[j]<<" :\n";
                    for(int k=0;k<all_mutants[i][j].size();++k)
                        std::cout<<all_mutants[i][j][k]<<'\n';
                }
            }
        }
        catch(char *exception_info)//接受变异生成器抛出的"Boolean Expression Solecism!"异常
        {
            all_mutants[i].clear();
            std::cout<<exception_info<<"in "<<i+1<<"-th expression!\n";
        }
    }

    //释放变异生成器资源
    for(int i=0;i<mutant_creaters.size();i++)
    {
        delete mutant_creaters[i];
    }

    //将变异表达式写入输出文件mutant_expr_file
    try
    {
        WriteMutantExprFile(mutant_expr_file,origin_expr_file,expressions,mutant_types,all_mutants);
    }
    catch(...)//接受生成及写入变异布尔表达式文件时抛出的"Fail to Create File!"或"Fail to Write File!"异常
    {
        std::cout<<"Error when creating file "<<mutant_expr_file;
        return 0;
    }
}