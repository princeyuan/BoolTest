#ifndef EXPER_SELF_TEST_H
#define EXPER_SELF_TEST_H

#include <fstream>
#include <iostream>

#include "../SolutionBase/FailTestsFileIO.h"
#include "../SolutionBase/AssistMethod.h"

#include "Reducer/Greedy.h"
#include "Reducer/GE.h"
#include "Reducer/GRE.h"
#include "Reducer/Harrold.h"

void Exper_SelfTest(const std::string &common_expr_name,
					int expr_index_begin,int expr_index_end,
					const std::string &tests_file_dir)
{//Testing all test reduce algorithms, useing TCAS fail test cases as the test suite

	//Handle argument
	std::list<std::string> allfilelist;
	std::string filelist_name=tests_file_dir+std::string("AllFileList.txt");
	GetAllFileList(filelist_name.c_str(),allfilelist);

	char types[10][4]={"MLF","LNF","SA0","SA1","LRF","CCF","CDF","ASF","ORF","ENF"};

	//Init algorithms
	std::vector<Reducer*> algo(4);
	algo[0]=new Greedy;algo[1]=new GE;algo[2]=new GRE;algo[3]=new Harrold;

	for(int type_index=0;type_index<10;++type_index)
	{
		for(int expr_index=expr_index_begin;expr_index<=expr_index_end;++expr_index)
		{
			std::string index_str;
			int2str(expr_index,index_str);
			std::string name=common_expr_name+index_str;

			std::vector<TestReq*> reqs;
			std::vector<TestCase*> tests;
			std::set<int> tests_value_set;

			ReadReqs(allfilelist,name.c_str(),types[type_index],reqs);
			GetTestsValueFromReqs(reqs,tests_value_set);

			int i=0;
			tests.resize(tests_value_set.size());
			for(std::set<int>::const_iterator it=tests_value_set.begin();
				it!=tests_value_set.end();++it,++i)
				tests[i]=new TestCase(*it);

			for(int algo_index=0;algo_index<algo.size();++algo_index)
			{
				std::cout<<name<<'+'<<types[type_index]<<'+'<<algo[algo_index]->name();
			
				std::list<TestCase*> result;
				algo[algo_index]->reduce(reqs,tests,result);
				int kill_num=ComputeKillNumber(result,reqs);
				if(kill_num!=reqs.size())
					std::cout<<"\tFail!"<<std::endl;
				else
					std::cout<<std::endl;
			}

			for(std::vector<TestReq*>::iterator itr=reqs.begin();itr!=reqs.end();++itr)
				delete *itr;
			for(std::vector<TestCase*>::iterator itt=tests.begin();itt!=tests.end();++itt)
				delete *itt;
		}
	}

	for(int i=0;i<algo.size();++i)delete algo[i];
}

#endif