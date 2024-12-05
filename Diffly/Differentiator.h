#ifndef DIFFERENTIATOR_H_INCLUDED
#define DIFFERENTIATOR_H_INCLUDED

#include <math.h>
#include <stdlib.h>

#include "DifferentiatorErrors.h"
#include "DifferentiatorMath.h"
#include "RecursiveDescension.h"
#include "DifferentiatorTree.h"

#include "cmdParser.h"
#include "FileBufferizer/FileBufferizer.h"
#include "doublecmp/doublecmp.h"

#ifndef NDEBUG
#include "DBTools/DBTools.h"
#endif

void ErrorParser(int code, int line);

void* Differentiate(tree_t* tree, void* node);

int FindOperation(const int symbol);

enum NodeTypes
{
    CONST_VALUE_TYPE_CODE = 0,
    OPERATION_TYPE_CODE = 1,
    VARIABLE_TYPE_CODE = 2,
    FUNCTION_TYPE_CODE = 3,
};

void* DiffSum(tree_t* tree, void* Node);
void* DiffMul(tree_t* tree, void* Node);
void* DiffDiv(tree_t* tree, void* Node);
void* DiffVar(tree_t* tree, void* node);
void* DiffPow(tree_t* tree, void* Node);

void* DiffConst(tree_t* tree, void* node);

void* DifferentiateOperation(tree_t* tree, void* node);

void* DifferentiateFunction(tree_t* tree, void* node);

void* SimplifyExpression(tree_t* tree, void* node);



// TODO better headers
int DumpSumTex(void* node, FILE* out);
int DumpSubTex(void* node, FILE* out);
int DumpMulTex(void* node, FILE* out);
int DumpDivTex(void* node, FILE* out);
int DumpPowTex(void* node, FILE* out);
int DumpVarTex(void* node, FILE* out);
int DumpConstTex(void* node, FILE* out);
int DumpOperationTex(void* node, FILE* out);
int DumpNodeTex(void* node, FILE* out);


#define GET_NODE_DATA(value, node) memcpy(&value, GetNodeData(node, DATA_FIELD_CODE, 0), sizeof(value))
#define GET_NODE_TYPE(type, node) memcpy(&type, GetNodeData(node, TYPE_FIELD_CODE, 0), sizeof(type))
#define GET_NODE_DESCENDANT(desc, node, number) memcpy(&desc, GetNodeData(node, DESCENDANTS_FIELD_CODE, number), sizeof(desc))


#define TEX_DUMP_CODE_GEN(fname, f_code)\
int Dump##fname##Tex(void* node, FILE* out);
#include "TexDumpFunctionsCodeGen.h" 




const int NUMBER_OF_OPERATIONS = 5;
struct operation_t
{
    char symbol;
    int code;
    void* (*DiffFunc)(tree_t* tree, void* node);
    double (*CalcFunc)(double a, double b);
    int (*TexDumpFunc)(void* node, FILE* out);
};
enum DIFFERENTIATOR_OPERATIONS
{
    SUM_DIFF = 0,
    SUB_DIFF = 1,
    MUL_DIFF = 2,
    DIV_DIFF = 3,
    POW_DIFF = 4,

};
const operation_t operations[NUMBER_OF_OPERATIONS] = 
{
    {'+'     , 0, DiffSum, CalcSum, DumpSumTex},
    {'-'     , 1, DiffSum, CalcSub, DumpSubTex},
    {'*'     , 2, DiffMul, CalcMul, DumpMulTex},
    {'/'     , 3, DiffDiv, CalcDiv, DumpDivTex},
    {'^'     , 4, DiffPow, CalcPow, DumpPowTex},
};

const size_t FUNCTION_NAME_MAX = 16;

