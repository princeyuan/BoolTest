#include "ProjectMethod.h"

#include <fstream>
#include <iostream>
#include "../SolutionBase/AssistMethod.h"

void ReadReqs(const std::list<std::string> &allfilelist,
			  const char *expr_name, const char *fault_type,
			  std::vector<TestReq*> &reqs)
{
	std::string file_tag=std::string(expr_name)+std::string(fault_type);
	std::list<TestReq*> req_list;

	for(std::list<std::string>::const_iterator it=allfilelist.begin();
		it!=allfilelist.end();
		++it)
	{
		int i=it->size()-1;
		for(;i>=0;--i)
			if((*it)[i]=='/')
				break;
		std::string real_file_name(it->begin()+i+1,it->end());

		if(0==strncmp(real_file_name.c_str(),file_tag.c_str(),file_tag.size()))
		{
			std::ifstream infile;
			infile.open(it->c_str());
			if(!infile)continue;
			char buffer[1024];
			std::vector<int> temp;
			std::vector<int> req_values;
			while(infile.getline(buffer,1024,'\n'))
			{
				getAllIntsFromLine(buffer,temp);
				req_values.insert(req_values.end(),temp.begin(),temp.end());
			}
			req_list.push_back(new TestReq(req_values));
			infile.close();
		}
	}

	reqs.assign(req_list.begin(),req_list.end());
}

void GetTestsFromReqs(const std::vector<TestReq*> &reqs,
					  std::vector<TestCase*> &tests)
{
	std::set<int> temp;
	for(std::vector<TestReq*>::const_iterator it=reqs.begin();
		it!=reqs.end();++it)
	{
		const std::set<int> &req_value=(*it)->getValue();
		temp.insert(req_value.begin(),req_value.end());
	}
	tests.resize(temp.size());
	std::set<int>::const_iterator iti=temp.begin();
	for(int i=0;iti!=temp.end();++iti,++i)
	{
		tests[i]=new TestCase(*iti);
	}
}

void GetTestsValueFromReqs(const std::vector<TestReq*> &reqs,
						   std::set<int> &tests_value)
{
	tests_value.clear();
	for(std::vector<TestReq*>::const_iterator it=reqs.begin();
		it!=reqs.end();++it)
	{
		const std::set<int> &req_value=(*it)->getValue();
		tests_value.insert(req_value.begin(),req_value.end());
	}
}

int ComputeKillNumber(const std::list<TestCase*> &tests,
					  const std::vector<TestReq*> &object_reqs)
{
	int kill_number=0;

	for(std::vector<TestReq*>::const_iterator it=object_reqs.begin();
		it!=object_reqs.end();
		++it)
	{
		for(std::list<TestCase*>::const_iterator iti=tests.begin();
			iti!=tests.end();
			++iti)
		{
			if((*it)->satisfy(*iti))
			{
				++kill_number;
				break;
			}
		}
	}

	return kill_number;
}