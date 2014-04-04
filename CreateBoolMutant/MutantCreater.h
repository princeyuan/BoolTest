#ifndef MUTANT_CREATER_H
#define MUTANT_CREATER_H

#include <list>
#include <vector>
#include <string>

#include <iostream>

class MutantCreater
{
public:
	void CreateMutants(const std::string &expression,std::vector<std::string> &mutate_expressions);

protected:
	virtual void Initialize(const std::string &expression)=0;

	virtual void CreateMutantsRun(std::vector<std::string> &expressions)=0;

protected:

	std::string m_expression;
};

#endif