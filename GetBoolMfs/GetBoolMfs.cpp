#include <set>
#include <list>
#include <vector>
#include <iostream>
#include <algorithm>
#include <direct.h>

#include "../SolutionBase/AssistMethod.h"

int GetBoolFaultSchema(const std::list<std::vector<bool> > &failed_tests,
					   std::set<std::vector<int> > &fs_set);

int GetMfsSetFromFsSet(const std::set<std::vector<int> > &fs_set,
					   std::set<std::vector<int> > &mfs_set);

void RecordMfs(const std::set<std::vector<int> > &mfs_set,
			   const std::string &filename);

void main(int argc, char* argv[])
{
	//Handle arguments
	//e.g.: GetBoolMfs TCAS 1 - 20 Bin/ [MFS/] [ASF ORF ...]
	if(argc<6)return;

	std::string common_expr_name(argv[1]);

	if(strcmp(argv[3],"-")!=0)return;
	int expr_index_begin=atoi(argv[2]);
	int expr_index_end=atoi(argv[4]);
	
	std::list<std::string> allfilelist;
	std::string filelist_name=std::string(argv[5])+std::string("AllFileList.txt");
	GetAllFileList(filelist_name.c_str(),allfilelist);

	char types_temp[10][4]={"MLF","LNF","SA0","SA1","LRF","CCF","CDF","ASF","ORF","ENF"};
	std::vector<std::string> types_temp_vector(types_temp,types_temp+10);
	std::vector<std::string> types;
	std::string output_dir;
	if(argc>6)
	{
		if(find(types_temp_vector.begin(),types_temp_vector.end(),argv[6])==types_temp_vector.end())
		{
			output_dir=std::string(argv[6]);
			if(argc>7)
			{
				for(int i=7;i<argc;++i)
					types.push_back(std::string(argv[i]));
			}
			else
			{
				types.insert(types.begin(),types_temp,types_temp+10);
			}
		}
		else
		{
			output_dir=std::string("MFS/");
			for(int i=6;i<argc;++i)
				types.push_back(std::string(argv[i]));
		}
	}
	else
	{
		types.insert(types.begin(),types_temp,types_temp+10);
	}

	if(_mkdir(output_dir.c_str()))
	{
		std::cout<<"Error when create direct "<<output_dir.c_str()<<'\n';
		return;
	}

	//for each origin expression
	for(int expr_index=expr_index_begin;expr_index<=expr_index_end;++expr_index)
	{
		std::cout<<" Computing MFSs for expression "<<expr_index<<" with fault type:\n \t";

		std::string index_str;
		int2str(expr_index,index_str);
		std::string expr_name=common_expr_name+index_str;

		//for each fault type
		for(int type_index=0;type_index<types.size();++type_index)
		{
			std::cout<<types[type_index]<<' ';

			for(std::list<std::string>::const_iterator it=allfilelist.begin();
				it!=allfilelist.end();++it)
			{
				int i=it->size()-1;
				for(;i>=0;--i)
					if((*it)[i]=='/')
						break;
				std::string real_file_name(it->begin()+i+1,it->end());
				std::string file_tag=std::string(expr_name)+std::string(types[type_index]);

				//for each concrete fault
				if(0==strncmp(real_file_name.c_str(),file_tag.c_str(),file_tag.size()))
				{
					std::string mfs_file_name=output_dir+
											  std::string(real_file_name.begin(),real_file_name.end()-13)+
											  std::string("_MfsSet.txt");

					std::list<std::vector<bool> > fail_tests;
					if(!getBinaryTestSuiteFromFile(*it,fail_tests))continue;

					std::set<std::vector<int> > fs_set,mfs_set;
					GetBoolFaultSchema(fail_tests,fs_set);
					GetMfsSetFromFsSet(fs_set,mfs_set);
					RecordMfs(mfs_set,mfs_file_name);
				}
			}
		}

		std::cout<<"... Done!"<<std::endl;
	}
}

int GetBoolFaultSchema(const std::list<std::vector<bool> > &failed_tests,
					   std::set<std::vector<int> > &fs_set)
{
	if(failed_tests.empty())return -1;

	int len=failed_tests.begin()->size();
	std::vector<int> schema(len,-1);

	//find all fault schemas

	//for each failed test case
	for(std::list<std::vector<bool> >::const_iterator it=failed_tests.begin();
		it!=failed_tests.end();++it)
	{
		//filtrate i-value schemas of current failed test case
		for(int i=1;i<=len;++i)
		{
			std::vector<int> index(i);
			for(int j=0;j<i;++j)
			{
				index[j]=j;
			}

			do{
				schema.clear();
				schema.resize(len,-1);

				for(int j=0;j<i;++j)
				{
					schema[index[j]]=(((*it)[index[j]])?1:0);
				}

				int k;
				std::vector<bool> test(len,false);

				//for each passed test case
				do{
					if(find(failed_tests.begin(),failed_tests.end(),test)==failed_tests.end())
					{
						k=0;
						for(;k<i;++k)
						{
							int temp_value=((test[index[k]])?1:0);
							if(schema[index[k]]!=temp_value)
								break;
						}
						if(k==i)
						{
							//current schema should be filtrated
							break;
						}
					}
				}while(next_binary(test));

				if(k!=i)
				{
					fs_set.insert(schema);
				}
			}while(next_Combination(index,len));
		}
	}

	return fs_set.size();
}

int GetMfsSetFromFsSet(const std::set<std::vector<int> > &fs_set,
					   std::set<std::vector<int> > &mfs_set)
{
	if(fs_set.empty())return -1;
	int len=fs_set.begin()->size();

	//for each fault schema
	for(std::set<std::vector<int> >::const_iterator it=fs_set.begin();
		it!=fs_set.end();++it)
	{
		//if there is not any child shcema that is fault schema
		std::set<std::vector<int> >::const_iterator iti=fs_set.begin();
		for(;iti!=fs_set.end();++iti)
		{
			if(it!=iti)
			{
				int i=0;
				for(;i<len;++i)
				{
					if((*it)[i]!=(*iti)[i] && (*iti)[i]!=-1)
					{
						break;
					}
				}
				if(i==len)//it means that *iti is a child schema of *it
				{
					break;
				}
			}
		}
		if(iti==fs_set.end())//it means that there is not any child schema of *it
		{
			mfs_set.insert(*it);
		}
	}

	return mfs_set.size();
}

void RecordMfs(const std::set<std::vector<int> > &mfs_set,
			   const std::string &filename)
{
	std::ofstream outfile;
	outfile.open(filename.c_str());
	
	for(std::set<std::vector<int> >::const_iterator it=mfs_set.begin();
		it!=mfs_set.end();++it)
	{
		for(std::vector<int>::const_iterator itv=it->begin();
			itv!=it->end();++itv)
		{
			if(*itv==-1)
				outfile<<'-';
			else
				outfile<<*itv;
		}
		outfile<<'\n';
	}

	outfile.close();
}