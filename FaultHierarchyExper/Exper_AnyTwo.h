#ifndef EXPER_ANY_TWO_H
#define EXPER_ANY_TWO_H

#include <fstream>
#include <iostream>

#include "../SolutionBase/FailTestsFileIO.h"
#include "../SolutionBase/AssistMethod.h"

#include "Reducer/Greedy.h"
#include "Reducer/GE.h"
#include "Reducer/GRE.h"
#include "Reducer/Harrold.h"

void Exper_AnyTwo(const std::string &common_expr_name,
				  int expr_index_begin,int expr_index_end,
				  const std::string &tests_file_dir,
				  const char *output_dir)
{//Find the kill percent between any two fault types in TCAS

	//Handle argument
	std::list<std::string> allfilelist;
	std::string filelist_name=tests_file_dir+std::string("AllFileList.txt");
	GetAllFileList(filelist_name.c_str(),allfilelist);

	char types[10][4]={"MLF","LNF","SA0","SA1","LRF","CCF","CDF","ASF","ORF","ENF"};

	std::vector<Reducer*> algo(4);
	algo[0]=new Greedy;algo[1]=new GE;algo[2]=new GRE;algo[3]=new Harrold;

	std::vector<std::vector<int> > req_number(expr_index_end-expr_index_begin+1,std::vector<int>(10));
	std::vector<std::vector<std::vector<int> > > 
		kill_number(100,std::vector<std::vector<int> >(expr_index_end-expr_index_begin+1,std::vector<int>(algo.size())));
	//int req_number[20][10];
	//int kill_number[100][20][4];

	for(int i=0;i<10;i++)
	{
		for(int j=i+1;j<10;j++)
		{
			std::cout<<" Analyzing fault hierarchy between "<<types[i]<<" and "<<types[j]<<"...";

			int sum_req_num_i=0;
			int sum_req_num_j=0;
			std::vector<int> sum_kill_num_itoj(algo.size(),0);
			std::vector<int> sum_kill_num_jtoi(algo.size(),0);

			for(int expr_index=expr_index_begin-expr_index_begin;
				expr_index<=expr_index_end-expr_index_begin;
				expr_index++)
			{
				std::string index_str;
				int2str(expr_index+expr_index_begin,index_str);
				std::string name=common_expr_name+index_str;

				std::vector<TestReq*> reqs_i,reqs_j;
				std::vector<TestCase*> tests_i,tests_j;
				std::set<int> tests_value_i,tests_value_j;

				ReadReqs(allfilelist,name.c_str(),types[i],reqs_i);
				ReadReqs(allfilelist,name.c_str(),types[j],reqs_j);

				req_number[expr_index][i]=reqs_i.size();sum_req_num_i+=reqs_i.size();
				req_number[expr_index][j]=reqs_j.size();sum_req_num_j+=reqs_j.size();

				GetTestsValueFromReqs(reqs_i,tests_value_i);
				GetTestsValueFromReqs(reqs_j,tests_value_j);

				for(std::set<int>::const_iterator it=tests_value_i.begin();
					it!=tests_value_i.end();++it)
					tests_i.push_back(new TestCase(*it));
				for(std::set<int>::const_iterator it=tests_value_j.begin();
					it!=tests_value_j.end();++it)
					tests_j.push_back(new TestCase(*it));

				//i kill how much j
				for(int algo_index=0;algo_index<algo.size();algo_index++)
				{
					int relation_index=i*10+j;
					std::list<TestCase*> result;
					algo[algo_index]->reduce(reqs_i,tests_i,result);
					kill_number[relation_index][expr_index][algo_index]=ComputeKillNumber(result,reqs_j);
					sum_kill_num_itoj[algo_index]+=kill_number[relation_index][expr_index][algo_index];
				}

				//j kill how much i
				for(int algo_index=0;algo_index<algo.size();algo_index++)
				{
					int relation_index=j*10+i;
					std::list<TestCase*> result;
					algo[algo_index]->reduce(reqs_j,tests_j,result);
					kill_number[relation_index][expr_index][algo_index]=ComputeKillNumber(result,reqs_i);
					sum_kill_num_jtoi[algo_index]+=kill_number[relation_index][expr_index][algo_index];
				}

				for(std::vector<TestReq*>::iterator itr_i=reqs_i.begin();itr_i!=reqs_i.end();++itr_i)
					delete *itr_i;
				for(std::vector<TestReq*>::iterator itr_j=reqs_j.begin();itr_j!=reqs_j.end();++itr_j)
					delete *itr_j;
				for(std::vector<TestCase*>::iterator itt_i=tests_i.begin();itt_i!=tests_i.end();++itt_i)
					delete *itt_i;
				for(std::vector<TestCase*>::iterator itt_j=tests_j.begin();itt_j!=tests_j.end();++itt_j)
					delete *itt_j;
			}

			//Output
			std::ofstream outfile;
			std::string result_file_itoj=std::string(output_dir)+
										 std::string(types[i])+std::string("@")+std::string(types[j])+std::string(".xls");
			outfile.open(result_file_itoj.c_str());
			outfile<<types[i]<<" kill "<<types[j]<<'\n';
			for(int expr_index=expr_index_begin-expr_index_begin;
				expr_index<=expr_index_end-expr_index_begin;
				expr_index++)
			{
				std::string index_str;
				int2str(expr_index+expr_index_begin,index_str);
				std::string name=common_expr_name+index_str;
				outfile<<name<<'\t';
			}
			outfile<<"Total Ave\n";
			for(int algo_index=0;algo_index<algo.size();algo_index++)
			{
				for(int expr_index=expr_index_begin-expr_index_begin;
					expr_index<=expr_index_end-expr_index_begin;
					expr_index++)
				{
					int relation_index=i*10+j;
					outfile<<((float)kill_number[relation_index][expr_index][algo_index])/((float)req_number[expr_index][j]);
					outfile<<'\t';
				}
				outfile<<((float)sum_kill_num_itoj[algo_index])/((float)sum_req_num_j)<<'\n';
			}
			outfile.close();

			std::string result_file_jtoi=std::string(output_dir)+
										 std::string(types[j])+std::string("@")+std::string(types[i])+std::string(".xls");
			outfile.open(result_file_jtoi.c_str());
			outfile<<types[j]<<" kill "<<types[i]<<'\n';
			for(int expr_index=expr_index_begin-expr_index_begin;
				expr_index<=expr_index_end-expr_index_begin;
				expr_index++)
			{
				std::string index_str;
				int2str(expr_index+expr_index_begin,index_str);
				std::string name=common_expr_name+index_str;
				outfile<<name<<'\t';
			}
			outfile<<"Total Ave\n";
			for(int algo_index=0;algo_index<algo.size();algo_index++)
			{
				for(int expr_index=expr_index_begin-expr_index_begin;
					expr_index<=expr_index_end-expr_index_begin;
					expr_index++)
				{
					int relation_index=j*10+i;
					outfile<<((float)kill_number[relation_index][expr_index][algo_index])/((float)req_number[expr_index][i]);
					outfile<<'\t';
				}
				outfile<<((float)sum_kill_num_jtoi[algo_index])/((float)sum_req_num_i)<<'\n';
			}
			outfile.close();

			std::cout<<"\tDone!"<<std::endl;
		}
	}

	for(int i=0;i<algo.size();++i)delete algo[i];
}

#endif