#ifndef EXPER_ONE_H
#define EXPER_ONE_H

#include <fstream>
#include <iostream>

#include "../SolutionBase/FailTestsFileIO.h"
#include "../SolutionBase/AssistMethod.h"

#include "Reducer/Greedy.h"
#include "Reducer/GE.h"
#include "Reducer/GRE.h"
#include "Reducer/Harrold.h"

void Exper_One(const std::string &common_expr_name,
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
    std::vector<std::vector<std::vector<int> > > 
        first_test_num(2,std::vector<std::vector<int> >(expr_index_end-expr_index_begin+1,std::vector<int>(ALGONUM,0)));
    std::vector<std::vector<std::vector<int> > > 
        left_test_num(2,std::vector<std::vector<int> >(expr_index_end-expr_index_begin+1,std::vector<int>(ALGONUMSQUARE,0)));
    std::vector<std::vector<std::vector<int> > > 
        first_kill_num(2,std::vector<std::vector<int> >(expr_index_end-expr_index_begin+1,std::vector<int>(ALGONUM,0)));
    std::vector<int> all_reqs_num(expr_index_end-expr_index_begin+1,0);
    
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
                int kill_num=ComputeKillNumber(result,all_reqs);
                ComputeKillNumberAndModify(result,part_left_reqs);

                first_test_num[round][expr_index][algo_index]=result.size();
                first_kill_num[round][expr_index][algo_index]=kill_num;

                for(int algo_index_j=0;algo_index_j<algo.size();++algo_index_j)
                {
                    std::list<TestCase*> second_result;
                    algo[algo_index_j]->reduce(part_left_reqs,part_tests[round==0?1:0],second_result);

                    int local_index=ALGONUM*algo_index+algo_index_j;
                    left_test_num[round][expr_index][local_index]=second_result.size();
                }
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
    char file_head[2][2]={"0","1"};
    for(int round=0;round<2;++round)
    {
        std::string filename1=output_dir+std::string(file_head[round])+std::string("@FirstTestNum.xls");
        std::string filename2=output_dir+std::string(file_head[round])+std::string("@FirstKillPer.xls");
        std::string filename3=output_dir+std::string(file_head[round])+std::string("@LeftTestNum.xls");
        std::string filename4=output_dir+std::string(file_head[round])+std::string("@TotalTestNum.xls");

        std::ofstream outfile1(filename1.c_str());
        std::ofstream outfile2(filename2.c_str());
        std::ofstream outfile3(filename3.c_str());
        std::ofstream outfile4(filename4.c_str());

        for(int expr_index=expr_index_begin-expr_index_begin;
            expr_index<=expr_index_end-expr_index_begin;
            expr_index++)
        {
            std::string index_str;
            int2str(expr_index+expr_index_begin,index_str);

            outfile1<<common_expr_name<<index_str<<'\t';
            outfile2<<common_expr_name<<index_str<<'\t';
            outfile3<<common_expr_name<<index_str<<'\t';
            outfile4<<common_expr_name<<index_str<<'\t';
        }
        outfile1<<"Total\n";
        outfile2<<"Total\n";
        outfile3<<"Total\n";
        outfile4<<"Total\n";

        for(int algo_index=0;algo_index<algo.size();++algo_index)
        {
            int sum_first_test_num=0;
            int sum_first_kill_num=0;
            int sum_reqs_num=0;

            for(int expr_index=expr_index_begin-expr_index_begin;
                expr_index<=expr_index_end-expr_index_begin;
                expr_index++)
            {
                sum_first_test_num+=first_test_num[round][expr_index][algo_index];
                sum_first_kill_num+=first_kill_num[round][expr_index][algo_index];
                sum_reqs_num+=all_reqs_num[expr_index];

                outfile1<<first_test_num[round][expr_index][algo_index]<<'\t';

                if(all_reqs_num[expr_index]!=0)
                    outfile2<<((float)first_kill_num[round][expr_index][algo_index])/((float)all_reqs_num[expr_index])<<'\t';
                else
                    outfile2<<-1<<'\t';
            }
            outfile1<<sum_first_test_num<<'\n';
            outfile2<<((float)sum_first_kill_num)/((float)sum_reqs_num)<<'\n';

            for(int algo_index_j=0;algo_index_j<algo.size();++algo_index_j)
            {
                int sum_left_test_num=0;
                int local_index=ALGONUM*algo_index+algo_index_j;

                for(int expr_index=expr_index_begin-expr_index_begin;
                    expr_index<=expr_index_end-expr_index_begin;
                    expr_index++)
                {
                    sum_left_test_num+=left_test_num[round][expr_index][local_index];

                    outfile3<<left_test_num[round][expr_index][local_index]<<'\t';
                    outfile4<<first_test_num[round][expr_index][algo_index]+
                        left_test_num[round][expr_index][local_index]<<'\t';
                }
                outfile3<<sum_left_test_num<<'\n';
                outfile4<<sum_first_test_num+sum_left_test_num<<'\n';
            }
        }

        outfile1.close();
        outfile2.close();
        outfile3.close();
        outfile4.close();
    }

    std::cout<<"Finish!"<<std::endl;

    for(int i=0;i<algo.size();++i)delete algo[i];
}

#endif