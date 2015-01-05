#ifndef VNF_CREATER_H
#define VNF_CREATER_H

#include "MutantCreater.h"

class VNFCreater : public MutantCreater
{
protected:
    virtual void CreaterInitialize();
    virtual void CreaterCleanStates();

    virtual void CreateMutantsRun(std::vector<std::string> &mu_expressions);

private:

    std::vector<std::vector<int> > m_varappear;
};

#endif