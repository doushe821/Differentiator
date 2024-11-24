#ifndef DIFFERENTIATOR_H_INCLUDED
#define DIFFERENTIATOR_H_INCLUDED

#include <stdlib.h>

#include "DBTools/DBTools.h"
#include "Tree/Tree.h"
#include "Tree/NodeStruct.h"
#include "List/List.h"

enum Errors 
{
    NULL_PTR_PARAM = -12,
    SYNTAX_ERROR,
    LIST_INITIALIZATION_FAILURE,
    READING_ERROR,
    ALLOCATION_FAILURE,
};

struct NodeTypeIdentifier
{
    int Type;
    double Value;
    int Code;
    int VarName;
};

const size_t DIFF_NODE_SIZE = BASE_NODE_SIZE + 16 + 8;

#define NewNode(type, parent, value, left, right) CreateNode(type, parent, sizeof(double), (value), 2, left, right)

const size_t OP_NAME_MAX = 7;


const size_t NUMBER_OF_OPERATIONS = 5;
const size_t NUMBER_OF_ARG_TYPES = 3;
const size_t NUMBER_OF_FUNTIONS = 18;
const int OPERATION_NOT_FOUND = -10;
const int FUNCTION_NOT_FOUND = -11;

struct argtype 
{
    char name[OP_NAME_MAX];
    int code;
};

const int CONST_VALUE_TYPE_CODE = 0;
const int OPERATION_TYPE_CODE = 1;
const int VARIABLE_VALUE_TYPE_CODE = 2;
const int FUNCTION_TYPE_CODE = 3;

struct operation
{
    char name[OP_NAME_MAX];
    int code;
    int priority;
};


const operation operations[NUMBER_OF_OPERATIONS] = 
{
    {"+"     , 1 , 0},
    {"-"     , 2 , 0},
    {"*"     , 3 , 1},
    {"/"     , 4 , 1},
    {"^"     , 5 , 2},
};

struct function
{
    char name[OP_NAME_MAX];
    int code;
};

const function functions[NUMBER_OF_FUNTIONS]
{
    {"sin"   ,       1},
    {"cos"   ,       2},
    {"arcsin",       3},
    {"arccos",       4},
    {"tg"    ,       5},
    {"ctg"   ,       6},
    {"arctg" ,       7},
    {"arcctg",       8},
    {"sh"    ,       9},
    {"ch"    ,      10},
    {"arcsh" ,      11},
    {"arcch" ,      12},
    {"th"    ,      13},
    {"cth"   ,      14},
    {"arcth" ,      15},
    {"arccth",      16},
    {"log"   ,      17},
    {"ln"    ,      18},
}; 

int GetFuncCode(const char* buffer);
    
NodeTypeIdentifier GetNodeType(const char* SubExpression);
Node_t ReadNode(const char* ExpressionString, size_t* StringIndex, List_t* List, Node_t Parent ON_DEBUG(COMMA size_t* iteration));
size_t GetFileSize(FILE* fp);

void EmergentDtor(FILE* fp, List_t* NodeList, char* ExpressionString, Node_t root, FILE* dmp);

#endif