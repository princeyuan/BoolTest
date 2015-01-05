#ifndef ENF_CREATER_H
#define ENF_CREATER_H

#include "MutantCreater.h"

//A sub-expression (except conditions) replaced by its negation
class ENFCreater : public MutantCreater
{
protected:
    virtual void CreaterInitialize();
    virtual void CreaterCleanStates();

    virtual void CreateMutantsRun(std::vector<std::string> &expressions);

private:
    std::list<int> m_left_brackets_positions;

};

#endif