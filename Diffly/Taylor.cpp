#include "Taylor.h"
#include "Differentiator.h"
#include <stdio.h>

int Taylor(tree_t* tree, VariableTable_t* VarTable, FILE* outTex, size_t degree)
{

    FILE* TaylorDB = fopen("TaylorDB.dot", "w+b");


    fprintf(outTex, "\\newpage\n");
    fprintf(outTex, "Here comes the Taylor:\\newline\n");
    VarTable->Vars[0].value = 0;
    void* func = tree->CloneTree(tree, tree->root);
    DumpTreeTex(func, outTex, VarTable);
    double* Coefficients = (double*)calloc(degree, sizeof(double));

    for(size_t i = 0; i < degree; i++)
    {
        Coefficients[i] = CalculateTree(func, VarTable);
        fprintf(outTex, "\\(a_{%zu}\\): \\(%lg\\)\\newline\n", i, Coefficients[i]);
        void* copy = func;
        func = Differentiate(tree, func, NULL, VarTable);
        //PrintTreeGraphViz(TaylorDB, *tree, "TaylorDB", &VarTable);
        //fprintf(outTex, "Deriv:\n");
        //DumpNodeTex(func, outTex, VarTable);
        BurnBranch(copy); 
    }
    
    fprintf(outTex, "Full MacLoren series:\\newline\n");
    fprintf(outTex, "\\[");
    DumpNodeTex(tree->root, outTex, VarTable);
    fprintf(outTex, "=");
    for(size_t i = 0; i < degree; i++)
    {
        fprintf(outTex, "%+lg*x^{%zu}", Coefficients[i]/((double)factorialD(i)), i); 
    }
    fprintf(outTex, "+o(x^{%zu})", degree-1);
    fprintf(outTex, "\\]\\newline\n");

    free(Coefficients);
    BurnBranch(func);
    return 0;
}

double CalculateTree(void* root, VariableTable_t* VarTable)
{
    return CalculateNode(root, VarTable);
}

double CalculateNode(void* node, VariableTable_t* VarTable)
{
    size_t type = 0;
    GET_NODE_TYPE(type, node);
    switch(type)
    {
        case CONST_VALUE_TYPE_CODE:
        {
            double value = 0;
            GET_NODE_DATA(value, node);
            MEOW("Value = %lg\n", value);
            return value;
        }
        case VARIABLE_TYPE_CODE:
        {
            int code = 0;
            GET_NODE_DATA(code, node);
            MEOW("x = %lg\n", VarTable->Vars[code].value);
            return VarTable->Vars[code].value;
        }
        case OPERATION_TYPE_CODE:
        {
            char* desc1 = NULL;
            GET_NODE_DESCENDANT(desc1, node, 0);
            char* desc2 = NULL;
            GET_NODE_DESCENDANT(desc2, node, 0);
            int code = 0;
            GET_NODE_DATA(code, node);
            MEOW("OpCOde is %d\n", code);
            return operations[code].CalcFunc(CalculateNode(desc1, VarTable), CalculateNode(desc2, VarTable));
        }
        case FUNCTION_TYPE_CODE:
        {
            char* desc = NULL;
            GET_NODE_DESCENDANT(desc, node, 0);
            int code = 0;
            GET_NODE_DATA(code, node);
            return functions[code].calcFunction(CalculateNode(desc, VarTable));
        }
        default:
        {
            return CALCULATION_FAILURE;
        }
    }
    return 0;
}