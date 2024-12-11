#include <stdio.h>

#include "../Headers/Taylor.h"
#include "../Headers/Differentiator.h"
#include "../Libs/doublecmp/doublecmp.h"

int Taylor(tree_t* tree, VariableTable_t* VarTable, FILE* outTex, FILE* outPlot, size_t degree, double point)
{
    VarTable->Vars[0].value = point;

    fprintf(outTex, "\\newpage\n");
    fprintf(outTex, "Here comes the Taylor:\\newline\n");

    void* func = tree->CloneTree(tree, tree->root);
    DumpTreeTex(func, outTex, NULL, VarTable);

    double* Coefficients = (double*)calloc(degree, sizeof(double));

    for(size_t i = 0; i < degree; i++)
    {
        Coefficients[i] = CalculateNode(func, VarTable);
        fprintf(outTex, "\\(a_{%zu}\\): \\(%lg\\)\\newline\n", i, Coefficients[i]);
        void* copy = func;
        func = Differentiate(tree, func, NULL, NULL, VarTable);
        //fprintf(outTex, "Deriv:\n");
        //DumpNodeTex(func, outTex, VarTable);
        BurnBranch(copy); 
    }
    
    fprintf(outTex, "Full MacLoren series:\\newline\n");
    fprintf(outTex, "\\[");
    DumpNodeTex(tree->root, outTex, NULL, VarTable);
    fprintf(outTex, "=");

    fprintf(outPlot, "\nplt.plot(");

    if(doublecmp(Coefficients[0], 0, EPS) != 0)
    {
        fprintf(outTex, "%lg*x^{%d}", Coefficients[0]/((double)factorialD(0)), 0); 
        fprintf(outPlot, "%lg*x**%d", Coefficients[0]/((double)factorialD(0)), 0);
    }

    for(size_t i = 1; i < degree; i++)
    {
        if(doublecmp(Coefficients[i], 0, EPS) != 0)
        {
            fprintf(outTex, "%+lg*x^{%zu}", Coefficients[i]/((double)factorialD(i)), i);  //TODO remove plus at the begining
            if(doublecmp(Coefficients[i], 0, EPS) < 0)
            {
                fprintf(outPlot, "%lg*x**%zu", Coefficients[i]/((double)factorialD(i)), i);
            }
            else
            {
                fprintf(outPlot, "+%lg*x**%zu", Coefficients[i]/((double)factorialD(i)), i);
            }
        }
    }

    fprintf(outPlot, ")\n");
    fprintf(outTex, "+o(x^{%zu})", degree-1);
    fprintf(outTex, "\\]\\newline\n");

    free(Coefficients);
    BurnBranch(func);
    return 0;
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
            GET_NODE_DESCENDANT(desc2, node, 1);
            int code = 0;
            GET_NODE_DATA(code, node);
            MEOW("OpCOde is %d, operation is %c\n", code, operations[code].symbol);
            double retVal = operations[code].CalcFunc(CalculateNode(desc1, VarTable), CalculateNode(desc2, VarTable));
            MEOW(MAGENTA_COLOR_ESC_SEQ"%lg\n" DEFAULT_COLOR_ESC_SEQ, retVal);
            return retVal;
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