#include "BoolTranslater.h"

void BoolTranslater::run(const std::string &source,std::string &destination)
{
	destination=source;
	
	for(int i=0;i<destination.size();i++)
	{
		if(destination[i]=='*')
		{
			destination.replace(i,1,2,'&');
		}
		if(destination[i]=='+')
		{
			destination.replace(i,1,2,'|');
		}
	}
}