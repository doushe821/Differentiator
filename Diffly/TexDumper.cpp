#include <stdio.h>

#include "TexDumper.h"

int DumpNodeTex(void* node, FILE* out)
{
    size_t type = 0;
    GET_NODE_TYPE(type, node);

    switch(type)
    {
        case CONST_VALUE_TYPE_CODE:
        {
            DumpConstTex(node, out);
        }
        case VARIABLE_TYPE_CODE:
        {
            DumpVarTex(node, out);
        }
        case OPERATION_TYPE_CODE:
        {
            DumpOperationTex(node, out);
        }
        case FUNCTION_TYPE_CODE:
        {
            DumpFunctionTex(node, out);
        }
    }
}

int DumpFunctionTex(void* node, FILE* out)
{
    int code = 0;
    GET_NODE_DATA(code, node);
    functions[code].TexDumpFunc(node, out);
}

int DumpOperationTex(void* node, FILE* out)
{
    int code = 0;
    GET_NODE_DATA(code, node);
    operations[code].TexDumpFunc(node, out);
    return 0;
}

int DumpSumTex(void* node, FILE* out)
{
    char* desc1 = NULL;
    GET_NODE_DESCENDANT(desc1, node, 0);
    char* desc2 = NULL;
    GET_NODE_DESCENDANT(desc2, node, 1);

    DumpNodeTex(desc1, out);
    fprintf(out, "%c", operations[SUM_DIFF].symbol);
    DumpNodeTex(desc2, out);
    
    return 0;
}

int DumpSubTex(void* node, FILE* out)
{
    char* desc1 = NULL;
    GET_NODE_DESCENDANT(desc1, node, 0);
    char* desc2 = NULL;
    GET_NODE_DESCENDANT(desc2, node, 1);

    DumpNodeTex(desc1, out);
    fprintf(out, "%c", operations[SUB_DIFF].symbol);
    DumpNodeTex(desc2, out);
    
    return 0;
}

int DumpMulTex(void* node, FILE* out)
{
    char* desc1 = NULL;
    GET_NODE_DESCENDANT(desc1, node, 0);
    char* desc2 = NULL;
    GET_NODE_DESCENDANT(desc2, node, 1);

    DumpNodeTex(desc1, out);
    fprintf(out, "%c", operations[MUL_DIFF].symbol);
    DumpNodeTex(desc2, out);
    
    return 0;
}

int DumpDivTex(void* node, FILE* out)
{
    char* desc1 = NULL;
    GET_NODE_DESCENDANT(desc1, node, 0);
    char* desc2 = NULL;
    GET_NODE_DESCENDANT(desc2, node, 1);

    fprintf(out, "\frac{");
    DumpNodeTex(desc1, out);
    fprintf(out, "}{");
    DumpNodeTex(desc2, out);
    fprintf(out, "}");
    
    return 0;
}

int DumpPowTex(void* node, FILE* out)
{
    char* desc1 = NULL;
    GET_NODE_DESCENDANT(desc1, node, 0);
    char* desc2 = NULL;
    GET_NODE_DESCENDANT(desc2, node, 1);

    DumpNodeTex(desc1, out);
    fprintf(out, "%c{", operations[POW_DIFF].symbol);
    DumpNodeTex(desc2, out);
    fprintf(out, "}");
    return 0;
}

/*int DumpSinTex(void* node, FILE* out)
{
    char* argument = NULL;
    GET_NODE_DESCENDANT(argument, node, 0);

    fprintf(out, "\\%s(", functions[SIN_F].name);
    DumpNodeTex(argument, out);
    fprintf(out, ")");
    return 0;
}

int DumpCosTex(void* node, FILE* out)
{
    char* argument = NULL;
    GET_NODE_DESCENDANT(argument, node, 0);

    fprintf(out, "\\%s(", functions[COS_F].name);
    DumpNodeTex(argument, out);
    fprintf(out, ")");
    return 0;
}

int DumpTgTex(void* node, FILE* out) 
{
    char* argument = NULL;
    GET_NODE_DESCENDANT(argument, node, 0);

    fprintf(out, "\\%s(", functions[SIN_F].name);
    DumpNodeTex(argument, out);
    fprintf(out, ")");
    return 0;
}*/


int DumpConstTex(void* node, FILE* out)
{
    double value = 0;
    memcpy(&value, GetNodeData(node, DATA_FIELD_CODE, 0), sizeof(value));
    fprintf(out, "%d", value);
    return 0;
}

int DumpVarTex(void* node, FILE* out)
{
    int aciiCode = 0;
    memcpy(&aciiCode, GetNodeData(node, DATA_FIELD_CODE, 0), sizeof(aciiCode));
    fprintf(out, "%c", aciiCode);
    return 0;
}