#include "ATF_ProgramCreater.h"

#include <cmath>
#include <fstream>

//版本1 不存储oracal, 而是每次都利用原始布尔表达式计算, 对某个变异找到所有失效测试用例后才进行IO
void GAFT_ProgramCreater::run(const std::vector<std::string> &origin_fun_names,
                              const std::vector<std::vector<std::string> > &mutant_fun_names,
                              const std::vector<int> &argument_number_list,
                              const char *origin_code_file,
                              const char *mutant_code_file,
                              const char *program_src_file)
{
    std::ofstream outfile;
    outfile.open(program_src_file);
    if(!outfile)return;

    std::string pure_origin_code_file,pure_mutant_code_file;
    ExtractPureFilenameWithExtension(origin_code_file,pure_origin_code_file);
    ExtractPureFilenameWithExtension(mutant_code_file,pure_mutant_code_file);

    //#include
    outfile<<"#include <list>\n#include <vector>\n#include <string.h>\n";
    
    //可选择系统Windows or Linux
    if(m_is_win_sys)
        outfile<<"#include <direct.h>\n";
    else
        outfile<<"#include <sys/stat.h>\n";

    outfile<<"#include \""<<pure_origin_code_file<<"\"\n";
    outfile<<"#include \""<<pure_mutant_code_file<<"\"\n";
    outfile<<"#include \"AFT_Method.h\"\n\n";

    //For each origin boolean expression
    //create a funcation to find fail tests of each mutant expression
    for(int i=0;i<mutant_fun_names.size();++i)
    {
        outfile<<"void Function"<<i+1<<"(const std::vector<int> &is_binary,const std::vector<std::string> &out_dir)\n{\n";

        outfile<<"std::list<std::string> filelist;\n";
        outfile<<"std::list<bool *> fail_tests;\n";
        outfile<<"bool test["<<argument_number_list[i]<<"];\n";
        outfile<<"for(int i=0;i<"<<argument_number_list[i]<<";++i)test[i]=false;\n\n";

        for(int j=0;j<mutant_fun_names[i].size();++j)
        {
            
            outfile<<"ClearTestSuite(fail_tests);";

            outfile<<"do{";
            outfile<<"if("<<origin_fun_names[i]<<"(";

            outfile<<"test[0]";
            for(int para_index=1;para_index<argument_number_list[i];++para_index)
                outfile<<",test["<<para_index<<"]";

            outfile<<")!="<<mutant_fun_names[i][j]<<"(";

            outfile<<"test[0]";
            for(int para_index=1;para_index<argument_number_list[i];++para_index)
                outfile<<",test["<<para_index<<"]";

            outfile<<"))AddOneTest(fail_tests,test,"<<argument_number_list[i]<<");";

            outfile<<"}while(next_Binary(test,"<<argument_number_list[i]<<"));";

            outfile<<"RecordBoolTestSuite("<<argument_number_list[i]<<",fail_tests,\""<<mutant_fun_names[i][j]<<"_FailTests.txt\",is_binary,out_dir,filelist);\n";
        }

        outfile<<"ClearTestSuite(fail_tests);\n";
        outfile<<"RecordFileList(filelist,out_dir);\n";

        outfile<<"}\n\n";
    }

    //The Main function to call above functions
    outfile<<"int main(int argc, char **argv)\n{\n";

    outfile<<"std::vector<int> is_binary;\n";
    outfile<<"std::vector<std::string> out_dir;\n\n";

    outfile<<"if(argc==1){\n\tis_binary.resize(1,1);\n\tout_dir.resize(1,std::string(\"Result/\"));\n}\n";
    outfile<<"else if(argc==2){\n\tif(0==strcmp(argv[1],\"-b\"))is_binary.resize(1,1);\n\telse if(0==strcmp(argv[1],\"-i\"))is_binary.resize(1,0);\n\telse return 0;\n\tout_dir.resize(1,std::string(\"Result/\"));\n}\n";
    outfile<<"else if(argc==3){\n\tif(0==strcmp(argv[1],\"-b\"))is_binary.resize(1,1);\n\telse if(0==strcmp(argv[1],\"-i\"))is_binary.resize(1,0);\n\telse return 0;\n\tout_dir.resize(1,std::string(argv[2]));\n}\n";
    outfile<<"else if(argc==5){\n\tis_binary.resize(2);\n\tif(0==strcmp(argv[1],\"-b\"))is_binary[0]=1;\n\telse if(0==strcmp(argv[1],\"-i\"))is_binary[0]=0;\n\telse return 0;\n\tif(0==strcmp(argv[3],\"-b\"))is_binary[1]=1;\n\telse if(0==strcmp(argv[3],\"-i\"))is_binary[1]=0;\n\telse return 0;\n\tout_dir.resize(2);\n\tout_dir[0]=std::string(argv[2]);\n\tout_dir[1]=std::string(argv[4]);\n}\n";
    outfile<<"else return 0;\n\n";

    //可选择系统Windows or Linux
    if(m_is_win_sys)
        outfile<<"for(int i=0;i<out_dir.size();++i)_mkdir(out_dir[i].c_str());\n\n";
    else
        outfile<<"for(int i=0;i<out_dir.size();++i)mkdir(out_dir[i].c_str(),S_IRWXU|S_IRWXG|S_IRWXO);\n\n";

    for(int i=0;i<origin_fun_names.size();++i)
    {
        outfile<<"Function"<<i+1<<"(is_binary,out_dir);\n";
    }
    outfile<<"\nreturn 0;\n}\n";

    outfile.close();
}

