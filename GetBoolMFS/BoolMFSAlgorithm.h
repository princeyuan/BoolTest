#ifndef BOOL_MFS_ALGORITHM_H
#define BOOL_MFS_ALGORITHM_H

#include <map>
#include <list>
#include <string>
#include <vector>
#include <algorithm>

#define EMPTY_VALUE_IN_SCHEMA (-1)

void int2str(int number,std::string &str)
{
    str.clear();
    while(number!=0)
    {
        str.insert(str.begin(),'0'+(number%10));
        number=number/10;
    }
}

bool next_Testcase(std::vector<int> &test_case)
{
    int last=test_case.size()-1;

    if(test_case[last]==0)
    {
        test_case[last]=1;
        return true;
    }

    if(test_case[last]==1)
    {
        test_case[last]=0;
        for(int i=last-1;i>=0;--i)
        {
            if(test_case[i]==0)
            {
                test_case[i]=1;
                return true;;
            }
            else //test_case[i]==0
            {
                test_case[i]=0;
            }
        }
        return false;
    }

    return false;
}

bool next_Combination(std::vector<int> &combination,int num_elements)
{
    std::vector<int> temp=combination;
    int strength=combination.size();
    bool flag=false;

    for(int i=0;i<strength;i++)
    {
        if(combination[i]!=num_elements-strength+i)
            flag=true;
    }

    if(flag)
    {
        int i;
        for(i=0;i<strength-1;i++)
        {
            if(combination[i]+1!=combination[i+1])
            {
                combination[i]++;
                break;
            }
        }
        if(i==strength-1)
            combination[i]++;
        for(int j=0;j<i;j++)
            combination[j]=j;
    }

    return flag;
}

//Check if s1 is sub-schema of s2
bool IsSubSchema(const std::vector<int> &s1, const std::vector<int> &s2)
{
    if(s1.size()!=s2.size())
    {
        return false;
    }
    for(int i=0;i<s1.size();++i)
    {
        if(s1[i]!=s2[i] && s1[i]!=EMPTY_VALUE_IN_SCHEMA)
        {
            return false;
        }
    }
    return true;
}

/*--------------------------------------------------*/

typedef int SchemaStrength;

void ExtractFailureSchemas(const std::list<std::vector<int> > &fail_tests,
                           std::map<std::vector<int>,SchemaStrength> &fs_set)
{
    if(fail_tests.empty())return;

    int length=fail_tests.begin()->size();

    //find all failure schemas
    //for each failed test case
    for(std::list<std::vector<int> >::const_iterator it=fail_tests.begin();
        it!=fail_tests.end();++it)
    {
        //for each i-value schemas in current failed test case
        for(int i=1;i<=length;++i)
        {
            std::vector<int> index(i);
            for(int j=0;j<i;++j)
            {
                index[j]=j;
            }

            //for each non-empty parameter set with i parameters
            do{
                std::pair<std::vector<int>,SchemaStrength> 
                    schema(std::vector<int>(length,EMPTY_VALUE_IN_SCHEMA),i);
                for(int j=0;j<i;++j)
                {
                    schema.first[index[j]]=(*it)[index[j]];
                }

                std::vector<int> pass_test(length,0);
                int k=-1;

                //filter by each passed test case
                do{
                    if(find(fail_tests.begin(),fail_tests.end(),pass_test)==fail_tests.end())
                    {
                        for(k=0;k<i;++k)
                        {
                            if(schema.first[index[k]]!=pass_test[index[k]])
                                break;
                        }
                        if(k==i)
                        {
                            //current schema should be filtrated
                            break;
                        }
                    }
                }while(next_Testcase(pass_test));

                if(k!=i)
                {
                    fs_set.insert(schema);
                }
            }while(next_Combination(index,length));
        }
    }
}

void GetMFSSetFromFSSet(const std::map<std::vector<int>,SchemaStrength> &fs_set,
                        std::map<std::vector<int>,SchemaStrength> &mfs_set)
{
    if(fs_set.empty())return;

    typedef std::list<std::map<std::vector<int>,SchemaStrength>::const_iterator> IterList;

    int length=fs_set.begin()->first.size();
    std::vector<IterList > cache(length);
    for(std::map<std::vector<int>,SchemaStrength>::const_iterator it_fs=fs_set.begin();
        it_fs!=fs_set.end();++it_fs)
    {
        cache[it_fs->second-1].push_back(it_fs);
    }

    for(int i=0;i<length;++i)//handle all (i+1)-value schemas 
    {
        if(!cache[i].empty())
        {
            for(IterList::const_iterator it_schemait=cache[i].begin();
                it_schemait!=cache[i].end();++it_schemait)
            {
                int j;

                for(j=i+1;j<length;++j)//handle all (j+1)-value schemas (where j>i)
                {
                    IterList::iterator it_higher_schemait=cache[j].begin();
                    while(it_higher_schemait!=cache[j].end())
                    {
                        if(IsSubSchema((*it_schemait)->first,(*it_higher_schemait)->first))
                        {
                            it_higher_schemait=cache[j].erase(it_higher_schemait);
                            //erase返回的iterator为当前iterator的后续
                        }
                        else
                        {
                            ++it_higher_schemait;
                        }
                    }
                }
            }
        }
    }

    for(int i=0;i<length;++i)
    {
        if(!cache[i].empty())
        {
            for(IterList::const_iterator it_schemait=cache[i].begin();
                it_schemait!=cache[i].end();++it_schemait)
            {
                mfs_set.insert(**it_schemait);
            }
        }
    }
}

/*--------------------------------------------------*/

typedef bool HaveChildOrNot;

