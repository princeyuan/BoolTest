#ifndef MUTANT_CREATER_BUILDER_H
#define MUTANT_CREATER_BUILDER_H

#include <string>
#include "MutantCreater.h"

class MutantCreaterBuilder
{
public:
    MutantCreater *Build(const std::string &mutant_type_name);
};

#endif
