#ifndef LRF_CREATER_H
#define LRF_CREATER_H

#include "MutantCreater.h"

//An occurrence of a condition is replaced by another possible condition
class LRFCreater : public MutantCreater
{
protected:
	virtual void Initialize(const std::string &expression);

	virtual void CreateMutantsRun(std::vector<std::string> &expression);

private:

	std::vector<char> m_varlist;

	std::vector<int> m_literal_appear;
};

#endif