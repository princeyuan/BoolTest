#ifndef VRF_CREATER_H
#define VRF_CREATER_H

#include "MutantCreater.h"

class VRFCreater : public MutantCreater
{
protected:
    virtual void CreaterInitialize();
    virtual void CreaterCleanStates();

    virtual void CreateMutantsRun(std::vector<std::string> &mu_expression);

private:

    std::vector<std::string> m_varlist;

    std::vector<std::vector<int> > m_varappear;
    std::vector<int> m_varlength;
};

#endif