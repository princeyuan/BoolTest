#include "MutantCreater.h"

void MutantCreater::CreateMutants(const std::string &expression,
                                  std::vector<std::string>  &mutate_expressions)
{
    m_expression=expression;
    CreaterCleanStates();
    CreaterInitialize();

    CreateMutantsRun(mutate_expressions);

    CreaterCleanStates();
    m_expression.clear();
}

bool MutantCreater::IsBooleanVariable(const std::string &expression,int index,int &var_length)
{
    if((expression[index]>='a' && expression[index]<='z') || 
        (expression[index]>='A' && expression[index]<='Z'))
    {
        int k=1;
        while(index+k<=expression.size()-1 && 
            expression[index+k]!='!' && 
            expression[index+k]!='+' && 
            expression[index+k]!='*' &&
            expression[index+k]!='(' &&
            expression[index+k]!=')')
            ++k;
        var_length=k;

        return true;
    }
    else
    {
        var_length=0;
        return false;
    }
}

void MutantCreater::RecordLiteralAppearAndLength(const std::string &expression,
                                                 std::vector<int> &literal_appear_list,
                                                 std::vector<int> &literal_length_list)
{
    int var_length=0;
    int exp_length=expression.size();

    //for each symbol of boolean expression
    for(int i=0;i<exp_length;i++)
    {
        //if the symbol is boolean variable 
        if(IsBooleanVariable(expression,i,var_length))
        {
            //record appear location of each literal
            literal_appear_list.push_back(i);
            //record the length of each literal
            literal_length_list.push_back(var_length);
            //index i jump to the end of this literal
            i=i+var_length-1;
        }
    }
}

void MutantCreater::RecordVarListAndAppearAndLength(const std::string &expression,
                                                    std::vector<std::string> &var_list,
                                                    std::vector<std::vector<int> > &var_appear_list,
                                                    std::vector<int> &var_length_list)
{
    int var_length=0;
    int exp_length=expression.size();

    //for each symbol of boolean expression
    for(int i=0;i<exp_length;i++)
    {
        //if the symbol is boolean variable 
        if(IsBooleanVariable(expression,i,var_length))
        {
            int list_length=var_list.size();
            //check whether the variable has been contained in variable list
            int j;
            for(j=0;j<list_length;j++)
            {
                if((var_length==var_list[j].length()) && 
                    (expression.compare(i,var_length,var_list[j])==0))
                    break;
            }

            //if has not been contained, add it in
            if(j==list_length)
            {
                var_list.push_back(expression.substr(i,var_length));
                std::vector<int> index_temp;
                index_temp.push_back(i);
                var_appear_list.push_back(index_temp);
                var_length_list.push_back(var_length);
            }
            //else, record the current appear location
            else
                var_appear_list[j].push_back(i);

            //index i jump to the end of this literal
            i=i+var_length-1;
        }
    }
}

void MutantCreater::RecordVarListAndLengthAndMap(const std::string &expression,
                                                 std::vector<std::string> &var_list,
                                                 std::vector<int> &var_length_list,
                                                 std::vector<std::pair<int,int> > &literal_var_map)
{
    int var_length=0;
    int exp_length=expression.size();

    //for each symbol of boolean expression
    for(int i=0;i<exp_length;i++)
    {
        //if the symbol is boolean variable 
        if(IsBooleanVariable(expression,i,var_length))
        {
            int list_length=var_list.size();
            //check whether the variable has been contained in variable list
            int j;
            for(j=0;j<list_length;j++)
            {
                if((var_length==var_list[j].length()) && 
                    (expression.compare(i,var_length,var_list[j])==0))
                    break;
            }

            //if has not been contained, add it in
            if(j==list_length)
            {
                var_list.push_back(expression.substr(i,var_length));
                var_length_list.push_back(var_length);
                literal_var_map.push_back(std::pair<int,int>(i,j));
            }

            //else, record the current appear location
            else
                literal_var_map.push_back(std::pair<int,int>(i,j));

            //index i jump to the end of this literal
            i=i+var_length-1;
        }
    }
}