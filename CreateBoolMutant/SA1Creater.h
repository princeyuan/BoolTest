#ifndef SA1_CREATER_H
#define SA1_CREATER_H

#include "MutantCreater.h"

//An occurrence of a condition is replaced by 1
class SA1Creater : public MutantCreater
{
protected:
    virtual void CreaterInitialize();
    virtual void CreaterCleanStates();

    virtual void CreateMutantsRun(std::vector<std::string> &mu_expressions);

private:
    
    std::vector<int> m_literal_appear;
    std::vector<int> m_literal_length;
};

#endif