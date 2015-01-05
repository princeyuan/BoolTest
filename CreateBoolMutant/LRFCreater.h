#ifndef LRF_CREATER_H
#define LRF_CREATER_H

#include "MutantCreater.h"

//An occurrence of a condition is replaced by another possible condition
class LRFCreater : public MutantCreater
{
protected:
    virtual void CreaterInitialize();
    virtual void CreaterCleanStates();

    virtual void CreateMutantsRun(std::vector<std::string> &expression);

private:

    std::vector<std::string> m_varlist;

    std::vector<int> m_varlength;

    std::vector<std::pair<int,int> > m_literal_var_map;
};

#endif