void* FDiffSin   (tree_t* tree, void* node);
void* FDiffCos   (tree_t* tree, void* node);
void* FDiffArcsin(tree_t* tree, void* node);
void* FDiffArccos(tree_t* tree, void* node);
void* FDiffTg    (tree_t* tree, void* node);
void* FDiffCtg   (tree_t* tree, void* node);
void* FDiffArctg (tree_t* tree, void* node);
void* FDiffArcctg(tree_t* tree, void* node);
void* FDiffSh    (tree_t* tree, void* node);
void* FDiffCh    (tree_t* tree, void* node);
void* FDiffArcsh (tree_t* tree, void* node);
void* FDiffArcch (tree_t* tree, void* node);
void* FDiffTh    (tree_t* tree, void* node);
void* FDiffCth   (tree_t* tree, void* node);
void* FDiffArcth (tree_t* tree, void* node);
void* FDiffArccth(tree_t* tree, void* node);
void* FDiffLn    (tree_t* tree, void* node);
void* FDiffLog   (tree_t* tree, void* node);


void* DiffPowF(tree_t* tree, void* node);

const int NUMBER_OF_FUNCTIONS = 18;
struct function
{
    char name[FUNCTION_NAME_MAX];
    int code;
    double (*calcFunction)(double);
    void* (*FDiffFunc)(tree_t* tree, void* node);
    int (*TexDumpFunc)(void* node, FILE* out);
};

enum DIFFERENTIATOR_FUNCTIONS
{
    SIN_F      = 0,
    COS_F      = 1,
    ARCSIN_F   = 2,
    ARCCOS_F   = 3,
    TAN_F      = 4,
    COT_F      = 5,
    ATAN_F     = 6,
    ACOT_F     = 7,
    SINH_F     = 8,
    COSH_F     = 9,
    ASINH_F    = 10,
    ACOSH_F    = 11,
    TANH_F     = 12,
    COTH_F     = 13,
    ATANH_F    = 14,
    ACOTH_F    = 15,
    LN_F       = 16,
    LOG_F      = 17,
};
const function functions[NUMBER_OF_FUNCTIONS]
{
    {"sin"   ,       0, sin  ,    FDiffSin, DumpSinTex   },
    {"cos"   ,       1, cos  ,    FDiffCos, DumpCosTex   },
    {"arcsin",       2, asin , FDiffArcsin, DumpArcsinTex},
    {"arccos",       3, acos , FDiffArccos, DumpArccosTex},
    {"tg"    ,       4, tan  ,     FDiffTg, DumpTgTex    },
    {"ctg"   ,       5, cot  ,    FDiffCtg, DumpCtgTex   },
    {"arctg" ,       6, atan ,  FDiffArctg, DumpArctgTex },
    {"arcctg",       7, acot , FDiffArcctg, DumpArcctgTex},
    {"sh"    ,       8, sinh ,     FDiffSh, DumpShTex    },
    {"ch"    ,       9, cosh ,     FDiffCh, DumpChTex    },
    {"arcsh" ,      10, asinh,  FDiffArcsh, DumpArcshTex },
    {"arcch" ,      11, acosh,  FDiffArcch, DumpArcchTex },
    {"th"    ,      12, tanh ,     FDiffTh, DumpThTex    },
    {"cth"   ,      13, NULL ,    FDiffCth, DumpCthTex   },
    {"arcth" ,      14, atanh,  FDiffArcth, DumpArcthTex },
    {"arccth",      15, NULL , FDiffArccth, DumpArccthTex},
    {"ln"    ,      16, log  ,     FDiffLn, DumpLnTex    }, // TODO virtual function table
    {"log"   ,      17,  NULL,        NULL, NULL         }, // TODO root
};


#define TEX_DUMP_CODE_GEN(fname, f_code)\
int Dump##fname##Tex(void* node, FILE* out)\
{\
    char* argument = NULL;\
    GET_NODE_DESCENDANT(argument, node, 0);\
    fprintf(out, "\\%s(", functions[f_code].name);\
    DumpNodeTex(argument, out);\
    fprintf(out, ")");\
    return 0;\
}
#include "TexDumpFunctionsCodeGen.h"


#define INIT_C_NODE(value) tree->InitNode(tree, NULL, CONST_VALUE_TYPE_CODE, 8, value, 0)

