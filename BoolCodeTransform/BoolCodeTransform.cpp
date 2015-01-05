#include <set>
#include <iostream>
#include <algorithm>

#include "CodeFileCreater.h"
#include "../CreateBoolMutant/ExpressionFileIO.h"
//using ReadOriginExprFile() and ReadMutantExprFile()

void ExtractInputVars(const std::string &expression,
                      std::vector<std::string> &input_vars);

void BoolTranslate(const std::string &source,std::string &destination);

void int2str(int number,std::string &str);

void WriteOriginCodeFile(const std::string &common_name,
                         const std::vector<std::string> &expressions,
                         const std::vector<std::vector<std::string> > &all_argument_list,
                         const std::string &filename);

void WriteMutantCodeFile(const std::string &common_name,
                         const std::vector<int> &origin_index,
                         const std::vector<std::string> &origin_exprs,
                         const std::vector<std::vector<std::string> > &mutant_types,
                         const std::vector<std::vector<std::vector<std::string> > > &expressions,
                         const std::vector<std::vector<std::string> > &all_argument_list,
                         const std::string &filename);

int main(int argc, char* argv[])
{
    //帮助BoolCodeTransform /?
    if(argc==2 && strcmp("/?",argv[1])==0)
    {
        std::cout<<"BoolCodeTransform file_expressions file_expression_codes [-mu]\n";
        std::cout<<"\tfile_expressions: file contains origin (or mutant) boolean expressions\n";
        std::cout<<"\tfile_expression_codes: file contains boolean expression codes\n";
        std::cout<<"\t[-mu]: handle mutant expressions\n";
        return 0;
    }
    
    //正常使用, 例如: BoolCodeTransform TCAS.txt TCAS.h 或 BoolCodeTransform TCASMu.txt TCASMu.h -mu
    //参数预处理
    if(argc!=3 && argc!=4)
    {
        std::cout<<"Error arguments!";
        return 0;
    }
    char *expr_file=argv[1];    //参数1:存放布尔表达式的文件
    char *code_file=argv[2];    //参数2:存放布尔表达式的代码文件
    
    bool is_mutant_file=false;
    if(argc==4 && strcmp("-mu",argv[3])==0)//参数3:决定转换的是原始布尔表达式文件还是变异布尔表达式文件
        is_mutant_file=true;

    /*---------------------------------------------------------------*/

    if(!is_mutant_file)
    {//Transform origin boolean expression file
        std::string common_name;
        std::vector<std::string> expressions;
        std::vector<std::vector<std::string> > all_argument_list;

        try
        {
            ReadOriginExprFile(expr_file,expressions);
            ExtractPureFilenameNoExtension(expr_file,common_name);
        }
        catch(...)
        {
            std::cout<<"Error when reading file "<<expr_file;
            return 0;
        }

        all_argument_list.resize(expressions.size());
        for(int i=0;i<expressions.size();++i)
        {
            ExtractInputVars(expressions[i],all_argument_list[i]);
        }

        try{
            WriteOriginCodeFile(common_name,expressions,all_argument_list,code_file);
        }
        catch(...)
        {
            std::cout<<"Error when creating file "<<code_file;
            return 0;
        }
    }
    else
    {//Transform mutant boolean expression file
        std::string common_name;
        std::string origin_expr_file_fullname;
        std::vector<int> origin_index;
        std::vector<std::string> origin_exprs;
        std::vector<std::vector<std::string> > mutant_types;
        std::vector<std::vector<std::vector<std::string> > > expressions;
        std::vector<std::vector<std::string> > all_argument_list;

        try{
            ReadMutantExprFile(expr_file,origin_expr_file_fullname,
                origin_index,origin_exprs,mutant_types,expressions);
            ExtractPureFilenameNoExtension(origin_expr_file_fullname,common_name);
        }
        catch(...)
        {
            std::cout<<"Error when reading file "<<expr_file;
            return 0;
        }

        all_argument_list.resize(origin_exprs.size());
        for(int i=0;i<expressions.size();++i)
        {
            ExtractInputVars(origin_exprs[i],all_argument_list[i]);
        }

        try
        {
            WriteMutantCodeFile(common_name,origin_index,origin_exprs,mutant_types,expressions,
                all_argument_list,code_file);
        }
        catch(...)
        {
            std::cout<<"Error when creating file "<<code_file;
            return 0;
        }
    }

    return 0;
}

void ExtractInputVars(const std::string &expression,
                      std::vector<std::string> &input_vars)
{
    std::set<std::string> temp_input_vars;
    for(int i=0;i<expression.size();++i)
    {
        if((expression[i]>='a' && expression[i]<='z') || 
            (expression[i]>='A' && expression[i]<='Z'))
        {
            int cur=i+1;
            while(cur<=expression.size()-1 && 
                  expression[cur]!='!' && 
                  expression[cur]!='+' && 
                  expression[cur]!='*' &&
                  expression[cur]!='(' &&
                  expression[cur]!=')')
                ++cur;

            //check whether the variable has been contained
            int j;
            for(j=0;j<input_vars.size();j++)
            {
                if(((cur-i)==input_vars[j].length()) && 
                    (expression.compare(i,cur-i,input_vars[j])==0))
                    break;
            }
            //if has not been contained, add it in
            if(j==input_vars.size())
            {
                temp_input_vars.insert(expression.substr(i,cur-i));
            }
        }
    }
    input_vars.assign(temp_input_vars.begin(),temp_input_vars.end());
    sort(input_vars.begin(),input_vars.end());
}

