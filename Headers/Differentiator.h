#ifndef DIFFERENTIATOR_H_INCLUDED
#define DIFFERENTIATOR_H_INCLUDED

#include <math.h>
#include <stdlib.h>

#include "DifferentiatorErrors.h"
#include "DifferentiatorMath.h"
//#include "RecursiveDescension.h"
#include "DifferentiatorTree.h"
#include "TexDumper.h"
#include "VariableTable.h"

#include "cmdParser.h"
#include "../Libs/FileBufferizer/FileBufferizer.h"
#include "../Libs/doublecmp/doublecmp.h"

#ifndef NDEBUG
#include "../Libs/DBTools/DBTools.h"
#endif

void ErrorParser(const int code, const int line, const char* file);

void* Differentiate(tree_t* tree, void* node, FILE* TexOut, FILE* PlotOut, VariableTable_t* VarTable);

int FindOperation(const int symbol);

void* GetNodeData(void* node, const int FieldCode, const size_t DescendantNumber);

int PrintTreeTex(tree_t* tree, FILE* out);

enum NodeTypes
{
    CONST_VALUE_TYPE_CODE = 0,
    OPERATION_TYPE_CODE = 1,
    VARIABLE_TYPE_CODE = 2,
    FUNCTION_TYPE_CODE = 3,
};

void* DiffSum(tree_t* tree, void* Node, FILE* TexOut, FILE* PlotOut, VariableTable_t* VarTable);
void* DiffMul(tree_t* tree, void* Node, FILE* TexOut, FILE* PlotOut, VariableTable_t* VarTable);
void* DiffDiv(tree_t* tree, void* Node, FILE* TexOut, FILE* PlotOut, VariableTable_t* VarTable);
void* DiffVar(tree_t* tree, void* node, FILE* TexOut, FILE* PlotOut, VariableTable_t* VarTable);
void* DiffPow(tree_t* tree, void* Node, FILE* TexOut, FILE* PlotOut, VariableTable_t* VarTable);

void* DiffConst(tree_t* tree, __attribute((unused))void* node, __attribute((unused)) FILE* TexOut, __attribute((unused)) FILE* PlotOut, VariableTable_t* VarTable);


void* DifferentiateOperation(tree_t* tree, void* node, FILE* TexOut, FILE* PlotOut, VariableTable_t* VarTable);
void* DifferentiateFunction(tree_t* tree , void* node, FILE* TexOut, FILE* PlotOut, VariableTable_t* VarTable);

void*  SimplifyExpression(tree_t* tree, void* node, VariableTable_t* VarTable);



// TODO better headers
int DumpSumTex(void* node, FILE* out, FILE* plot, VariableTable_t* VarTable);
int DumpSubTex(void* node, FILE* out, FILE* plot, VariableTable_t* VarTable);
int DumpMulTex(void* node, FILE* out, FILE* plot, VariableTable_t* VarTable);
int DumpDivTex(void* node, FILE* out, FILE* plot, VariableTable_t* VarTable);
int DumpPowTex(void* node, FILE* out, FILE* plot, VariableTable_t* VarTable);

#define GET_NODE_DATA(value, node) memcpy(&value, GetNodeData(node, DATA_FIELD_CODE, 0), sizeof(value))
#define GET_NODE_TYPE(type, node) memcpy(&type, GetNodeData(node, TYPE_FIELD_CODE, 0), sizeof(type))
#define GET_NODE_DESCENDANT(desc, node, number) memcpy(&desc, GetNodeData(node, DESCENDANTS_FIELD_CODE, number), sizeof(desc))


#define TEX_DUMP_CODE_GEN(fname, f_code)\
int Dump##fname##Tex(void* node, FILE* out, FILE* plot, VariableTable_t* VarTable);
#include "CodeGen/TexDumpFunctionsCodeGen.h" 

