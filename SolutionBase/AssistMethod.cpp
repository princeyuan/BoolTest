#include "AssistMethod.h"

/*int pow(int a,int b)
{
    int r=1;
    for(int i=0;i<b;i++)
        r*=a;
    return r;
}*/

bool next_Binary(std::vector<int> &bin_tuple)
{
	int last=bin_tuple.size()-1;

	if(bin_tuple[last]==0)
	{
		bin_tuple[last]=1;
		return true;
	}

	if(bin_tuple[last]==1)
	{
		bin_tuple[last]=0;
		for(int i=last-1;i>=0;--i)
		{
			if(bin_tuple[i]==0)
			{
				bin_tuple[i]=1;
				return true;;
			}
			else //test[i]==true
			{
				bin_tuple[i]=0;
			}
		}
		return false;
	}

	return false;
}

bool next_Combination(std::vector<int> &combination,int num_elements)
{
    std::vector<int> temp=combination;
    int strength=combination.size();
    bool flag=false;

    for(int i=0;i<strength;i++)
    {
        if(combination[i]!=num_elements-strength+i)flag=true;
    }

    if(flag)
    {
        int i;
        for(i=0;i<strength-1;i++)
        {
            if(combination[i]+1!=combination[i+1])
            {
                combination[i]++;
                break;
            }
        }
        if(i==strength-1)combination[i]++;
        for(int j=0;j<i;j++)combination[j]=j;
    }

    return flag;
};

int getOneBinaryTestFromLine(char* str,std::vector<int> &test)
{
    test.resize(strlen(str));
    for(int i=0;i<strlen(str);++i)
    {
        if(str[i]=='1')
            test[i]=1;
        else if(str[i]=='0')
            test[i]=0;
    }
    return test.size();
}

bool getBinaryTestSuiteFromFile(const std::string &file,std::list<std::vector<int> > &tests)
{
    std::ifstream infile;
    infile.open(file.c_str());
    if(!infile)return false;

    char buffer[256];
    while(infile.getline(buffer,100,'\n'))
    {
        std::vector<int> temp;
        if(getOneBinaryTestFromLine(buffer,temp)!=0)//length of temp is not 0
            tests.push_back(temp);
    }

    infile.close();
    return true;
}

char* getOneIntFromLine(char *str,int &num)
{
    bool positive=true;
    int i=0;
    int length=strlen(str);

loop:
    while ((str[i]<'0' || str[i]>'9') && (str[i]!='-') && (i<length))
    {
        ++i;
    }

    if(i==length)
    {
        return NULL;
    }
    else if(str[i]=='-')
    {
        ++i;
        positive=false;
    }
    
    std::string token;
    if(str[i]>='0' && str[i]<='9')
    {
        int j=0;
        while((str[i]<='9') && (str[i]>='0') && (i<length))
        {
            token.push_back(str[i++]);
        }
        num=atoi(token.c_str());
        if(positive==false)
            num=-num;
    }
    else
    {
        ++i;
        goto loop;
    }

     str=&str[i];
    return str;
}

int getAllIntsFromLine(char* str,std::vector<int> &nums)
{
    int num;
    std::list<int> temp;
    while(*str!='\0')
    {
        str=getOneIntFromLine(str,num);
        if(str!=NULL)
        {
            temp.push_back(num);
        }
        else
            break;
    }

    nums.clear();
    nums.assign(temp.begin(),temp.end());

    return nums.size();
}

/*void int2str(int number,std::string &str,int len)
{
    str.clear();
    for(int i=0;i<len;i++)
    {
        int sym_num=number/((int)pow(10,len-1-i));
        str.append(1,(char)(sym_num+'0'));
        number=number%((int)pow(10,len-1-i));
    }
}*/

void int2str(int number,std::string &str)
{
    str.clear();
    while(number!=0)
    {
        str.insert(str.begin(),'0'+(number%10));
        number=number/10;
    }
}

void GetAllFileList(const char *allfilelist_file_name,
                    std::list<std::string> &allfilelist)
{
    int i=strlen(allfilelist_file_name)-1;
    for(;i>=0;--i)
        if(allfilelist_file_name[i]=='/')
            break;
    std::string dir(allfilelist_file_name,allfilelist_file_name+i+1);

    std::ifstream infile;
    infile.open(allfilelist_file_name);
    if(!infile)return;
    char buffer[1024];
    while(infile.getline(buffer,100,'\n'))
        allfilelist.push_back(dir+std::string(buffer));
    infile.close();
}