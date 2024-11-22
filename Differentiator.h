#ifndef DIFFERENTIATOR_H_INCLUDED
#define DIFFERENTIATOR_H_INCLUDED

#include <stdlib.h>

#include "DBTools/DBTools.h"
#include "Tree/Tree.h"
#include "Tree/NodeStruct.h"

enum Errors 
{
    NULL_PTR_PARAM = -12,
    SYNTAX_ERROR,
    OPERATION_NOT_FOUND,
    NO_ERRORS = 0,
};

const size_t DIFF_NODE_SIZE = BASE_NODE_SIZE + 16 + 8;

#define NewNode(type, parent, value, left, right) CreateNode(type, parent, sizeof(double), (value), 2, left, right)

const size_t OP_NAME_MAX = 7;

struct operation
{
    char name[OP_NAME_MAX];
    int code;
    int priority;
};

const size_t NUMBER_OF_OPERATIONS = 23;
const size_t NUMBER_OF_ARG_TYPES = 3;

struct argtype 
{
    char name[OP_NAME_MAX];
    int code;
};

const int CONST_VALUE_TYPE_CODE = 0;
const int OPERATION_TYPE_CODE = 1;
const int VARIABLE_VALUE_TYPE_CODE = 2;

const operation operations[NUMBER_OF_OPERATIONS] = 
{
    {"+"     , 1 , 0},
    {"-"     , 2 , 0},
    {"*"     , 3 , 1},
    {"/"     , 4 , 1},
    {"^"     , 5 , 2},
    {"sin"   , 6 , 2},
    {"cos"   , 7 , 2},
    {"tg"    , 8 , 2},
    {"ctg"   , 9 , 2},
    {"arcsin", 10, 2},
    {"arccos", 10, 2},
    {"arctg" , 11, 2},
    {"arcctg", 12, 2},
    {"sh"    , 13, 2},
    {"ch"    , 14, 2},
    {"th"    , 15, 2},
    {"cth"   , 16, 2},
    {"arcsh" , 17, 2},
    {"arcch" , 18, 2},
    {"arcth" , 19, 2},
    {"arccth", 20, 2},
    {"log"   , 21, 2},
    {"ln"    , 22, 2},
};

int GetLongOpCode(char* buffer);
int GetShortOpCode(int code);

#endif