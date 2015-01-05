#ifndef MLF_CREATER_H
#define MLF_CREATER_H

#include "MutantCreater.h"

//An occurrence of a condition is omitted in the expression
class MLFCreater : public MutantCreater
{
protected:
    virtual void CreaterInitialize();
    virtual void CreaterCleanStates();

    virtual void CreateMutantsRun(std::vector<std::string> &expressions);

private:
    
    std::vector<int> m_literal_appear;

    std::vector<int> m_literal_length;
};

#endif