//版本2 存储oracal, 对某个变异找到所有失效测试用例后才进行IO
void GAFT_ProgramCreater::run_o(const std::vector<std::string> &origin_fun_names,
                                const std::vector<std::vector<std::string> > &mutant_fun_names,
                                const std::vector<int> &argument_number_list,
                                const char *origin_code_file,
                                const char *mutant_code_file,
                                const char *program_src_file)
{
    std::ofstream outfile;
    outfile.open(program_src_file);
    if(!outfile)return;

    std::string pure_origin_code_file,pure_mutant_code_file;
    ExtractPureFilenameWithExtension(origin_code_file,pure_origin_code_file);
    ExtractPureFilenameWithExtension(mutant_code_file,pure_mutant_code_file);

    //#include
    outfile<<"#include <list>\n#include <vector>\n#include <string.h>\n";

    //可选择系统Windows or Linux
    if(m_is_win_sys)
        outfile<<"#include <direct.h>\n";
    else
        outfile<<"#include <sys/stat.h>\n";

    outfile<<"#include \""<<pure_origin_code_file<<"\"\n";
    outfile<<"#include \""<<pure_mutant_code_file<<"\"\n";
    outfile<<"#include \"AFT_Method.h\"\n\n";

    //For each origin boolean expression
    //create a funcation to find fail tests of each mutant expression
    for(int i=0;i<mutant_fun_names.size();++i)
    {
        outfile<<"void Function"<<i+1<<"(const std::vector<int> &is_binary,const std::vector<std::string> &out_dir)\n{\n";

        outfile<<"std::list<std::string> filelist;\n";
        outfile<<"std::list<bool *> fail_tests;\n";
        outfile<<"bool test["<<argument_number_list[i]<<"];\n";
        outfile<<"for(int i=0;i<"<<argument_number_list[i]<<";++i)test[i]=false;\n\n";

        outfile<<"bool oracle["<<(int)pow(2.0,argument_number_list[i])<<"];\n";
        outfile<<"int i=0;do{oracle[i++]="<<origin_fun_names[i]<<"(";

        outfile<<"test[0]";
        for(int para_index=1;para_index<argument_number_list[i];++para_index)
            outfile<<",test["<<para_index<<"]";
        outfile<<");}while(next_Binary(test,"<<argument_number_list[i]<<"));\n\n";

        for(int j=0;j<mutant_fun_names[i].size();++j)
        {
            outfile<<"ClearTestSuite(fail_tests);";
            outfile<<"i=0;";

            outfile<<"do{if(oracle[i++]!="<<mutant_fun_names[i][j]<<"(";

            outfile<<"test[0]";
            for(int para_index=1;para_index<argument_number_list[i];++para_index)
                outfile<<",test["<<para_index<<"]";

            outfile<<"))AddOneTest(fail_tests,test,"<<argument_number_list[i]<<");";

            outfile<<"}while(next_Binary(test,"<<argument_number_list[i]<<"));";

            outfile<<"RecordBoolTestSuite("<<argument_number_list[i]<<",fail_tests,\""<<mutant_fun_names[i][j]<<"_FailTests.txt\",is_binary,out_dir,filelist);\n";
        }

        outfile<<"ClearTestSuite(fail_tests);\n";
        outfile<<"RecordFileList(filelist,out_dir);\n";

        outfile<<"}\n\n";
    }

    //The Main function to call above functions
    outfile<<"int main(int argc, char **argv)\n{\n";

    outfile<<"std::vector<int> is_binary;\n";
    outfile<<"std::vector<std::string> out_dir;\n\n";

    outfile<<"if(argc==1){\n\tis_binary.resize(1,1);\n\tout_dir.resize(1,std::string(\"Result/\"));\n}\n";
    outfile<<"else if(argc==2){\n\tif(0==strcmp(argv[1],\"-b\"))is_binary.resize(1,1);\n\telse if(0==strcmp(argv[1],\"-i\"))is_binary.resize(1,0);\n\telse return 0;\n\tout_dir.resize(1,std::string(\"Result/\"));\n}\n";
    outfile<<"else if(argc==3){\n\tif(0==strcmp(argv[1],\"-b\"))is_binary.resize(1,1);\n\telse if(0==strcmp(argv[1],\"-i\"))is_binary.resize(1,0);\n\telse return 0;\n\tout_dir.resize(1,std::string(argv[2]));\n}\n";
    outfile<<"else if(argc==5){\n\tis_binary.resize(2);\n\tif(0==strcmp(argv[1],\"-b\"))is_binary[0]=1;\n\telse if(0==strcmp(argv[1],\"-i\"))is_binary[0]=0;\n\telse return 0;\n\tif(0==strcmp(argv[3],\"-b\"))is_binary[1]=1;\n\telse if(0==strcmp(argv[3],\"-i\"))is_binary[1]=0;\n\telse return 0;\n\tout_dir.resize(2);\n\tout_dir[0]=std::string(argv[2]);\n\tout_dir[1]=std::string(argv[4]);\n}\n";
    outfile<<"else return 0;\n\n";

    //可选择系统Windows or Linux
    if(m_is_win_sys)
        outfile<<"for(int i=0;i<out_dir.size();++i)_mkdir(out_dir[i].c_str());\n\n";
    else
        outfile<<"for(int i=0;i<out_dir.size();++i)mkdir(out_dir[i].c_str(),S_IRWXU|S_IRWXG|S_IRWXO);\n\n";

    for(int i=0;i<origin_fun_names.size();++i)
    {
        outfile<<"Function"<<i+1<<"(is_binary,out_dir);\n";
    }
    outfile<<"\nreturn 0;\n}\n";

    outfile.close();
}

void GAFT_ProgramCreater::ExtractPureFilenameWithExtension(const std::string &filename,
                                                           std::string &pure_filename)
{
    std::string::const_iterator it_head=filename.begin();
    for(std::string::const_iterator it=filename.begin();
        it!=filename.end();++it)
    {
        if(*it=='/')
            it_head=it+1;
    }

    pure_filename.assign(it_head,filename.end());
}