void BoolTranslate(const std::string &source,std::string &destination)
{
    destination=source;
    
    for(int i=0;i<destination.size();i++)
    {
        if(destination[i]=='*')
        {
            destination.replace(i,1,2,'&');
        }
        if(destination[i]=='+')
        {
            destination.replace(i,1,2,'|');
        }
    }
}

void int2str(int number,std::string &str)
{
    str.clear();
    while(number!=0)
    {
        str.insert(str.begin(),'0'+(number%10));
        number=number/10;
    }
}

void WriteOriginCodeFile(const std::string &common_name,
                         const std::vector<std::string> &expressions,
                         const std::vector<std::vector<std::string> > &all_argument_list,
                         const std::string &filename)
{
    //转化为C格式的表达式
    std::vector<std::string> all_code_exprs(expressions.size());
    for(int i=0;i<expressions.size();i++)
    {
        BoolTranslate(expressions[i],all_code_exprs[i]);
    }

    //批量生成函数名
    std::vector<std::string> fun_names(expressions.size(),common_name);
    for(int i=0;i<expressions.size();i++)
    {
        std::string temp;
        int2str(i+1,temp);
        fun_names[i].append(temp);
    }

    //转化为C函数并写入指定的一个文件
    CodeFileCreater code_file_creater;
    code_file_creater.run(fun_names,all_argument_list,all_code_exprs,CodeFileBoth,filename);
}

void WriteMutantCodeFile(const std::string &common_name,
                         const std::vector<int> &origin_index,
                         const std::vector<std::string> &origin_exprs,
                         const std::vector<std::vector<std::string> > &mutant_types,
                         const std::vector<std::vector<std::vector<std::string> > > &expressions,
                         const std::vector<std::vector<std::string> > &all_argument_list,
                         const std::string &filename)
{
    //转化为C格式的表达式
    std::vector<std::vector<std::vector<std::string> > > all_code_mutants(expressions.size());

    for(int i=0;i<expressions.size();i++)
    {
        all_code_mutants[i].resize(expressions[i].size());
        for(int j=0;j<expressions[i].size();j++)
        {
            all_code_mutants[i][j].resize(expressions[i][j].size());
            for(int k=0;k<expressions[i][j].size();k++)
            {
                BoolTranslate(expressions[i][j][k],all_code_mutants[i][j][k]);
            }
        }
    }

    //批量生成函数名
    std::vector<std::vector<std::vector<std::string> > > fun_names(expressions.size());
    for(int i=0;i<expressions.size();i++)
    {
        std::string temp1,current_common_name(common_name);
        int2str(origin_index[i],temp1);
        current_common_name.append(temp1);

        fun_names[i].resize(expressions[i].size());
        for(int j=0;j<expressions[i].size();++j)
        {
            fun_names[i][j].resize(expressions[i][j].size());
            for(int k=0;k<expressions[i][j].size();++k)
            {
                fun_names[i][j][k].append(current_common_name);
                fun_names[i][j][k].append(mutant_types[i][j]);
                std::string temp2;
                int2str(k+1,temp2);
                fun_names[i][j][k].append(temp2);
            }
        }
    }

    //转化为C函数并写入指定的一个文件
    CodeFileCreater  code_file_creater;
    for(int i=0;i<all_code_mutants.size();++i)
    {
        for(int j=0;j<all_code_mutants[i].size();++j)
        {
            std::vector<std::vector<std::string> > args_temp(expressions[i][j].size(),all_argument_list[i]);

            //以指定方式写入文件
            if((i==0 && j==0) && !(i==all_code_mutants.size()-1 && j==all_code_mutants[i].size()-1))
            {
                code_file_creater.run(fun_names[i][j],args_temp,
                    all_code_mutants[i][j],CodeFileBegin,filename);
            }
            else if(!(i==0 && j==0) && (i==all_code_mutants.size()-1 && j==all_code_mutants[i].size()-1))
            {
                code_file_creater.run(fun_names[i][j],args_temp,
                    all_code_mutants[i][j],CodeFileEnd,filename);
            }
            else if((i==0 && j==0) && (i==all_code_mutants.size()-1 && j==all_code_mutants[i].size()-1))
            {
                code_file_creater.run(fun_names[i][j],args_temp,
                    all_code_mutants[i][j],CodeFileBoth,filename);
            }
            else //!(i==0 && j==0) && !(i==all_code_mutants.size()-1 && j==all_code_mutants[i].size()-1)
            {
                code_file_creater.run(fun_names[i][j],args_temp,
                    all_code_mutants[i][j],CodeFileNone,filename);
            }
        }
    }
}
