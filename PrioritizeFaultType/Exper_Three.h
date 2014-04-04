#ifndef EXPER_THREE_H
#define EXPER_THREE_H

#include <fstream>
#include <iostream>

#include "../SolutionBase/FailTestsFileIO.h"
#include "../SolutionBase/AssistMethod.h"

#include "Reducer/Greedy.h"
#include "Reducer/GE.h"
#include "Reducer/GRE.h"
#include "Reducer/Harrold.h"

float NAPFD(const std::vector<int> & addtl_kill_num,int test_num,int fault_num)
{
	if(test_num==0 || fault_num==0)return 0.0;

	float temp_value=0.0;
	for(int i=0;i<addtl_kill_num.size();++i)
		temp_value+=addtl_kill_num[i];
	temp_value+=(test_num-addtl_kill_num.size())*addtl_kill_num[addtl_kill_num.size()-1];
	return (temp_value/test_num)/fault_num;
}

void Exper_Three(const std::string &common_expr_name,
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

	std::vector<Reducer*> algo(4);
	algo[0]=new Greedy;algo[1]=new GE;algo[2]=new GRE;algo[3]=new Harrold;

	#define ALGONUM 4
	#define ALGONUMSQUARE 16

	//record data
	/*std::vector<std::vector<std::vector<int> > > 
		first_test_num(2,std::vector<std::vector<int> >(expr_index_end-expr_index_begin+1,std::vector<int>(ALGONUM,0)));
	std::vector<std::vector<std::vector<int> > > 
		left_test_num(2,std::vector<std::vector<int> >(expr_index_end-expr_index_begin+1,std::vector<int>(ALGONUMSQUARE,0)));
	std::vector<std::vector<std::vector<int> > > 
		first_kill_num(2,std::vector<std::vector<int> >(expr_index_end-expr_index_begin+1,std::vector<int>(ALGONUM,0)));*/
	std::vector<int> all_reqs_num(expr_index_end-expr_index_begin+1,0);
	std::vector<int> max_test_num(expr_index_end-expr_index_begin+1,0);
	std::vector<std::vector<std::vector<float> > > napfd
		(2,std::vector<std::vector<float> >(expr_index_end-expr_index_begin+1,std::vector<float>(ALGONUMSQUARE,0.0)));
	std::vector<std::vector<std::vector<std::vector<int> > > > addtl_kill_num
		(2,std::vector<std::vector<std::vector<int> > >
			(expr_index_end-expr_index_begin+1,std::vector<std::vector<int> >
				(ALGONUMSQUARE,std::vector<int>())));
	
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
		all_reqs_num[expr_index]=all_reqs.size();

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
		for(int round=0;round<=1;++round)
		//round=0: CCF+CDF run firstly
		//round=1: ASF+ORF+ENF run firstly
		{
			std::vector<TestReq*> part_left_reqs(part_reqs[round==0?1:0]);

			for(int algo_index=0;algo_index<algo.size();++algo_index)
			{
				std::list<TestCase*> result;
				algo[algo_index]->reduce(part_reqs[round],part_tests[round],result);
				/*int kill_num=ComputeKillNumber(result,all_reqs);*/
				ComputeKillNumberAndModify(result,part_left_reqs);

				/*first_test_num[round][expr_index][algo_index]=result.size();
				first_kill_num[round][expr_index][algo_index]=kill_num;*/

				for(int algo_index_j=0;algo_index_j<algo.size();++algo_index_j)
				{
					std::list<TestCase*> second_result;
					algo[algo_index_j]->reduce(part_left_reqs,part_tests[round==0?1:0],second_result);

					std::list<TestCase*> full_result;
					full_result.insert(full_result.end(),result.begin(),result.end());
					full_result.insert(full_result.end(),second_result.begin(),second_result.end());

					if(max_test_num[expr_index]<full_result.size())max_test_num[expr_index]=full_result.size();

					int local_index=ALGONUM*algo_index+algo_index_j;

					/*left_test_num[round][expr_index][local_index]=second_result.size();*/
					AddtlKillNumber(full_result,all_reqs,addtl_kill_num[round][expr_index][local_index]);
				}
			}
		}

		//step 3: computing N-APFD
		for(int round=0;round<=1;++round)
		{
			for(int result_index=0;result_index<ALGONUMSQUARE;++result_index)
			{
				napfd[round][expr_index][result_index]=
					NAPFD(addtl_kill_num[round][expr_index][result_index],max_test_num[expr_index],all_reqs_num[expr_index]);
			}
		}

		std::cout<<"Done!"<<std::endl;

		for(std::vector<TestReq*>::iterator itr_i=all_reqs.begin();itr_i!=all_reqs.end();++itr_i)delete *itr_i;
		for(std::vector<TestReq*>::iterator itr_j=part_reqs[0].begin();itr_j!=part_reqs[0].end();++itr_j)delete *itr_j;
		for(std::vector<TestReq*>::iterator itr_k=part_reqs[1].begin();itr_k!=part_reqs[1].end();++itr_k)delete *itr_k;
		for(std::vector<TestCase*>::iterator itr_l=part_tests[0].begin();itr_l!=part_tests[0].end();++itr_l)delete *itr_l;
		for(std::vector<TestCase*>::iterator itr_m=part_tests[1].begin();itr_m!=part_tests[1].end();++itr_m)delete *itr_m;
	}

	//output
	std::string filename=output_dir+std::string("NAPFD.xls");
	std::ofstream outfile(filename.c_str());

	for(int result_index=0;result_index<ALGONUMSQUARE;++result_index)
	{
		for(int expr_index=expr_index_begin-expr_index_begin;
			expr_index<=expr_index_end-expr_index_begin;
			expr_index++)
		{
			for(int round=0;round<=1;++round)
			{
				outfile<<napfd[round][expr_index][result_index]<<'\t';
			}
			outfile<<'\t';
		}
		outfile<<'\n';
	}
	outfile.close();

	std::cout<<"Finish!"<<std::endl;

	for(int i=0;i<algo.size();++i)delete algo[i];
}

#endif