const int NUMBER_OF_OPERATIONS = 5;
struct operation_t
{
    char symbol;
    int code;
    void* (*DiffFunc)(tree_t* tree, void* node, FILE* TexOut, FILE* PlotOut, VariableTable_t* VarTable);
    double (*CalcFunc)(double a, double b);
    int (*TexDumpFunc)(void* node, FILE* out, FILE* plot, VariableTable_t* VarTable);
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

void* FDiffSin   (tree_t* tree, void* node, FILE* TexOut, FILE* PlotOut, VariableTable_t* VarTable);
void* FDiffCos   (tree_t* tree, void* node, FILE* TexOut, FILE* PlotOut, VariableTable_t* VarTable);
void* FDiffArcsin(tree_t* tree, void* node, FILE* TexOut, FILE* PlotOut, VariableTable_t* VarTable);
void* FDiffArccos(tree_t* tree, void* node, FILE* TexOut, FILE* PlotOut, VariableTable_t* VarTable);
void* FDiffTg    (tree_t* tree, void* node, FILE* TexOut, FILE* PlotOut, VariableTable_t* VarTable);
void* FDiffCtg   (tree_t* tree, void* node, FILE* TexOut, FILE* PlotOut, VariableTable_t* VarTable);
void* FDiffArctg (tree_t* tree, void* node, FILE* TexOut, FILE* PlotOut, VariableTable_t* VarTable);
void* FDiffArcctg(tree_t* tree, void* node, FILE* TexOut, FILE* PlotOut, VariableTable_t* VarTable);
void* FDiffSh    (tree_t* tree, void* node, FILE* TexOut, FILE* PlotOut, VariableTable_t* VarTable);
void* FDiffCh    (tree_t* tree, void* node, FILE* TexOut, FILE* PlotOut, VariableTable_t* VarTable);
void* FDiffArcsh (tree_t* tree, void* node, FILE* TexOut, FILE* PlotOut, VariableTable_t* VarTable);
void* FDiffArcch (tree_t* tree, void* node, FILE* TexOut, FILE* PlotOut, VariableTable_t* VarTable);
void* FDiffTh    (tree_t* tree, void* node, FILE* TexOut, FILE* PlotOut, VariableTable_t* VarTable);
void* FDiffCth   (tree_t* tree, void* node, FILE* TexOut, FILE* PlotOut, VariableTable_t* VarTable);
void* FDiffArcth (tree_t* tree, void* node, FILE* TexOut, FILE* PlotOut, VariableTable_t* VarTable);
void* FDiffArccth(tree_t* tree, void* node, FILE* TexOut, FILE* PlotOut, VariableTable_t* VarTable);
void* FDiffLn    (tree_t* tree, void* node, FILE* TexOut, FILE* PlotOut, VariableTable_t* VarTable);
void* FDiffLog   (tree_t* tree, void* node, FILE* TexOut, FILE* PlotOut, VariableTable_t* VarTable);

const int NUMBER_OF_FUNCTIONS = 18;
struct function
{
    char name[FUNCTION_NAME_MAX];
    int code;
    double (*calcFunction)(double);
    void* (*FDiffFunc)(tree_t* tree, void* node, FILE* TexOut, FILE* PlotOut, VariableTable_t* VarTable);
    int (*TexDumpFunc)(void* node, FILE* out, FILE* plot, VariableTable_t* VarTable);
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
    {"sin"    ,       0, sin  ,    FDiffSin, DumpSinTex   },
    {"cos"    ,       1, cos  ,    FDiffCos, DumpCosTex   },
    {"arcsin" ,       2, asin , FDiffArcsin, DumpArcsinTex},
    {"arccos" ,       3, acos , FDiffArccos, DumpArccosTex},
    {"tan"    ,       4, tan  ,     FDiffTg, DumpTgTex    },
    {"cot"    ,       5, cot  ,    FDiffCtg, DumpCtgTex   },
    {"arcot"  ,       6, atan ,  FDiffArctg, DumpArctgTex },
    {"arctan" ,       7, acot , FDiffArcctg, DumpArcctgTex},
    {"sinh"   ,       8, sinh ,     FDiffSh, DumpShTex    },
    {"cosh"   ,       9, cosh ,     FDiffCh, DumpChTex    },
    {"arcsih" ,      10, asinh,  FDiffArcsh, DumpArcshTex },
    {"arccosh",      11, acosh,  FDiffArcch, DumpArcchTex },
    {"tanh"   ,      12, tanh ,     FDiffTh, DumpThTex    },
    {"coth"   ,      13, NULL ,    FDiffCth, DumpCthTex   },
    {"arctanh",      14, atanh,  FDiffArcth, DumpArcthTex },
    {"arccoth",      15, NULL , FDiffArccth, DumpArccthTex},
    {"ln"     ,      16, log  ,     FDiffLn, DumpLnTex    }, // TODO virtual function table
    {"log"    ,      17,  NULL,        NULL, NULL         }, // TODO root
};

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

#define SIMPLE(node) SimplifyExpression(tree, node, VarTable)

#define S_SUM_NODE_DIFF(node1, node2) SimplifyExpression(tree, tree->InitNode(tree, NULL, OPERATION_TYPE_CODE, 4, &operations[SUM_DIFF].code, 2, node1, node2), VarTable)
#define S_SUB_NODE_DIFF(node1, node2) SimplifyExpression(tree, tree->InitNode(tree, NULL, OPERATION_TYPE_CODE, 4, &operations[SUB_DIFF].code, 2, node1, node2), VarTable)
#define S_MUL_NODE_DIFF(node1, node2) SimplifyExpression(tree, tree->InitNode(tree, NULL, OPERATION_TYPE_CODE, 4, &operations[MUL_DIFF].code, 2, node1, node2), VarTable)
#define S_DIV_NODE_DIFF(node1, node2) SimplifyExpression(tree, tree->InitNode(tree, NULL, OPERATION_TYPE_CODE, 4, &operations[DIV_DIFF].code, 2, node1, node2), VarTable)
#define S_POW_NODE_DIFF(node1, node2) SimplifyExpression(tree, tree->InitNode(tree, NULL, OPERATION_TYPE_CODE, 4, &operations[POW_DIFF].code, 2, node1, node2), VarTable)

#define S_SIN_NODE_DIFF(node) SimplifyExpression(tree, tree->InitNode(tree, NULL, FUNCTION_TYPE_CODE, 4,  &functions[SIN_F].code, 1, node), VarTable)
#define S_COS_NODE_DIFF(node) SimplifyExpression(tree, tree->InitNode(tree, NULL, FUNCTION_TYPE_CODE, 4,  &functions[COS_F].code, 1, node), VarTable)
#define S_SINH_NODE_DIFF(node) SimplifyExpression(tree, tree->InitNode(tree, NULL, FUNCTION_TYPE_CODE, 4, &functions[SINH_F].code, 1, node), VarTable)
#define S_COSH_NODE_DIFF(node) SimplifyExpression(tree, tree->InitNode(tree, NULL, FUNCTION_TYPE_CODE, 4, &functions[COSH_F].code, 1, node), VarTable)
#define S_LN_NODE_DIFF(node) SimplifyExpression(tree, tree->InitNode(tree, NULL, FUNCTION_TYPE_CODE, 4,   &functions[LN_F].code, 1, node), VarTable)




#endif