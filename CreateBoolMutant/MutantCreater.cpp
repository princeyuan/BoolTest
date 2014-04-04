#include "MutantCreater.h"

void MutantCreater::CreateMutants(const std::string &expression,
									  std::vector<std::string>  &mutate_expressions)
{
	m_expression=expression;

	Initialize(expression);

	CreateMutantsRun(mutate_expressions);
}