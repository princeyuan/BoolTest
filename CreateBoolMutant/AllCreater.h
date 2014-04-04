#ifndef ALL_CREATER_H
#define ALL_CREATER_H

#include <vector>
#include <string>

class AllCreater
{
public:

	void CreateGivenClassMutants(const std::string &expression,
		std::vector<std::vector<std::string> > &all_mutant_exprs);

	int SetMutationType(const std::vector<std::string> &types);

protected:

	std::vector<std::string> m_types;
};

#endif