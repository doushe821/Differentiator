#ifndef VARIABLE_TABLE_H_INCLUDED
#define VARIABLE_TABLE_H_INCLUDED

#include <stdlib.h>

const size_t MAX_VARIABLES = 16;
const size_t VARIABLE_NAME_MAX = 128;

struct Variable_t
{
    char name[VARIABLE_NAME_MAX];
    double value;
};

struct VariableTable_t
{
    Variable_t* Vars;
    int Index;
};

#endif