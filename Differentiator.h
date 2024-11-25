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

const size_t DIFF_NODE_SIZE = BASE_NODE_SIZE + 16 + 8;

struct DiffBaseNode
{
    size_t type;
    Node_t parent;
    size_t fertility;
    size_t kid1;
    size_t kid2;
    size_t datasize;
    double data;
};

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

const BaseNode FILLER_NODE = {};

struct operation
{
    char name;
    int code;
    int DiffFunc(List_t* list, Node_t Node);
};
const operation operations[NUMBER_OF_OPERATIONS] = 
{
    {'+'     , 1, DiffSum},
    {'-'     , 2, DiffSum},
    {'*'     , 3, DiffMul},
    {'/'     , 4, DiffDiv},
    {'^'     , 5, DiffPow},
};

struct function
{
    char name[OP_NAME_MAX];
    int code;
    int DiffFunc(List_t* list, Node_t Node);
};
const function functions[NUMBER_OF_FUNTIONS]
{
    {"sin"   ,       1,    FDiffSin},
    {"cos"   ,       2,    FDiffCos},
    {"arcsin",       3, FDiffArcsin},
    {"arccos",       4, FDiffArccos},
    {"tg"    ,       5,     FDiffTg},
    {"ctg"   ,       6,    FDiffCtg},
    {"arctg" ,       7,  FDiffArctg},
    {"arcctg",       8, FDiffArcctg},
    {"sh"    ,       9,     FDiffSh},
    {"ch"    ,      10,     FDiffCh},
    {"arcsh" ,      11, FDiffArchsh},
    {"arcch" ,      12,  FDiffArcch},
    {"th"    ,      13,     FDiffTh},
    {"cth"   ,      14,    FDiffCth},
    {"arcth" ,      15,  FDiffArcth},
    {"arccth",      16, FDiffArccth},
    {"log"   ,      17,    FDiffLog},
    {"ln"    ,      18,     FDiffLn},
}; 

int GetFuncCode(const char* buffer);
    
Node_t ReadNode(const char* ExpressionString, size_t* StringIndex, List_t* List, Node_t Parent ON_DEBUG(COMMA size_t* iteration));
size_t GetFileSize(FILE* fp);

void EmergentDtor(FILE* fp, List_t* NodeList, char* ExpressionString, Node_t root, FILE* dmp);

Node_t CreateNodeList(List_t* list, size_t type, Node_t parent, void* data, Node_t left, Node_t right);

int GetOpCode(int code);
#endif