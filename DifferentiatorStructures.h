#ifndef DIFFERENTIATOR_STRUCTURES_H_INCLUDED
#define DIFFERENTIATOR_STRUCTURES_H_INCLUDED

#include "Tree/Tree.h"
#include "Tree/NodeStruct.h"

enum Errors 
{
    NULL_PTR_PARAM = -12,
    SYNTAX_ERROR,
    LIST_INITIALIZATION_FAILURE,
    READING_ERROR,
    ALLOCATION_FAILURE,
};

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

struct Status
{
    int ErrorCode;
    int Line;
    char File[FILENAME_MAX];
};

Status DiffSum(Node_t Node);
//Status DiffMul(List_t* list, Node_t Node);
//Status DiffDiv(List_t* list, Node_t Node);
//Status DiffPow(List_t* list, Node_t Node);

struct operation_t
{
    char name;
    int code;
    //Status (*DiffFunc)(Node_t Node);
};
const operation_t operations[NUMBER_OF_OPERATIONS] = 
{
    {'+'     , 1},//, DiffSum},
    {'-'     , 2},//, DiffSum},
    {'*'     , 3},//, DiffMul},
    {'/'     , 4},//, DiffDiv},
    {'^'     , 5},//, DiffPow},
};

/*Status FDiffSin   (List_t* List, Node_t Node);
Status FDiffCos   (List_t* List, Node Node);
Status FDiffArcsin(List_t* List, Node Node);
Status FDiffArccos(List_t* List, Node Node);
Status FDiffTg    (List_t* List, Node Node);
Status FDiffCtg   (List_t* List, Node Node);
Status FDiffArctg (List_t* List, Node Node);
Status FDiffArcctg(List_t* List, Node Node);
Status FDiffSh    (List_t* List, Node Node);
Status FDiffCh    (List_t* List, Node Node);
Status FDiffArcsh (List_t* List, Node Node);
Status FDiffArcch (List_t* List, Node Node);
Status FDiffTh    (List_t* List, Node Node);
Status FDiffCth   (List_t* List, Node Node);
Status FDiffArcth (List_t* List, Node Node);
Status FDiffArccth(List_t* List, Node Node);
Status FDiffLog   (List_t* List, Node Node);
Status FDiffLn    (List_t* List, Node Node);*/

struct function
{
    char name[OP_NAME_MAX];
    int code;
    //Status (*FDiffFunc)(List_t* list, Node_t Node);
};
const function functions[NUMBER_OF_FUNTIONS]
{
    {"sin"   ,       1}, //,    FDiffSin},
    {"cos"   ,       2}, //,    FDiffCos},
    {"arcsin",       3}, //, FDiffArcsin},
    {"arccos",       4}, //, FDiffArccos},
    {"tg"    ,       5}, //,     FDiffTg},
    {"ctg"   ,       6}, //,    FDiffCtg},
    {"arctg" ,       7}, //,  FDiffArctg},
    {"arcctg",       8}, //, FDiffArcctg},
    {"sh"    ,       9}, //,     FDiffSh},
    {"ch"    ,      10}, //,     FDiffCh},
    {"arcsh" ,      11}, //, FDiffArcsh},
    {"arcch" ,      12}, //,  FDiffArcch},
    {"th"    ,      13}, //,     FDiffTh},
    {"cth"   ,      14}, //,    FDiffCth},
    {"arcth" ,      15}, //,  FDiffArcth},
    {"arccth",      16}, //, FDiffArccth},
    {"log"   ,      17}, //,    FDiffLog},
    {"ln"    ,      18}, //,     FDiffLn},
}; 

#endif