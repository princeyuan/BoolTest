#include "CodeFileCreater.h"

#include <fstream>
#include <algorithm>

void CodeFileCreater::run(const std::vector<std::string> &fun_names,
                          const std::vector<std::vector<std::string> > &all_args,
                          const std::vector<std::string> &code_expressions,
                          const WriteCodeFileState &state,
                          const std::string &filename)
{
    std::string sourcecode;

    CreateSourceCode(fun_names,all_args,code_expressions,sourcecode);    
    CreateSourceFile(sourcecode,state,filename);
}

void CodeFileCreater::CreateSourceCode(const std::vector<std::string> &fun_names,
                                       const std::vector<std::vector<std::string> > &all_args,
                                       const std::vector<std::string> &code_expressions,
                                       std::string &sourcecode)
{
    for(int i=0;i<code_expressions.size();i++)
    {
        std::string temp;

        CreateFunName(fun_names[i],temp);
        CreateArglist(all_args[i],temp);
        CreateFunBody(code_expressions[i],temp);

        sourcecode.append(temp);
    }
}

void CodeFileCreater::CreateSourceFile(const std::string &sourcecode,
                                       const WriteCodeFileState &state,
                                       const std::string &filename)
{
    std::ofstream outfile;
    if(state==CodeFileBegin || state==CodeFileBoth)
        outfile.open(filename.c_str());
    else
        outfile.open(filename.c_str(),std::ios::app);
    if(!outfile)
    {
        throw "Fail to Create File!";
    }

    try
    {
        if(state==CodeFileBegin || state==CodeFileBoth)
        {
            std::string head_micro;
            CreateHeadMicro(filename,head_micro);
            outfile<<"#ifndef "<<head_micro<<"\n#define "<<head_micro<<"\n\n";
        }

        for(int i=0;i<sourcecode.size();i++)
        {
            outfile<<sourcecode[i];
        }

        if(state==CodeFileEnd || state==CodeFileBoth)
        {
            outfile<<"\n#endif\n";
        }
    }
    catch (...)
    {
        outfile.close();
        throw "Fail to Write File!";
    }

    outfile.close();
}

void CodeFileCreater::CreateFunName(const std::string &name,std::string &code)
{
    code.append("bool ");
    code.append(name);
}

void CodeFileCreater::CreateArglist(const std::vector<std::string> &arguments,std::string &code)
{
    code.append("(");
    std::vector<std::string>::const_iterator it=arguments.begin();
    while(true)
    {
        code.append("bool ");
        code.append(*it);
        if((++it)!=arguments.end())
            code.append(",");
        else
            break;
    }
    code.append(")\n");
}

void CodeFileCreater::CreateFunBody(const std::string &expression,std::string &code)
{
    code.append("{\n\t");
    code.append("return ");
    code.append(expression);
    code.append(";\n}\n");
}

void CodeFileCreater::CreateHeadMicro(const std::string &filename,std::string &head_micro)
{
    head_micro.clear();

    std::string::const_iterator it_begin=filename.begin();
    for(std::string::const_iterator it=filename.begin();
        it!=filename.end();++it)
    {
        if(*it=='/')
            it_begin=it+1;
    }

    head_micro.assign(it_begin,filename.end());

    for(std::string::iterator it=head_micro.begin();
        it!=head_micro.end();++it)
    {
        if(*it=='.')
            *it='_';
        else if(*it>='a' && *it<='z')
            *it='A'+(*it-'a');
    }
}