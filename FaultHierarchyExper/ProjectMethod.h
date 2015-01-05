#ifndef FHE_PROJECT_METHOD_H
#define FHE_PROJECT_METHOD_H

#include <set>
#include <list>
#include <string>

#include "Tests/TestReq.h"
#include "Tests/TestCase.h"

using namespace TestReduce;

void ReadReqs(const std::list<std::string> &allfilelist,
			  const char *expr_name, const char *fault_type,
			  std::vector<TestReq*> &reqs);

void GetTestsFromReqs(const std::vector<TestReq*> &reqs,
					  std::vector<TestCase*> &tests);

void GetTestsValueFromReqs(const std::vector<TestReq*> &reqs,
						   std::set<int> &tests_value);

int ComputeKillNumber(const std::list<TestCase*> &tests,
					  const std::vector<TestReq*> &object_reqs);

#endif