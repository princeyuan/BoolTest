#ifndef BOOL_TEST_SCHEMA_FILE_IO_H
#define BOOL_TEST_SCHEMA_FILE_IO_H

#include <map>
#include <list>
#include <string>
#include <vector>
#include <fstream>
#include <string.h>

#include "BoolMFSAlgorithm.h"
#include "BoolPFSAlgorithm.h"

void GetAllFileList(const char *allfilelist_file_name,
                    std::list<std::string> &allfilelist)
{
    int i=strlen(allfilelist_file_name)-1;
    for(;i>=0;--i)
        if(allfilelist_file_name[i]=='/')
            break;
    std::string dir(allfilelist_file_name,allfilelist_file_name+i+1);

    std::ifstream infile;
    infile.open(allfilelist_file_name);
    if(!infile)return;
    char buffer[1024];
    while(infile.getline(buffer,100,'\n'))
        allfilelist.push_back(dir+std::string(buffer));
    infile.close();
}

int getOneBinaryTestFromLine(char* str,std::vector<int> &test)
{
    test.resize(strlen(str));
    for(int i=0;i<strlen(str);++i)
    {
        if(str[i]=='1')
            test[i]=1;
        else if(str[i]=='0')
            test[i]=0;
    }
    return test.size();
}

bool getBinaryTestSuiteFromFile(const std::string &file,std::list<std::vector<int> > &tests)
{
    std::ifstream infile;
    infile.open(file.c_str());
    if(!infile)return false;

    char buffer[256];
    while(infile.getline(buffer,100,'\n'))
    {
        std::vector<int> temp;
        if(getOneBinaryTestFromLine(buffer,temp)!=0)//length of temp is not 0
            tests.push_back(temp);
    }

    infile.close();
    return true;
}

void RecordMFS(const std::map<std::vector<int>,int> &mfs_set,
               const char *filename)
{
    std::ofstream outfile;
    outfile.open(filename);
    
    for(std::map<std::vector<int>,int>::const_iterator it=mfs_set.begin();
        it!=mfs_set.end();++it)
    {
        for(std::vector<int>::const_iterator it_v=(*it).first.begin();
            it_v!=(*it).first.end();++it_v)
        {
            if(*it_v==-1)
                outfile<<'-';
            else
                outfile<<*it_v;
        }
        outfile<<'\n';
    }

    outfile.close();
}

void RecordMFS(const std::set<std::vector<int> > &mfs_set,
               const char *filename)
{
    std::ofstream outfile;
    outfile.open(filename);
    
    for(std::set<std::vector<int> >::const_iterator it=mfs_set.begin();
        it!=mfs_set.end();++it)
    {
        for(std::vector<int>::const_iterator it_v=it->begin();
            it_v!=it->end();++it_v)
        {
            if(*it_v==-1)
                outfile<<'-';
            else
                outfile<<*it_v;
        }
        outfile<<'\n';
    }

    outfile.close();
}

void RecordPFS(const std::map<std::vector<int>,SpectraInfo> &pschemas,
               const char *filename)
{
    std::ofstream outfile;
    outfile.open(filename);
    std::list<std::pair<std::vector<int>,SpectraInfo> > temp;
    for(std::map<std::vector<int>,SpectraInfo>::const_iterator it=pschemas.begin();
        it!=pschemas.end();++it)
    {
        temp.push_back(*it);
    }
    
    while(!temp.empty())
    {
        double per=0.0;
        std::list<std::pair<std::vector<int>,SpectraInfo> >::iterator max;
        for(std::list<std::pair<std::vector<int>,SpectraInfo> >::iterator it=temp.begin();
            it!=temp.end();++it)
        {
            double cur_per=(double)(it->second.m_num_fail)/(double)(it->second.m_num_cov);
            if(cur_per > per)
            {
                per=cur_per;
                max=it;
            }
        }
        
        for(std::vector<int>::const_iterator it_v=(*max).first.begin();
            it_v!=(*max).first.end();++it_v)
        {
            if(*it_v==-1)
                outfile<<'-';
            else
                outfile<<*it_v;
        }

        outfile<<"\t "<<max->second.m_num_fail<<' '<<max->second.m_num_cov<<'\t'<<per<<'\n';
        
        temp.erase(max);
    }

    outfile.close();
}

#endif