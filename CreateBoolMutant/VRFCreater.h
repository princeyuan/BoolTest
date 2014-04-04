#ifndef VRF_CREATER_H
#define VRF_CREATER_H

#include "MutantCreater.h"

class VRFCreater : public MutantCreater
{
protected:
	virtual void Initialize(const std::string &expression);

	virtual void CreateMutantsRun(std::vector<std::string> &expression);

private:

	std::vector<char> m_varlist;

	std::vector<std::vector<int> > m_varappear;
};

#endif