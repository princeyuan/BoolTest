#ifndef EXPRESSION_FILE_IO_H
#define EXPRESSION_FILE_IO_H

#include <vector>
#include <string>
#include <fstream>

//ExtractPureFilenameNoExtension,ReadOriginExprFile,ReadMutantExprFile are useb by BoolCodeTransform
//ReadOriginExprFile,WriteMutantExprFile are used by CreateBoolMutant

//ReadOriginExprFile读取原始布尔表达式文件, 获取所有表达式, 获取原始表达式文件名作为表达式通用名称
//ReadMutantExprFile读取变异布尔表达式文件, 获取表达式通用名称, 表达式编号, 原始布尔表达式, 变异类型, 变异布尔表达式
//WriteMutantExprFile写入变异布尔表达式文件, 写入原始布尔表达式文件名, 原始布尔表达式, 变异类型, 变异布尔表达式

//Extract the pure file name without the path and the file extension
//E.g. a file "E:/ThePath/File.txt" will output "File"
void ExtractPureFilenameNoExtension(const std::string &filename,
                                    std::string &pure_filename)
{
    std::string::const_iterator it_head=filename.begin();
    for(std::string::const_iterator it=filename.begin();
        it!=filename.end();++it)
    {
        if(*it=='/')
            it_head=it+1;
    }
    std::string::const_iterator it_tail=it_head;
    for(;it_tail!=filename.end();++it_tail)
    {
        if(*it_tail=='.')
            break;
    }
    pure_filename.assign(it_head,it_tail);
}

void ReadOriginExprFile(const char *expr_file,
                        std::vector<std::string> &expressions)
{
    std::ifstream infile;
    infile.open(expr_file);
    if(!infile)
    {
        throw "Fail to Open File!";
    }

    try
    {
        char buffer[1024];
        while(infile.getline(buffer,1024,'\n'))
        {
            if(strlen(buffer)!=0)
            {
                expressions.push_back(buffer);
            }
        }
    }
    catch (...)
    {
        infile.close();
        throw "Fail to Read File!";
    }

    infile.close();
}

void ReadMutantExprFile(const char *expr_file,
                        std::string &origin_expr_file_fullname,
                        std::vector<int> &origin_index_lists,
                        std::vector<std::string> &origin_exprs,
                        std::vector<std::vector<std::string> > &mutant_types,
                        std::vector<std::vector<std::vector<std::string> > > &expressions)
{
    std::ifstream infile;
    infile.open(expr_file);
    if(!infile)
    {
        throw "Fail to Open File!";
    }

    std::vector<std::string> one_origin_types;
    std::vector<std::string> one_type_expr;
    std::vector<std::vector<std::string> > one_origin_expr;
    
    try
    {
        char buffer[1024];
        while(infile.getline(buffer,1024,'\n'))
        {
            if(strlen(buffer)!=0)
            {
                if(strncmp("#Original Expression File: ",buffer,strlen("#Original Expression File: "))==0)
                {
                    char *temp=buffer+strlen("#Original Expression File: ");
                    origin_expr_file_fullname=temp;
                }
                else if(strncmp("#Original Expression ",buffer,strlen("#Original Expression "))==0)
                {
                    char *temp=buffer+strlen("#Original Expression ");

                    char *pos=temp;
                    for(;*pos!='\0';++pos)
                    {
                        if(*pos==':')
                        {
                            *pos='\0';
                            break;
                        }
                    }
                    origin_index_lists.push_back(atoi(temp));

                    temp=pos+2;
                    origin_exprs.push_back(std::string(temp));
                }
                else if(strncmp("#End Original Expression",buffer,strlen("#End Original Expression"))==0)
                {
                    expressions.push_back(one_origin_expr);
                    one_origin_expr.clear();

                    mutant_types.push_back(one_origin_types);
                    one_origin_types.clear();
                }
                else if(strncmp("#Mutation Type: ",buffer,strlen("#Mutation Type: "))==0)
                {
                    char *temp=buffer+strlen("#Mutation Type: ");
                    one_origin_types.push_back(temp);
                }
                else if(strncmp("#End Mutation Type",buffer,strlen("#End Mutation Type"))==0)
                {
                    one_origin_expr.push_back(one_type_expr);
                    one_type_expr.clear();
                }
                else if(strncmp("#End File",buffer,strlen("#End File"))==0)
                {
                    break;
                }
                else
                {
                    one_type_expr.push_back(buffer);
                }
            }
        }
    }
    catch (...)
    {
        infile.close();
        throw "Fail to Read File!";
    }

    infile.close();
}

void WriteMutantExprFile(const char *mutant_expr_file,
                         const char *origin_expr_file,
                         const std::vector<std::string> &origin_exprs,
                         const std::vector<std::string> &mutant_types,
                         const std::vector<std::vector<std::vector<std::string> > > &expressions)
{
    std::ofstream outfile;
    outfile.open(mutant_expr_file);
    if(!outfile)
    {
        throw "Fail to Create File!";
    }

    try
    {
        outfile<<"#Original Expression File: "<<origin_expr_file<<"\n\n";
        for(int i=0;i<expressions.size();i++)
        {
            outfile<<"#Original Expression "<<i+1<<": "<<origin_exprs[i]<<"\n\n";
            for(int j=0;j<expressions[i].size();++j)
            {
                outfile<<"#Mutation Type: "<<mutant_types[j]<<'\n';
                for(int k=0;k<expressions[i][j].size();++k)
                    outfile<<expressions[i][j][k]<<'\n';
                outfile<<"#End Mutation Type\n\n";
            }
            outfile<<"#End Original Expression\n\n";
        }
        outfile<<"#End Mutant File\n";
    }
    catch(...)
    {
        outfile.close();
        throw "Fail to Write File!";
    }
    outfile.close();
}

#endif