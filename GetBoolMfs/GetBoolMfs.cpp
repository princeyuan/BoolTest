#include <set>
#include <list>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

#include <string.h>

#include "BoolMFSAlgorithm.h"
#include "BoolPFSAlgorithm.h"
#include "BoolTestSchemaFileIO.h"

int main(int argc, char* argv[])
{
    //Handle arguments
    //e.g.: GetBoolMFS TCAS 1 - 20 bResult/ MFS/ [ASF ORF ...] [-log:] [log_file]
    if(argc<6)return 0;

    std::string common_expr_name(argv[1]);

    if(strcmp(argv[3],"-")!=0)return 0;
    int expr_index_begin=atoi(argv[2]);
    int expr_index_end=atoi(argv[4]);
    
    std::list<std::string> allfilelist;
    std::string filelist_name=std::string(argv[5])+std::string("AllFileList.txt");
    GetAllFileList(filelist_name.c_str(),allfilelist);

    std::string output_dir(argv[6]);

    bool is_log=false;
    char *log_filename=NULL;
    char types_temp[][4]={"ASF","CCF","CDF","ENF","LNF","LRF","MLF","ORF","SA0","SA1","VNF","VRF"};
    std::vector<std::string> types;
    if(argc>7)
    {
        for(int i=7;i<argc;++i)
        {
            types.push_back(std::string(argv[i]));
        }
    }
    else
    {
        types.insert(types.begin(),types_temp,types_temp+12);
    }

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
            std::list<std::string> smaller_mfs_names;
            std::list<std::set<int> > smaller_mfs_strength;

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
                    ++cur_type_count;
                    std::string mu_expr_name(real_file_name.begin(),
                                             real_file_name.end()-strlen("_FailTests.txt"));
                    std::string mfs_file_name=output_dir
                                             +mu_expr_name
                                             +std::string("_MFS.txt");

                    std::list<std::vector<int> > fail_tests;
                    if(!getBinaryTestSuiteFromFile(*it,fail_tests))
                        continue;
                    
                    //std::map<std::vector<int>,int> fs;
                    //std::map<std::vector<int>,int> mfs_set;

                    std::list<std::vector<std::list<std::pair<std::vector<int>,bool> > > > fs;
                    std::set<std::vector<int> > mfs_set;

                    ExtractFailureSchemas(fail_tests,fs);

                    GetMFSSetFromFSSet(fs,mfs_set);

                    RecordMFS(mfs_set,mfs_file_name.c_str());
                }
            }

            std::cout<<cur_type_count<<"\n\t";
        }
        std::cout<<"... Done!\n";
    }

    return 0;
}
