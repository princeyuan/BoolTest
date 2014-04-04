/*#ifndef BOOL_APFD_H
#define BOOL_APFD_H

#include <vector>
#include <Tests/TestCase.h>
#include <Test/TestReq.h>

class BoolAPFD
{
public:

	float operator() (const std::vector<TestReq *> &reqs,
					  const std::vector<TestCase *> &tests,
					  const std::vector<int> &sorted_test_index,
					  std::vector<int> &increment)
	{
		std::vector<int> flag(0,reqs.size());

		for(int i=0;i<sorted_test_index.size();++i)
		{
			tests[i]

			int new_kill_num=0;
			for(std::vector<TestReq*>::const_iterator it=reqs.begin();
				it!=reqs.end();++it)
			{
				if((*it)->satisfy(*(test[i])))
					++new_kill_num;
			}
		}
	}
};

#endif*/