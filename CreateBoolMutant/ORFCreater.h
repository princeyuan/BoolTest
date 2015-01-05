#ifndef ORF_CREATER_H
#define ORF_CREATER_H

#include "MutantCreater.h"

//An occurrence of a logical connective + replaced by * or vice versa
class ORFCreater : public MutantCreater
{
protected:
    virtual void CreaterInitialize();
    virtual void CreaterCleanStates();

    virtual void CreateMutantsRun(std::vector<std::string> &expressions);

private:
    std::list<int> m_operator_positions;
};

#endif