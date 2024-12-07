#include <stdio.h>

#include "TexDumper.h"
#include "Differentiator.h"

/*int DumpPlot(void* root, FILE* out)
{
    DumpNodeTex(root, out);
    return 0;
}*/

int DumpTreeTex(void* root, FILE* out, VariableTable_t* VarTable)
{
    if(root == NULL || out == NULL)
    {
        return NULL_POINTER_DIFF;
    }
    fprintf(out, "\\[");
    DumpNodeTex(root, out, VarTable);
    fprintf(out, "\\]\n");

    return 0;
}

int DumpNodeTex(void* node, FILE* out, VariableTable_t* VarTable)
{
    size_t type = 0;
    GET_NODE_TYPE(type, node);

    switch(type)
    {
        case CONST_VALUE_TYPE_CODE:
        {
            DumpConstTex(node, out, VarTable);
            break;
        }
        case VARIABLE_TYPE_CODE:
        {
            DumpVarTex(node, out, VarTable);
            break;
        }
        case OPERATION_TYPE_CODE:
        {
            DumpOperationTex(node, out, VarTable);
            break;
        }
        case FUNCTION_TYPE_CODE:
        {
            DumpFunctionTex(node, out, VarTable);
            break;
        }
        default:
        {
            MEOW("I'm just a chill guy %s\n", __TIME__);
            abort();
        }
    }
    return 0;
}

#define TEX_DUMP_CODE_GEN(fname, f_code)\
int Dump##fname##Tex(void* node, FILE* out, VariableTable_t* VarTable)\
{\
    char* argument = NULL;\
    GET_NODE_DESCENDANT(argument, node, 0);\
    fprintf(out, "\\%s(", functions[f_code].name);\
    DumpNodeTex(argument, out, VarTable);\
    fprintf(out, ")");\
    return 0;\
}
#include "TexDumpFunctionsCodeGen.h"

int DumpFunctionTex(void* node, FILE* out, VariableTable_t* VarTable)
{
    int code = 0;
    GET_NODE_DATA(code, node);
    functions[code].TexDumpFunc(node, out, VarTable);
    return 0;
}

int DumpOperationTex(void* node, FILE* out, VariableTable_t* VarTable)
{
    int code = 0;
    GET_NODE_DATA(code, node);
    operations[code].TexDumpFunc(node, out, VarTable);
    return 0;
}

int DumpSumTex(void* node, FILE* out, VariableTable_t* VarTable)
{
    char* desc1 = NULL;
    GET_NODE_DESCENDANT(desc1, node, 0);
    char* desc2 = NULL;
    GET_NODE_DESCENDANT(desc2, node, 1);

    DumpNodeTex(desc1, out, VarTable);
    fprintf(out, "%c", operations[SUM_DIFF].symbol);
    DumpNodeTex(desc2, out, VarTable);
    
    return 0;
}

int DumpSubTex(void* node, FILE* out, VariableTable_t* VarTable)
{
    char* desc1 = NULL;
    GET_NODE_DESCENDANT(desc1, node, 0);
    char* desc2 = NULL;
    GET_NODE_DESCENDANT(desc2, node, 1);

    DumpNodeTex(desc1, out, VarTable);
    fprintf(out, "%c", operations[SUB_DIFF].symbol);
    DumpNodeTex(desc2, out, VarTable);
    
    return 0;
}

int DumpMulTex(void* node, FILE* out, VariableTable_t* VarTable)
{
    char* desc1 = NULL;
    GET_NODE_DESCENDANT(desc1, node, 0);
    char* desc2 = NULL;
    GET_NODE_DESCENDANT(desc2, node, 1);

    DumpNodeTex(desc1, out, VarTable);
    fprintf(out, "%c", operations[MUL_DIFF].symbol);
    DumpNodeTex(desc2, out, VarTable);
    
    return 0;
}

int DumpDivTex(void* node, FILE* out, VariableTable_t* VarTable)
{
    char* desc1 = NULL;
    GET_NODE_DESCENDANT(desc1, node, 0);
    char* desc2 = NULL;
    GET_NODE_DESCENDANT(desc2, node, 1);

    fprintf(out, "\\frac{");
    DumpNodeTex(desc1, out, VarTable);
    fprintf(out, "}{");
    DumpNodeTex(desc2, out, VarTable);
    fprintf(out, "}");
    
    return 0;
}

int DumpPowTex(void* node, FILE* out, VariableTable_t* VarTable)
{
    char* desc1 = NULL;
    GET_NODE_DESCENDANT(desc1, node, 0);
    char* desc2 = NULL;
    GET_NODE_DESCENDANT(desc2, node, 1);

    DumpNodeTex(desc1, out, VarTable);
    fprintf(out, "%c{", operations[POW_DIFF].symbol);
    DumpNodeTex(desc2, out, VarTable);
    fprintf(out, "}");
    return 0;
}

/*int DumpSinTex(void* node, FILE* out, VariableTable_t* VarTable)
{
    char* argument = NULL;
    GET_NODE_DESCENDANT(argument, node, 0);

    fprintf(out, "\\%s(", functions[SIN_F].name);
    DumpNodeTex(argument, out);
    fprintf(out, ")");
    return 0;
}

int DumpCosTex(void* node, FILE* out, VariableTable_t* VarTable)
{
    char* argument = NULL;
    GET_NODE_DESCENDANT(argument, node, 0);

    fprintf(out, "\\%s(", functions[COS_F].name);
    DumpNodeTex(argument, out);
    fprintf(out, ")");
    return 0;
}

int DumpTgTex(void* node, FILE* out, VariableTable_t* VarTable) 
{
    char* argument = NULL;
    GET_NODE_DESCENDANT(argument, node, 0);

    fprintf(out, "\\%s(", functions[SIN_F].name);
    DumpNodeTex(argument, out);
    fprintf(out, ")");
    return 0;
}*/


int DumpConstTex(void* node, FILE* out, __attribute((unused)) VariableTable_t* VarTable)
{
    double value = 0;
    memcpy(&value, GetNodeData(node, DATA_FIELD_CODE, 0), sizeof(value));
    fprintf(out, "%lg", value);
    return 0;
}

int DumpVarTex(void* node, FILE* out, VariableTable_t* VarTable)
{
    int Code = 0;
    memcpy(&Code, GetNodeData(node, DATA_FIELD_CODE, 0), sizeof(Code));
    fprintf(out, "%s", VarTable->Vars[Code].name);
    return 0;
}