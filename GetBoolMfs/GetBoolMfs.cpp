#include <set>
#include <list>
#include <vector>
#include <string>
#include <string.h>
#include <iostream>
#include <algorithm>

#include "BoolMFSAlgorithm.h"
#include "BoolPFSAlgorithm.h"
#include "BoolTestSchemaFileIO.h"

int main(int argc, char* argv[])
{
    //Handle arguments
    if(argc<5)return 0;
    
    bool is_mfs_not_pfs;
    if(strstr(argv[0],"MFS")!=NULL)
        is_mfs_not_pfs=true;
    else if(strstr(argv[0],"PFS")!=NULL)
        is_mfs_not_pfs=false;
    else
        return 0;
    
    std::string common_expr_name(argv[1]);
    int expr_index_begin = 0;
    int expr_index_end   = 0;
    std::string filelist_name;
    std::string output_dir;
    char types_temp[][4]={"ASF","CCF","CDF","ENF","LNF","LRF","MLF","ORF","SA0","SA1","VNF","VRF"};
    std::vector<std::string> types;
    std::vector<std::pair<int,int> > rangs;
    int type_arg_begin;
    
    //e.g.: GetBoolMFS TCAS 1 - 20 bTests/ MFS/ [ASF ORF ...]
    if(strcmp(argv[3],"-")==0)
    {
        expr_index_begin = atoi(argv[2]);
        expr_index_end   = atoi(argv[4]);
        filelist_name=std::string(argv[5])+std::string("AllFileList.txt");
        output_dir=argv[6];
        type_arg_begin=7;
    }
    //e.g.: GetBoolMFS TCAS 1 bTests/ MFS/ [ASF ORF ...]
    else
    {
        expr_index_begin = atoi(argv[2]);
        expr_index_end   = atoi(argv[2]);
        filelist_name=std::string(argv[3])+std::string("AllFileList.txt");
        output_dir=argv[4];
        type_arg_begin=5;
    }
    
    if(argc>type_arg_begin)
    {
        for(int i=type_arg_begin;i<argc;++i)
        {
            types.push_back(std::string(argv[i]));
            if(i+3<argc && strcmp(argv[i+2],"-")==0)
            {
                rangs.push_back(std::pair<int,int>(atoi(argv[i+1]),atoi(argv[i+3])));
                i+=3;
            }
            else if(i+2<argc && strcmp(argv[i+2],"--")==0)
            {
                rangs.push_back(std::pair<int,int>(atoi(argv[i+1]),-1));
                i+=2;
            }
            else
            {
                rangs.push_back(std::pair<int,int>(-1,-1));
            }
        }
    }
    else
    {
        types.insert(types.begin(),types_temp,types_temp+12);
        rangs.resize(12,std::pair<int,int>(-1,-1));
    }
    
    std::list<std::string> allfilelist;
    GetAllFileList(filelist_name.c_str(),allfilelist);

    /*---------------------------------------------------------------*/

    //for each origin expression
    for(int expr_index=expr_index_begin;expr_index<=expr_index_end;++expr_index)
    {
        std::cout<<"Computing MFSs for expression "<<expr_index<<" with fault type:\n\t";

        std::string index_str;
        int2str(expr_index,index_str);
        std::string expr_name=common_expr_name+index_str;

        //for each fault type
        for(int type_index=0;type_index<types.size();++type_index)
        {
            std::cout<<types[type_index]<<": total ";
            int cur_type_count=0;

            for(std::list<std::string>::const_iterator it=allfilelist.begin();
                it!=allfilelist.end();++it)
            {
                int i=it->size()-1;
                for(;i>=0;--i)
                    if((*it)[i]=='/')
                        break;
                std::string real_file_name(it->begin()+i+1,it->end());
                std::string file_tag=std::string(expr_name)+std::string(types[type_index]);

                //for each concrete fault
                if(0==strncmp(real_file_name.c_str(),file_tag.c_str(),file_tag.size()))
                {
                    std::string mu_index_str(real_file_name.begin()+file_tag.size(),
                                             real_file_name.end()-strlen("_FailTests.txt"));
                    int mu_index=atoi(mu_index_str.c_str());
                    if((rangs[type_index].first!=-1 && rangs[type_index].second!=-1 && 
                        mu_index>=rangs[type_index].first && mu_index<=rangs[type_index].second) || 
                       (rangs[type_index].first!=-1 && rangs[type_index].second==-1 &&
                        mu_index>=rangs[type_index].first)  ||
                       (rangs[type_index].first==-1 && rangs[type_index].second==-1))
                    {
                        std::string mu_expr_name(real_file_name.begin(),
                                                 real_file_name.end()-strlen("_FailTests.txt"));
                        std::string out_file_name=output_dir
                                                 +mu_expr_name
                                                 +(is_mfs_not_pfs?"_MFS.txt":"_PFS.txt");

                        std::list<std::vector<int> > fail_tests;
                        if(!getBinaryTestSuiteFromFile(*it,fail_tests))
                            continue;
                        else
                            ++cur_type_count;
                    
                        if(is_mfs_not_pfs)
                        {
                            std::list<std::vector<std::list<std::pair<std::vector<int>,HaveChildOrNot> > > > fs;
                            std::set<std::vector<int> > mfs_set;

                            ExtractFailureSchemas(fail_tests,fs);
                            GetMFSSetFromFSSet(fs,mfs_set);
                            RecordMFS(mfs_set,out_file_name.c_str());
                        }
                        else
                        {
                            //
                        }
                    }
                }
            }

            std::cout<<cur_type_count<<"\n\t";
        }
        std::cout<<"... Done!\n";
    }

    return 0;
}
