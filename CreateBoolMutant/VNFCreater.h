#ifndef VNF_CREATER_H
#define VNF_CREATER_H

#include "MutantCreater.h"

class VNFCreater : public MutantCreater
{
protected:
	virtual void Initialize(const std::string &expression);

	virtual void CreateMutantsRun(std::vector<std::string> &expressions);

private:

	std::vector<char> m_varlist;

	std::vector<std::vector<int> > m_varappear;
};

#endif