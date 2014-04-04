#ifndef EXPER_GIVEN_TWO_GROUP_H
#define EXPER_GIVEN_TWO_GROUP_H

#include <fstream>
#include <iostream>

#include "../SolutionBase/FailTestsFileIO.h"
#include "../SolutionBase/AssistMethod.h"

#include "Reducer/Greedy.h"
#include "Reducer/GE.h"
#include "Reducer/GRE.h"
#include "Reducer/Harrold.h"

void Exper_GivenTwoGroup(const std::string &common_expr_name,
						 int expr_index_begin,int expr_index_end,
						 const std::string &tests_file_dir,
						 int local_argc,
						 char* local_argv[])
{
	//Handle argument
	std::list<std::string> allfilelist;
	std::string filelist_name=tests_file_dir+std::string("AllFileList.txt");
	GetAllFileList(filelist_name.c_str(),allfilelist);

	std::list<std::string> src_types;
	std::list<std::string> obj_types;
	int i=0;
	for(;i<local_argc;++i)if(!strcmp(local_argv[i],"@"))break;
	if(i>=local_argc-1)
		return;
	else
	{
		for(int j=0;j<i;j++)
			src_types.push_back(local_argv[j]);
		for(int j=i+1;j<local_argc;j++)
			obj_types.push_back(local_argv[j]);
	}

	//Init algorithms
	std::vector<Reducer*> algo(4);
	algo[0]=new Greedy;algo[1]=new GE;algo[2]=new GRE;algo[3]=new Harrold;

	//Experiment for each original bool expression
	for(int expr_index=expr_index_begin-expr_index_begin;
		expr_index<=expr_index_end-expr_index_begin;
		expr_index++)
	{
		std::string index_str;
		int2str(expr_index+expr_index_begin,index_str);
		std::string expr_name=common_expr_name+index_str;

		std::vector<TestReq*> src_reqs;
		std::vector<TestReq*> obj_reqs;
		std::vector<TestCase*> src_tests;
		std::set<int> src_tests_value_set;

		//src_reqs
		for(std::list<std::string>::const_iterator itt=src_types.begin();
			itt!=src_types.end();++itt)
		{
			std::vector<TestReq*> src_reqs_temp;
			std::set<int> tests_value_temp;
			ReadReqs(allfilelist,expr_name.c_str(),itt->c_str(),src_reqs_temp);	
			GetTestsValueFromReqs(src_reqs_temp,tests_value_temp);

			src_reqs.insert(src_reqs.end(),src_reqs_temp.begin(),src_reqs_temp.end());
			src_tests_value_set.insert(tests_value_temp.begin(),tests_value_temp.end());
		}

		//src_tests
		int i=0;
		src_tests.resize(src_tests_value_set.size());
		for(std::set<int>::const_iterator it=src_tests_value_set.begin();
			it!=src_tests_value_set.end();++it,++i)
			src_tests[i]=new TestCase(*it);

		//obj_reqs
		for(std::list<std::string>::const_iterator itt=obj_types.begin();
			itt!=obj_types.end();++itt)
		{
			std::vector<TestReq*> obj_reqs_temp;
			ReadReqs(allfilelist,expr_name.c_str(),itt->c_str(),obj_reqs_temp);
			obj_reqs.insert(obj_reqs.end(),obj_reqs_temp.begin(),obj_reqs_temp.end());
		}

		//reduce src tests
		for(int algo_index=0;algo_index<algo.size();++algo_index)
		{
			std::list<TestCase*> result;
			algo[algo_index]->reduce(src_reqs,src_tests,result);
			int kill_number=ComputeKillNumber(result,obj_reqs);

			if(kill_number!=obj_reqs.size())
				std::cout<<"Error: ";
			else
				std::cout<<"OK: ";

			for(std::list<std::string>::const_iterator itt=src_types.begin();
				itt!=src_types.end();++itt)
				std::cout<<*itt<<' ';
			std::cout<<"-> ";
			for(std::list<std::string>::const_iterator itt=obj_types.begin();
				itt!=obj_types.end();++itt)
				std::cout<<*itt<<' ';
			std::cout<<"in "<<expr_name<<'+'<<algo[algo_index]->name();

			if(kill_number!=obj_reqs.size())
				std::cout<<"\t"<<kill_number<<'/'<<obj_reqs.size()<<std::endl;
			else
				std::cout<<std::endl;
		}

		for(int i=0;i<src_reqs.size();++i)delete src_reqs[i];
		for(int i=0;i<obj_reqs.size();++i)delete obj_reqs[i];
		for(int i=0;i<src_tests.size();++i)delete src_tests[i];
	}

	for(int i=0;i<algo.size();++i)delete algo[i];
}

#endif