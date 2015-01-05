#include "MutantExprFileIO.h"

#include <fstream>
#include <iostream>

void ReadMutantExprFile(const std::string &expr_file,
                        std::string &expr_common_name,
                        std::vector<int> &origin_index,
                        std::vector<std::string> &origin_exprs,
                        std::vector<std::vector<std::string> > &mutant_types,
                        std::vector<std::vector<std::vector<std::string> > > &expressions)
{
    std::ifstream infile;
    infile.open(expr_file.c_str());
    if(!infile)
    {
        std::cout<<"Error when reading file "<<expr_file;
        return;
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
                    for(char *pos=temp;*pos!='\0';++pos)
                        if(*pos=='/')
                            temp=pos+1;
                    for(char *pos=temp;*pos!='\0';++pos)
                    {
                        if(*pos=='.')
                        {
                            *pos='\0';
                            break;
                        }
                    }
                    expr_common_name=temp;
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
                    origin_index.push_back(atoi(temp));

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
        std::cout<<"Error when reading file "<<expr_file;
        return;
    }

    infile.close();
}

void WriteMutantExprFile(const std::string &mutant_expr_file,
                         const std::string &origin_expr_file,
                         const std::vector<std::string> &origin_exprs,
                         const std::vector<std::string> &mutant_types,
                         const std::vector<std::vector<std::vector<std::string> > > &expressions)
{
    std::ofstream outfile;
    outfile.open(mutant_expr_file.c_str());
    if(!outfile)
    {
        std::cout<<"Error when creating file "<<mutant_expr_file;
        return;
    };
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
    catch (...)
    {
        outfile.close();
        std::cout<<"Error when creating file "<<mutant_expr_file;
        return;
    }
    outfile.close();
}