#include "MuCreaterBuilder.h"

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

MutantCreater *MutantCreaterBuilder::Build(const std::string &mutant_type_name)
{
    MutantCreater *creater=0;

    if(0==mutant_type_name.compare("ASF"))
    {
        creater=new ASFCreater();
    }
    else if(0==mutant_type_name.compare("ENF"))
    {
        creater=new ENFCreater();
    }
    else if(0==mutant_type_name.compare("ORF"))
    {
        creater=new ORFCreater();
    }
    else if(0==mutant_type_name.compare("LNF"))
    {
        creater=new LNFCreater();
    }
    else if(0==mutant_type_name.compare("CCF"))
    {
        creater=new CCFCreater();
    }
    else if(0==mutant_type_name.compare("CDF"))
    {
        creater=new CDFCreater();
    }
    else if(0==mutant_type_name.compare("LRF"))
    {
        creater=new LRFCreater();
    }
    else if(0==mutant_type_name.compare("MLF"))
    {
        creater=new MLFCreater();
    }
    else if(0==mutant_type_name.compare("SA0"))
    {
        creater=new SA0Creater();
    }
    else if(0==mutant_type_name.compare("SA1"))
    {
        creater=new SA1Creater();
    }
    else if(0==mutant_type_name.compare("VRF"))
    {
        creater=new VRFCreater();
    }
    else if(0==mutant_type_name.compare("VNF"))
    {
        creater=new VNFCreater();
    }

    return creater;
}
