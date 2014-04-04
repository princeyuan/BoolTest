#include <string>
#include <direct.h>

#include "Exper_One.h"
#include "Exper_Two.h"
#include "Exper_Three.h"

void main(int argc, char* argv[])
{
	std::string common_expr_name(argv[1]);
	if(0!=strcmp(argv[3],"-"))return;
	int expr_index_begin=atoi(argv[2]);
	int expr_index_end=atoi(argv[4]);
	std::string tests_file_dir(argv[5]);

	char user_select;
	std::cout<<"Select experiment: 1. Test number & efficiency; 2. Random test number; 3. APFD"<<std::endl;
	std::cin>>user_select;

	if(argc==6)
	{
		_mkdir("Output");

		if(user_select=='1')
			Exper_One(common_expr_name,expr_index_begin,expr_index_end,tests_file_dir,"Output/");
		else if(user_select=='2')
			Exper_Two(common_expr_name,expr_index_begin,expr_index_end,tests_file_dir,"Output/");
		else  if(user_select=='3')
			Exper_Three(common_expr_name,expr_index_begin,expr_index_end,tests_file_dir,"Output/");
	}
	else if(argc==7)
	{
		_mkdir(argv[6]);

		if(user_select=='1')
			Exper_One(common_expr_name,expr_index_begin,expr_index_end,tests_file_dir,argv[6]);
		else if(user_select=='2')
			Exper_Two(common_expr_name,expr_index_begin,expr_index_end,tests_file_dir,argv[6]);
		else  if(user_select=='3')
			Exper_Three(common_expr_name,expr_index_begin,expr_index_end,tests_file_dir,argv[6]);
	}
}