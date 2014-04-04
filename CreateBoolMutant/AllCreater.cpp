#include "ALLCreater.h"

#include "ASFCreater.h"
#include "ENFCreater.h"
#include "LNFCreater.h"
#include "LRFCreater.h"
#include "ORFCreater.h"
#include "VNFCreater.h"
#include "VRFCreater.h"
#include "MLFCreater.h"
#include "CCFCreater.h"
#include "CDFCreater.h"
#include "SA0Creater.h"
#include "SA1Creater.h"

void AllCreater::CreateGivenClassMutants(const std::string &expression,
										 std::vector<std::vector<std::string> > &all_mutant_exprs)
{
	MutantCreater *creater=0;
	std::vector<std::string> mutants;

	for(std::vector<std::string>::const_iterator it=m_types.begin();
		it!=m_types.end();++it)
	{
		if(!strcmp(it->c_str(),"ASF"))
		{
			creater=new ASFCreater();
		}
		else if(!strcmp(it->c_str(),"ENF"))
		{
			creater=new ENFCreater();
		}
		else if(!strcmp(it->c_str(),"ORF"))
		{
			creater=new ORFCreater();
		}
		else if(!strcmp(it->c_str(),"LNF"))
		{
			creater=new LNFCreater();
		}
		else if(!strcmp(it->c_str(),"CCF"))
		{
			creater=new CCFCreater();
		}
		else if(!strcmp(it->c_str(),"CDF"))
		{
			creater=new CDFCreater();
		}
		else if(!strcmp(it->c_str(),"LRF"))
		{
			creater=new LRFCreater();
		}
		else if(!strcmp(it->c_str(),"MLF"))
		{
			creater=new MLFCreater();
		}
		else if(!strcmp(it->c_str(),"SA0"))
		{
			creater=new SA0Creater();
		}
		else if(!strcmp(it->c_str(),"SA1"))
		{
			creater=new SA1Creater();
		}
		else if(!strcmp(it->c_str(),"VRF"))
		{
			creater=new VRFCreater();
		}
		else if(!strcmp(it->c_str(),"VNF"))
		{
			creater=new VNFCreater();
		}

		if(creater!=0)
		{
			creater->CreateMutants(expression,mutants);
			all_mutant_exprs.push_back(mutants);

			mutants.clear();
			delete creater;
			creater=0;
		}
	}
}

int AllCreater::SetMutationType(const std::vector<std::string> &types)
{
	m_types=types;
	return m_types.size();
}