void ExtractFailureSchemas(
        const std::list<std::vector<int> > &fail_tests,
        std::list<std::vector<std::list<std::pair<std::vector<int>,HaveChildOrNot> > > > &fs)
{
    if(fail_tests.empty())return;

    int length=fail_tests.begin()->size();

    for(std::list<std::vector<int> >::const_iterator it=fail_tests.begin();
        it!=fail_tests.end();++it)
    {
        std::vector<std::list<std::pair<std::vector<int>,HaveChildOrNot> > > one_fs(length);

        std::list<std::list<int> > diff_params_list;

        //diff_param_list
        std::vector<int> pass_test(length,0);
        do
        {
            if(find(fail_tests.begin(),fail_tests.end(),pass_test)==fail_tests.end())
            {
                std::list<int> diff_params;
                for(int i=0;i<length;++i)
                {
                    if((*it)[i]!=pass_test[i])
                    {
                        diff_params.push_back(i);
                    }
                }
                diff_params_list.push_back(diff_params);
            }
        }
        while(next_Testcase(pass_test));

        //construct schemas
        std::list<std::vector<int> > *cur_list=NULL;
        std::list<std::vector<int> > *low_list=NULL;
        for(int i=1;i<=length;++i)
        {
            if(i==1)
            {
                cur_list=new std::list<std::vector<int> >();
            }
            if(i!=1)
            {
                low_list=cur_list;
                cur_list=new std::list<std::vector<int> >();
            }

            std::vector<int> index(i);
            for(int j=0;j<i;++j)
            {
                index[j]=j;
            }
            do
            {
                bool flag=true;
                for(std::list<std::list<int> >::const_iterator it_diff=diff_params_list.begin();
                    it_diff!=diff_params_list.end();++it_diff)
                {
                    std::vector<int> temp(i);
                    //elements in elements in diff_params_list are sorted
                    //elements in index are sorted
                    std::vector<int>::iterator temp_it=
                        set_intersection(it_diff->begin(),it_diff->end(),
                                         index.begin(),index.end(),
                                         temp.begin());
                    if(temp_it==temp.begin())
                    {
                        flag=false;
                        break;
                    }
                }

                if(flag)
                {
                    std::vector<int> schema(length,EMPTY_VALUE_IN_SCHEMA);
                    for(int j=0;j<i;++j)
                    {
                        schema[index[j]]=(*it)[index[j]];
                    }

                    cur_list->push_back(index);

                    bool has_child=false;
                    if(i!=1)
                    {       
                        for(std::list<std::vector<int> >::const_iterator it_low=low_list->begin();
                            it_low!=low_list->end();++it_low)
                        {
                            std::vector<int> temp(i);
                            std::vector<int>::iterator temp_it=
                                set_difference(it_low->begin(),it_low->end(),
                                               index.begin(),index.end(),
                                               temp.begin());
                            if(temp_it==temp.begin())
                            {
                                has_child=true;
                            }
                        }
                    }

                    one_fs[i-1].push_back(std::pair<std::vector<int>,HaveChildOrNot>(schema,has_child));
                }
            }
            while(next_Combination(index,length));

            if(i!=1 && i!=length)
            {
                delete low_list;
                low_list=NULL;
            }
            else if(i==length)
            {
                delete low_list;
                delete cur_list;
                low_list=NULL;
                cur_list=NULL;
            }
        }

        fs.push_back(one_fs);
    }
}

void GetMFSSetFromFSSet(
        const std::list<std::vector<std::list<std::pair<std::vector<int>,HaveChildOrNot> > > > &fs,
        std::set<std::vector<int> > &mfs_set)
{
    int length=fs.begin()->size();
    //typedef std::list<std::list<std::pair<std::vector<int>,HaveChildOrNot> >::const_iterator> IterList;
    //std::vector<IterList > cache(length);

    std::list<std::vector<std::list<std::pair<std::vector<int>,HaveChildOrNot> > > >::const_iterator it;
    for(it=fs.begin();it!=fs.end();++it)
    {
        for(int i=0;i<it->size();++i)
        {
            std::list<std::pair<std::vector<int>,HaveChildOrNot> >::const_iterator it_sche;
            for(it_sche=(*it)[i].begin();it_sche!=(*it)[i].end();++it_sche)
            {
                if(!(it_sche->second))
                {
                    //cache[i].push_back(it_sche);
                    mfs_set.insert(it_sche->first);
                }
            }
        }
    }

    //貌似这一步没有必要?
    /*for(int i=0;i<length;++i)//handle all (i+1)-value schemas 
    {
        if(!cache[i].empty())
        {
            for(IterList::const_iterator it_schemait=cache[i].begin();
                it_schemait!=cache[i].end();++it_schemait)
            {
                int j;

                for(j=i+1;j<length;++j)//handle all (j+1)-value schemas (where j>i)
                {
                    IterList::iterator it_higher_schemait=cache[j].begin();
                    while(it_higher_schemait!=cache[j].end())
                    {
                        if(IsSubSchema((*it_schemait)->first,(*it_higher_schemait)->first))
                        {
                            it_higher_schemait=cache[j].erase(it_higher_schemait);
                            //erase返回的iterator为当前iterator的后续
                        }
                        else
                        {
                            ++it_higher_schemait;
                        }
                    }
                }
            }
        }
    }*/

    /*for(int i=0;i<length;++i)
    {
        if(!cache[i].empty())
        {
            for(IterList::const_iterator it_schemait=cache[i].begin();
                it_schemait!=cache[i].end();++it_schemait)
            {
                mfs_set.insert((*it_schemait)->first);
            }
        }
    }*/
}

#endif