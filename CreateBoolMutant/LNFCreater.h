#ifndef LNF_CREATER_H
#define LNF_CREATER_H

#include "MutantCreater.h"

//An occurrence of a condition is replaced by its negation
class LNFCreater : public MutantCreater
{
protected:
    virtual void CreaterInitialize();
    virtual void CreaterCleanStates();

    virtual void CreateMutantsRun(std::vector<std::string> &expressions);

private:

    std::vector<int> m_literal_appear;
};

#endif