#include "Taylor.h"
#include "Differentiator.h"
#include <stdio.h>

int Taylor(tree_t* tree, VariableTable_t* VarTable, FILE* outTex)
{
    fprintf(outTex, "Here comes the Taylor:\n");
    fprintf(outTex, "Zero coefficient: %lg", CalculateTree(tree->root, VarTable));
    void* neww = Differentiate(tree, tree->root, outTex, VarTable);
    fprintf(outTex, "Zero coefficient: %lg", CalculateTree(neww, VarTable));
    BurnBranch(neww);
   /* double coefficients[7] = {};
    void* diffed = NULL;
    for(int i = 1; i < 7; i++)
    {
        void* old = tree->root;
        tree->root = Differentiate(tree, tree->root, outTex, VarTable);
        free(old);
        coefficients[i] = CalculateTree(diffed, VarTable);
        fprintf(outTex, "%d coefficient: %lg", i, coefficients[i]);
        
    }
    free(tree->root);
    fprintf(outTex, "Final series:\n");
    fprintf(outTex, "%lg + \\frac{%lg*x}{1!} + \\frac{%lg*x^2}{2!} + \\frac{%lg*x^3}{3!} + \\frac{%lg*x^4}{4!} + \\frac{%lg*x^5}{5!} + \\frac{%lg*x^6}{6!}", coefficients[0], coefficients[1], coefficients[2], coefficients[3], coefficients[4], coefficients[5], coefficients[6]);*/
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
            return value;
        }
        case VARIABLE_TYPE_CODE:
        {
            int code = 0;
            GET_NODE_DATA(code, node);
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
    }
    return 0;
}