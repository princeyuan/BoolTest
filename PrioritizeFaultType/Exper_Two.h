#ifndef EXPER_TWO_H
#define EXPER_TWO_H

#include <fstream>
#include <iostream>
#include <time.h>

#include "../SolutionBase/FailTestsFileIO.h"
#include "../SolutionBase/AssistMethod.h"

void Exper_Two(const std::string &common_expr_name,
			   int expr_index_begin,int expr_index_end,
			   const std::string &tests_file_dir,
			   const char *output_dir)
{
	//Handle argument
	std::list<std::string> allfilelist;
	std::string filelist_name=tests_file_dir+std::string("AllFileList.txt");
	GetAllFileList(filelist_name.c_str(),allfilelist);

	char types[10][4]={"MLF","LNF","SA0","SA1","LRF","CCF","CDF","ASF","ORF","ENF"};
	int fault_type_begin[3]={0,5,7};
	int fault_type_end[3]={10,7,10};

	#define RAND_TIMES 100
	//#define ALGONUM 4
	//#define ALGONUMSQUARE 16

	//record data
	std::vector<std::vector<int> > rand_test_num(expr_index_end-expr_index_begin+1,std::vector<int>(RAND_TIMES,0));
	
	//for each expression
	for(int expr_index=expr_index_begin-expr_index_begin;
		expr_index<=expr_index_end-expr_index_begin;
		expr_index++)
	{
		std::string index_str;
		int2str(expr_index+expr_index_begin,index_str);
		std::string name=common_expr_name+index_str;

		std::cout<<" Processing "<<name<<" ... ";

		//step 1: collect initial data
		std::vector<TestReq*> all_reqs;
		for(int type_index=fault_type_begin[0];type_index<fault_type_end[0];++type_index)
		{
			std::vector<TestReq*> reqs_temp;
			ReadReqs(allfilelist,name.c_str(),types[type_index],reqs_temp);
			all_reqs.insert(all_reqs.end(),reqs_temp.begin(),reqs_temp.end());
		}

		//part_reqs[0] is for CCF+CDF
		//part_reqs[1] is for ASF+ORF+ENF
		std::vector<TestReq*> part_reqs[2];
		std::vector<TestCase*> part_tests[2];
		for(int round=0;round<=1;++round)
		{
			std::set<int> part_tests_value_set;
			for(int type_index=fault_type_begin[round+1];type_index<fault_type_end[round+1];++type_index)
			{
				std::vector<TestReq*> reqs_temp;
				std::set<int> tests_value_temp;
				ReadReqs(allfilelist,name.c_str(),types[type_index],reqs_temp);
				GetTestsValueFromReqs(reqs_temp,tests_value_temp);
				part_reqs[round].insert(part_reqs[round].end(),reqs_temp.begin(),reqs_temp.end());
				part_tests_value_set.insert(tests_value_temp.begin(),tests_value_temp.end());
			}
			int i=0;
			part_tests[round].resize(part_tests_value_set.size());
			for(std::set<int>::const_iterator it=part_tests_value_set.begin();
				it!=part_tests_value_set.end();++it,++i)
				part_tests[round][i]=new TestCase(*it);
		}
		
		//step 2: experiment
		//random select from two groups
		srand(time(NULL));
		for(int cur_rand_times=0;cur_rand_times<RAND_TIMES;++cur_rand_times)
		{
			int all_test_num=part_tests[0].size()+part_tests[1].size();
			std::vector<int> select_flag(all_test_num,0);

			std::list<TestCase*> result;

			while(ComputeKillNumber(result,all_reqs)!=all_reqs.size())
			{
				int index=rand()%all_test_num;
				while(select_flag[index]!=0)
					index=((index+1)<all_test_num)?(index+1):((index+1)%all_test_num);

				select_flag[index]=1;

				if(index<part_tests[0].size())
				{
					result.push_back(part_tests[0][index]);
				}
				else
				{
					result.push_back(part_tests[1][index-part_tests[0].size()]);
				}
			}

			rand_test_num[expr_index][cur_rand_times]=result.size();
		}

		std::cout<<"Done!"<<std::endl;

		for(std::vector<TestReq*>::iterator itr_i=all_reqs.begin();itr_i!=all_reqs.end();++itr_i)delete *itr_i;
		for(std::vector<TestReq*>::iterator itr_j=part_reqs[0].begin();itr_j!=part_reqs[0].end();++itr_j)delete *itr_j;
		for(std::vector<TestReq*>::iterator itr_k=part_reqs[1].begin();itr_k!=part_reqs[1].end();++itr_k)delete *itr_k;
		for(std::vector<TestCase*>::iterator itr_l=part_tests[0].begin();itr_l!=part_tests[0].end();++itr_l)delete *itr_l;
		for(std::vector<TestCase*>::iterator itr_m=part_tests[1].begin();itr_m!=part_tests[1].end();++itr_m)delete *itr_m;
	}

	//output
		std::string filename4=output_dir+std::string("Rand@TotalTestNum.xls");

		std::ofstream outfile4(filename4.c_str());

		for(int expr_index=expr_index_begin-expr_index_begin;
			expr_index<=expr_index_end-expr_index_begin;
			expr_index++)
		{
			std::string index_str;
			int2str(expr_index+expr_index_begin,index_str);

			outfile4<<common_expr_name<<index_str<<'\t';
		}
		outfile4<<"Total\n";

		for(int cur_rand_times=0;cur_rand_times<RAND_TIMES;++cur_rand_times)
		{
			int sum_test_num=0;

			for(int expr_index=expr_index_begin-expr_index_begin;
				expr_index<=expr_index_end-expr_index_begin;
				expr_index++)
			{
				outfile4<<rand_test_num[expr_index][cur_rand_times]<<'\t';
				sum_test_num+=rand_test_num[expr_index][cur_rand_times];
			}
			outfile4<<sum_test_num<<'\n';
		}

		outfile4.close();

	std::cout<<"Finish!"<<std::endl;
}

#endif