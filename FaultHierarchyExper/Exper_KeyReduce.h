#ifndef EXPER_KEY_REDUCE_H
#define EXPER_KEY_REDUCE_H

#include <fstream>
#include <iostream>

#include "../SolutionBase/FailTestsFileIO.h"
#include "../SolutionBase/AssistMethod.h"

#include "Reducer/Greedy.h"
#include "Reducer/GE.h"
#include "Reducer/GRE.h"
#include "Reducer/Harrold.h"

void Exper_KeyReduce(const std::string &common_expr_name,
					 int expr_index_begin,int expr_index_end,
					 const std::string &tests_file_dir,
					 const char *output_dir)
{//Find the number of test cases that obtained by 10 fault types / 5 fault types / 2 fault types
 //Kill percent of 5 fault types / 2 fault types for all 10 fault types

	//Handle argument
	std::list<std::string> allfilelist;
	std::string filelist_name=tests_file_dir+std::string("AllFileList.txt");
	GetAllFileList(filelist_name.c_str(),allfilelist);

	char types[10][4]={"MLF","LNF","SA0","SA1","LRF","CCF","CDF","ASF","ORF","ENF"};
	int fault_type_begin[3]={0,5,5};
	int fault_type_end[3]={10,10,7};

	//Init algorithms
	std::vector<Reducer*> algo(4);
	algo[0]=new Greedy;algo[1]=new GE;algo[2]=new GRE;algo[3]=new Harrold;

	std::vector<std::vector<std::vector<int> > > 
		reduced_test_num(3,std::vector<std::vector<int> >(expr_index_end-expr_index_begin+1,std::vector<int>(algo.size())));
	std::vector<std::vector<std::vector<int> > > 
		reduced_kill_num(3,std::vector<std::vector<int> >(expr_index_end-expr_index_begin+1,std::vector<int>(algo.size())));
	std::vector<int> all_reqs_num(expr_index_end-expr_index_begin+1);

	//Experiment for each original bool expression
	for(int expr_index=expr_index_begin-expr_index_begin;
		expr_index<=expr_index_end-expr_index_begin;
		expr_index++)
	{
		std::string index_str;
		int2str(expr_index+expr_index_begin,index_str);
		std::string name=common_expr_name+index_str;

		std::cout<<" Processing "<<name<<" ... ";

		std::vector<TestReq*> all_reqs;
		std::vector<TestCase*> all_tests;
		std::set<int> all_tests_value_set;

		for(int type_index=fault_type_begin[0];type_index<fault_type_end[0];++type_index)
		{
			std::vector<TestReq*> reqs_temp;
			std::set<int> tests_value_temp;
			ReadReqs(allfilelist,name.c_str(),types[type_index],reqs_temp);
			GetTestsValueFromReqs(reqs_temp,tests_value_temp);

			all_reqs.insert(all_reqs.end(),reqs_temp.begin(),reqs_temp.end());
			all_tests_value_set.insert(tests_value_temp.begin(),tests_value_temp.end());
		}
		all_reqs_num[expr_index]=all_reqs.size();

		int i=0;
		all_tests.resize(all_tests_value_set.size());
		for(std::set<int>::const_iterator it=all_tests_value_set.begin();
			it!=all_tests_value_set.end();++it,++i)
			all_tests[i]=new TestCase(*it);

		for(int algo_index=0;algo_index<algo.size();++algo_index)
		{
			std::list<TestCase*> result;
			algo[algo_index]->reduce(all_reqs,all_tests,result);
			int kill_num=ComputeKillNumber(result,all_reqs);
			//Record
			reduced_test_num[0][expr_index][algo_index]=result.size();
			reduced_kill_num[0][expr_index][algo_index]=kill_num;
		}

		//Compute 5 types and 2 types
		for(int round=1;round<3;++round)
		{
			std::vector<TestReq*> part_reqs;
			std::vector<TestCase*> part_tests;
			std::set<int> part_tests_value_set;

			for(int type_index=fault_type_begin[round];type_index<fault_type_end[round];++type_index)
			{
				std::vector<TestReq*> reqs_temp;
				std::set<int> tests_value_temp;

				ReadReqs(allfilelist,name.c_str(),types[type_index],reqs_temp);
				GetTestsValueFromReqs(reqs_temp,tests_value_temp);

				part_reqs.insert(part_reqs.end(),reqs_temp.begin(),reqs_temp.end());
				part_tests_value_set.insert(tests_value_temp.begin(),tests_value_temp.end());
			}

			int i=0;
			part_tests.resize(part_tests_value_set.size());
			for(std::set<int>::const_iterator it=part_tests_value_set.begin();
				it!=part_tests_value_set.end();++it,++i)
				part_tests[i]=new TestCase(*it);

			for(int algo_index=0;algo_index<algo.size();++algo_index)
			{
				std::list<TestCase*> result;
				algo[algo_index]->reduce(part_reqs,part_tests,result);
				int kill_num=ComputeKillNumber(result,all_reqs);
				//Record
				reduced_test_num[round][expr_index][algo_index]=result.size();
				reduced_kill_num[round][expr_index][algo_index]=kill_num;
			}

			for(std::vector<TestReq*>::iterator itr=part_reqs.begin();itr!=part_reqs.end();++itr)
				delete *itr;
			for(std::vector<TestCase*>::iterator itt=part_tests.begin();itt!=part_tests.end();++itt)
				delete *itt;
		}

		for(std::vector<TestReq*>::iterator itr=all_reqs.begin();itr!=all_reqs.end();++itr)
			delete *itr;
		for(std::vector<TestCase*>::iterator itt=all_tests.begin();itt!=all_tests.end();++itt)
			delete *itt;

		std::cout<<"Done!"<<std::endl;
	}

	for(int i=0;i<algo.size();++i)delete algo[i];

	//Output
	char file_head[3][3]={"10","5","2"};
	for(int round=0;round<3;++round)
	{
		std::string filename1=output_dir+std::string(file_head[round])+std::string("@TestNum.xls");
		std::string filename2=output_dir+std::string(file_head[round])+std::string("@KillPer.xls");
		std::string filename3=output_dir+std::string(file_head[round])+std::string("@Effect.xls");

		std::ofstream outfile1(filename1.c_str());
		std::ofstream outfile2(filename2.c_str());
		std::ofstream outfile3(filename3.c_str());

		for(int expr_index=expr_index_begin-expr_index_begin;
			expr_index<=expr_index_end-expr_index_begin;
			expr_index++)
		{
			std::string index_str;
			int2str(expr_index+expr_index_begin,index_str);

			outfile1<<common_expr_name<<index_str<<'\t';
			outfile2<<common_expr_name<<index_str<<'\t';
			outfile3<<common_expr_name<<index_str<<'\t';
		}
		outfile1<<"Total\n";
		outfile2<<"Total\n";
		outfile3<<"Total\n";

		for(int algo_index=0;algo_index<algo.size();++algo_index)
		{
			int sum_test_num=0;
			int sum_kill_num=0;
			int sum_reqs_num=0;

			for(int expr_index=expr_index_begin-expr_index_begin;
				expr_index<=expr_index_end-expr_index_begin;
				expr_index++)
			{
				outfile1<<reduced_test_num[round][expr_index][algo_index]<<'\t';
				outfile2<<((float)reduced_kill_num[round][expr_index][algo_index])/
					((float)all_reqs_num[expr_index])<<'\t';
				outfile3<<((float)reduced_kill_num[round][expr_index][algo_index])/
					((float)all_reqs_num[expr_index])/
					((float)reduced_test_num[round][expr_index][algo_index])<<'\t';

				sum_test_num+=reduced_test_num[round][expr_index][algo_index];
				sum_kill_num+=reduced_kill_num[round][expr_index][algo_index];
				sum_reqs_num+=all_reqs_num[expr_index];
			}
			outfile1<<sum_test_num<<'\n';
			outfile2<<((float)sum_kill_num)/((float)sum_reqs_num)<<'\n';
			outfile3<<((float)sum_kill_num)/((float)sum_reqs_num)/((float)sum_test_num)<<'\n';
		}

		outfile1.close();
		outfile2.close();
		outfile3.close();
	}
}

#endif