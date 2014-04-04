#ifndef ASF_CREATER_H
#define ASF_CREATER_H

#include "MutantCreater.h"

#include <stack>
#include <utility>

//ASF is caused by omission of the brackets because of the misunderstanding about operator evaluation priorities
class ASFCreater : public MutantCreater
{
protected:
	virtual void Initialize(const std::string &expression);

	virtual void CreateMutantsRun(std::vector<std::string> &expressions);

private:
	std::stack<int> m_left_parenthesis;
	std::list<std::pair<int,int> > m_positions;

};

#endif