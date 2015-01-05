#include <string.h>
#include <fstream>
#include <iostream>

int main(int argc,char **argv)
{
    std::ifstream infile;
    infile.open(argv[1]);
    if(!infile)return 0;
    char buffer[1024];
    int i=0;
    while(infile.getline(buffer,1024,'\n'))
    {
        std::cout<<++i<<'\t';
        std::cout<<buffer<<'\t';
        std::cout<<strlen(buffer)<<std::endl;
    }
    return 0;
}