#ifndef MUTANT_CREATER_H
#define MUTANT_CREATER_H

#include <map>
#include <list>
#include <vector>
#include <string>

#include <iostream>

class MutantCreater
{
public:
    void CreateMutants(const std::string &expression,std::vector<std::string> &mutate_expressions);

protected:
    //Initialize states (member variables) befor creating mutants
    virtual void CreaterInitialize()=0;

    //Clean states before initializing them and after creating mutants
    virtual void CreaterCleanStates()=0;

    //Creating mutants with give mutant type
    virtual void CreateMutantsRun(std::vector<std::string> &mu_expressions)=0;
        
protected:
    //Check if there is a variable begin at position 'index' in 'expression',
    //'var_length' outputs the length of variable or 0 if the function returns TRUE or FALSE. 
    bool IsBooleanVariable(const std::string &expression,int index,
                           int &var_length);

    //Scan 'expression', record the position and the length of each literal
    void RecordLiteralAppearAndLength(const std::string &expression,
                                      std::vector<int> &literal_appear_list,
                                      std::vector<int> &literal_length_list);

    //Scan 'expression', record variable list, 
    //and record the position of each appearance of each variable,
    //and record the length of each variable
    void RecordVarListAndAppearAndLength(const std::string &expression,
                                         std::vector<std::string> &var_list,
                                         std::vector<std::vector<int> > &var_appear_list,
                                         std::vector<int> &var_length_list);

    //Scan 'expression', record variable list,
    //and record the length of each variable,
    //and give a map from the position of each literal to the index of corresponding variable
    void RecordVarListAndLengthAndMap(const std::string &expression,
                                      std::vector<std::string> &var_list,
                                      std::vector<int> &var_length_list,
                                      std::vector<std::pair<int,int> > &literal_var_map);

protected:

    std::string m_expression;
};

#endif