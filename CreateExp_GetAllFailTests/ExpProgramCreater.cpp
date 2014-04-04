#include "ExpProgramCreater.h"

#include <fstream>

/*void ExpProgramCreater::CreateProgram(const std::vector<std::string> &origin_fun_names,
									  const std::vector<std::vector<std::vector<std::string> > > &mutant_fun_names,
									  const std::vector<std::vector<char> > &all_argument_list,
									  const char *origin_expr_cpp_file,
									  const char *mutant_expr_cpp_file,
									  const char *program_src_file)
{
	std::ofstream outfile;
	outfile.open(program_src_file);
	if(!outfile)return;

	//#include
	outfile<<"#include <set>\n";
	outfile<<"#include <vector>\n";
	outfile<<"#include <string>\n";
	outfile<<"#include \""<<origin_expr_cpp_file<<"\"\n";
	outfile<<"#include \""<<mutant_expr_cpp_file<<"\"\n";
	outfile<<"#include \"../BoolMethod.h\"\n\n";

	//For each origin boolean expression
	//create a funcation to find fail tests of each mutant expression
	for(int i=0;i<mutant_fun_names.size();++i)
	{
		outfile<<"void Function"<<i+1<<"(bool is_binary)\n{\n";

		outfile<<"std::set<std::string> filelist;\n";
		outfile<<"std::vector<bool> test("<<all_argument_list[i].size()<<",false);\n";
		outfile<<"do{\n";
		outfile<<"bool oracal="<<origin_fun_names[i]<<"(";

		outfile<<"test[0]";
		for(int para_index=1;para_index<all_argument_list[i].size();++para_index)
		{
			outfile<<",test["<<para_index<<"]";
		}

		outfile<<");\n";

		for(int j=0;j<mutant_fun_names[i].size();++j)
		{
			for(int k=0;k<mutant_fun_names[i][j].size();++k)
			{
				outfile<<"if(oracal!="<<mutant_fun_names[i][j][k]<<"(";

				outfile<<"test[0]";
				for(int para_index=1;para_index<all_argument_list[i].size();++para_index)
				{
					outfile<<",test["<<para_index<<"]";
				}

				outfile<<"))RecordBoolTest(test,\"Result/"<<mutant_fun_names[i][j][k]<<"_FailTests.txt\",is_binary,filelist);\n";
			}
		}

		outfile<<"}while(GetNextBoolTest(test));\n";
		outfile<<"RecordFileList(filelist);\n";

		outfile<<"}\n\n";
	}

	//The Main function to call above functions
	outfile<<"void main(int argc, char **argv)\n{\n";
	outfile<<"bool is_binary=true;\n";
	outfile<<"if(argc>1)is_binary=false;\n\n";
	for(int i=0;i<origin_fun_names.size();++i)
	{
		outfile<<"Function"<<i+1<<"(is_binary);\n";
	}
	outfile<<"}\n";

	outfile.close();
}*/

void ExpProgramCreater::CreateProgram(const std::vector<std::string> &origin_fun_names,
									  const std::vector<std::vector<std::vector<std::string> > > &mutant_fun_names,
									  const std::vector<std::vector<char> > &all_argument_list,
									  const char *origin_expr_cpp_file,
									  const char *mutant_expr_cpp_file,
									  const char *program_src_file)
{
	std::ofstream outfile;
	outfile.open(program_src_file);
	if(!outfile)return;

	//#include
	outfile<<"#include <set>\n";
	outfile<<"#include <vector>\n";
	outfile<<"#include <string>\n";
	outfile<<"#include <direct.h>\n";
	outfile<<"#include \""<<origin_expr_cpp_file<<"\"\n";
	outfile<<"#include \""<<mutant_expr_cpp_file<<"\"\n";
	outfile<<"#include \"BoolMethod.h\"\n\n";

	//For each origin boolean expression
	//create a funcation to find fail tests of each mutant expression
	for(int i=0;i<mutant_fun_names.size();++i)
	{
		outfile<<"void Function"<<i+1<<"(const std::vector<bool> &is_binary,const std::vector<std::string> &out_dir)\n{\n";

		outfile<<"std::set<std::string> filelist;\n";
		outfile<<"std::vector<bool> test;\n";
		outfile<<"std::list<std::vector<bool> > fail_tests;\n\n";

		for(int j=0;j<mutant_fun_names[i].size();++j)
		{
			for(int k=0;k<mutant_fun_names[i][j].size();++k)
			{
				outfile<<"fail_tests.clear();";
				outfile<<"test.resize("<<all_argument_list[i].size()<<",false);";

				outfile<<"do{";
				outfile<<"if("<<origin_fun_names[i]<<"(";

				outfile<<"test[0]";
				for(int para_index=1;para_index<all_argument_list[i].size();++para_index)
					outfile<<",test["<<para_index<<"]";
				
				outfile<<")!="<<mutant_fun_names[i][j][k]<<"(";

				outfile<<"test[0]";
				for(int para_index=1;para_index<all_argument_list[i].size();++para_index)
					outfile<<",test["<<para_index<<"]";

				outfile<<"))fail_tests.push_back(test);";

				outfile<<"}while(GetNextBoolTest(test));";

				outfile<<"RecordBoolTestSuite(fail_tests,\""<<mutant_fun_names[i][j][k]<<"_FailTests.txt\",is_binary,out_dir,filelist);\n";
			}
		}

		outfile<<"RecordFileList(filelist,out_dir);\n";

		outfile<<"}\n\n";
	}

	//The Main function to call above functions
	outfile<<"void main(int argc, char **argv)\n{\n";

	outfile<<"std::vector<bool> is_binary;\n";
	outfile<<"std::vector<std::string> out_dir;\n\n";

	outfile<<"if(argc==1){\n\tis_binary.push_back(false);\n\tout_dir.push_back(std::string(\"Result/\"));\n}\n";
	outfile<<"else if(argc==2){\n\tif(0==strcmp(argv[1],\"-b\"))is_binary.push_back(true);\n\telse if(0==strcmp(argv[1],\"-i\"))is_binary.push_back(false);\n\telse return;\n\tout_dir.push_back(std::string(\"Result/\"));\n}\n";
	outfile<<"else if(argc==3){\n\tif(0==strcmp(argv[1],\"-b\"))is_binary.push_back(true);\n\telse if(0==strcmp(argv[1],\"-i\"))is_binary.push_back(false);\n\telse return;\n\tout_dir.push_back(std::string(argv[2]));\n}\n";
	outfile<<"else if(argc==5){\n\tif(0==strcmp(argv[1],\"-b\"))is_binary.push_back(true);\n\telse if(0==strcmp(argv[1],\"-i\"))is_binary.push_back(false);\n\telse return;\n\tif(0==strcmp(argv[3],\"-b\"))is_binary.push_back(true);\n\telse if(0==strcmp(argv[3],\"-i\"))is_binary.push_back(false);\n\telse return;\n\tout_dir.push_back(std::string(argv[2]));\n\tout_dir.push_back(std::string(argv[4]));\n}\n";
	outfile<<"else return;\n\n";

	outfile<<"for(int i=0;i<out_dir.size();++i)_mkdir(out_dir[i].c_str());\n\n";

	for(int i=0;i<origin_fun_names.size();++i)
	{
		outfile<<"Function"<<i+1<<"(is_binary,out_dir);\n";
	}
	outfile<<"}\n";

	outfile.close();
}