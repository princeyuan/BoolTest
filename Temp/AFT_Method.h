#ifndef GET_ALL_FAIL_TESTS_METHOD_H
#define GET_ALL_FAIL_TESTS_METHOD_H

#include <list>
#include <string>
#include <vector>
#include <fstream>

void AddOneTest(std::list<bool *> &test_suite,bool *test,int length)
{
	bool *temp=new bool[length];
	for(int i=0;i<length;++i)
		temp[i]=test[i];
	test_suite.push_back(temp);
}

void ClearTestSuite(std::list<bool *> &test_suite)
{
	for(std::list<bool *>::iterator it=test_suite.begin();
		it!=test_suite.end();++it)
		delete [](*it);
	test_suite.clear();
}

bool next_Binary(bool *bin_tuple,int length)
{
	int last=length-1;

	if(bin_tuple[last]==false)
	{
		bin_tuple[last]=true;
		return true;
	}

	if(bin_tuple[last]==true)
	{
		bin_tuple[last]=false;
		for(int i=last-1;i>=0;--i)
		{
			if(bin_tuple[i]==false)
			{
				bin_tuple[i]=true;
				return true;;
			}
			else //test[i]==true
			{
				bin_tuple[i]=false;
			}
		}
		return false;
	}

	return false;
}

void RecordBoolTestSuite(int test_length,
						 const std::list<bool *> &test_suite,
						 const char *filename,
						 const std::vector<int> &is_binary,
						 const std::vector<std::string> &out_dir,
						 std::list<std::string> &filelist)
{
	if(test_suite.empty())
		return;

	for(int i=0;i<is_binary.size() && i<out_dir.size();++i)
	{
		std::ofstream outfile;
		std::string local_filename=out_dir[i]+std::string(filename);
		outfile.open(local_filename.c_str());

		if(is_binary[i]!=0)
		{
			for(std::list<bool *>::const_iterator it=test_suite.begin();
				it!=test_suite.end();++it)
			{
				for(int i=0;i<test_length;++i)
				{
					if((*it)[i])
						outfile<<'1';
					else
						outfile<<'0';
				}
				outfile<<'\n';
			}
		}
		else
		{
			int k=0;
			for(std::list<bool *>::const_iterator it=test_suite.begin();
				it!=test_suite.end();++it)
			{
				int temp=0;
				for(int i=0;i<test_length;++i)
				{
					if((*it)[i])
						temp=temp*2+1;
					else
						temp=temp*2;
				}
				outfile<<temp<<'\t';

				if(++k==10)
				{
					outfile<<'\n';
					k=0;
				}
			}
		}

		outfile.close();
	}
	filelist.push_back(std::string(filename));
}

void RecordFileList(const std::list<std::string> &filelist,const std::vector<std::string> &out_dir)
{
	for(int i=0;i<out_dir.size();++i)
	{
		std::ofstream outfile;
		std::string filename=out_dir[i]+std::string("AllFileList.txt");
		outfile.open(filename.c_str(),std::ios::app);
		for(std::list<std::string>::const_iterator it=filelist.begin();
			it!=filelist.end();++it)
		{
			outfile<<*it<<'\n';
		}
		outfile.close();
	}
}

#endif