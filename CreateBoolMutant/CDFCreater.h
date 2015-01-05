#ifndef CDF_CREATER_H
#define CDF_CREATER_H

#include "MutantCreater.h"

//An occurrence of condition c is replaced by c+c¡ä, in which c¡ä is a possible condition
class CDFCreater : public MutantCreater
{
protected:
    virtual void CreaterInitialize();
    virtual void CreaterCleanStates();

    virtual void CreateMutantsRun(std::vector<std::string> &expressions);

private:

    std::vector<std::string> m_varlist;

    std::vector<int> m_varlength;

    std::vector<std::pair<int,int> > m_literal_var_map;
};

#endif