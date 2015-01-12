#ifndef BOOL_PFS_ALGORITHM_H
#define BOOL_PFS_ALGORITHM_H

#include "BoolMFSAlgorithm.h"

class SpectraInfo
{
public:
    SpectraInfo(int num_fail=0,int num_cov=0)
        :m_num_fail(num_fail),m_num_cov(num_cov)
    {}
    int m_num_fail;
    int m_num_cov;
};

void ExtractCandidateSchemas(const std::list<std::vector<int> > &fail_tests,
                             std::map<std::vector<int>,SpectraInfo> &candidate_schemas)
{
    if(fail_tests.empty())return;

    int length=fail_tests.begin()->size();

    //find all candidate failure schemas
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
                std::pair<std::vector<int>,SpectraInfo> 
                    schema(std::vector<int>(length,EMPTY_VALUE_IN_SCHEMA),SpectraInfo(1,1));
                for(int j=0;j<i;++j)
                {
                    schema.first[index[j]]=(*it)[index[j]];
                }

                int temp_size=candidate_schemas.size();
                candidate_schemas.insert(schema);

                if(candidate_schemas.size()==temp_size)
                {
                    std::map<std::vector<int>,SpectraInfo>::iterator schema_it=
                        candidate_schemas.find(schema.first);
                    if(schema_it!=candidate_schemas.end())
                    {
                        ++(schema_it->second.m_num_fail);
                        ++(schema_it->second.m_num_cov);
                    }
                }
            }while(next_Combination(index,length));
        }
    }
}

void ComputePSchemasInfo(const std::map<std::vector<int>,SpectraInfo> &candidate_schemas,
                         const std::list<std::vector<int> > &fail_tests,
                         std::map<std::vector<int>,SpectraInfo> &pschemas)
{
    if(candidate_schemas.empty())
        return;
    pschemas.insert(candidate_schemas.begin(),candidate_schemas.end());
    if(fail_tests.empty())    
        return;

    int length=fail_tests.begin()->size();
    std::vector<int> pass_test(length,0);

    do
    {
        if(find(fail_tests.begin(),fail_tests.end(),pass_test)==fail_tests.end())
        {
            for(std::map<std::vector<int>,SpectraInfo>::iterator it =pschemas.begin();
                it!=pschemas.end();++it)
            {
                if(IsSubSchema(it->first,pass_test))
                {
                    ++(it->second.m_num_cov);
                }
            }
        }
    }
    while(next_Testcase(pass_test));
}

#endif