#define SUM_NODE_DIFF(node1, node2) tree->InitNode(tree, NULL, OPERATION_TYPE_CODE, 4, &operations[SUM_DIFF].code, 2, node1, node2)
#define SUB_NODE_DIFF(node1, node2) tree->InitNode(tree, NULL, OPERATION_TYPE_CODE, 4, &operations[SUB_DIFF].code, 2, node1, node2)
#define MUL_NODE_DIFF(node1, node2) tree->InitNode(tree, NULL, OPERATION_TYPE_CODE, 4, &operations[MUL_DIFF].code, 2, node1, node2)
#define DIV_NODE_DIFF(node1, node2) tree->InitNode(tree, NULL, OPERATION_TYPE_CODE, 4, &operations[DIV_DIFF].code, 2, node1, node2)
#define POW_NODE_DIFF(node1, node2) tree->InitNode(tree, NULL, OPERATION_TYPE_CODE, 4, &operations[POW_DIFF].code, 2, node1, node2)

#define SIN_NODE_DIFF(node) tree->InitNode(tree, NULL, FUNCTION_TYPE_CODE, 4,  &functions[SIN_F].code, 1, node)
#define COS_NODE_DIFF(node) tree->InitNode(tree, NULL, FUNCTION_TYPE_CODE, 4,  &functions[COS_F].code, 1, node)
#define SINH_NODE_DIFF(node) tree->InitNode(tree, NULL, FUNCTION_TYPE_CODE, 4, &functions[SINH_F].code, 1, node)
#define COSH_NODE_DIFF(node) tree->InitNode(tree, NULL, FUNCTION_TYPE_CODE, 4, &functions[COSH_F].code, 1, node)
#define LN_NODE_DIFF(node) tree->InitNode(tree, NULL, FUNCTION_TYPE_CODE, 4,   &functions[LN_F].code, 1, node)

#define SIMPLE(node) SimplifyExpression(tree, node)

#define S_SUM_NODE_DIFF(node1, node2) SimplifyExpression(tree, tree->InitNode(tree, NULL, OPERATION_TYPE_CODE, 4, &operations[SUM_DIFF].code, 2, node1, node2))
#define S_SUB_NODE_DIFF(node1, node2) SimplifyExpression(tree, tree->InitNode(tree, NULL, OPERATION_TYPE_CODE, 4, &operations[SUB_DIFF].code, 2, node1, node2))
#define S_MUL_NODE_DIFF(node1, node2) SimplifyExpression(tree, tree->InitNode(tree, NULL, OPERATION_TYPE_CODE, 4, &operations[MUL_DIFF].code, 2, node1, node2))
#define S_DIV_NODE_DIFF(node1, node2) SimplifyExpression(tree, tree->InitNode(tree, NULL, OPERATION_TYPE_CODE, 4, &operations[DIV_DIFF].code, 2, node1, node2))
#define S_POW_NODE_DIFF(node1, node2) SimplifyExpression(tree, tree->InitNode(tree, NULL, OPERATION_TYPE_CODE, 4, &operations[POW_DIFF].code, 2, node1, node2))

#define S_SIN_NODE_DIFF(node) SimplifyExpression(tree, tree->InitNode(tree, NULL, FUNCTION_TYPE_CODE, 4,  &functions[SIN_F].code, 1, node))
#define S_COS_NODE_DIFF(node) SimplifyExpression(tree, tree->InitNode(tree, NULL, FUNCTION_TYPE_CODE, 4,  &functions[COS_F].code, 1, node))
#define S_SINH_NODE_DIFF(node) SimplifyExpression(tree, tree->InitNode(tree, NULL, FUNCTION_TYPE_CODE, 4, &functions[SINH_F].code, 1, node))
#define S_COSH_NODE_DIFF(node) SimplifyExpression(tree, tree->InitNode(tree, NULL, FUNCTION_TYPE_CODE, 4, &functions[COSH_F].code, 1, node))
#define S_LN_NODE_DIFF(node) SimplifyExpression(tree, tree->InitNode(tree, NULL, FUNCTION_TYPE_CODE, 4,   &functions[LN_F].code, 1, node))




#endif