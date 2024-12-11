#include <stdio.h>


#include "../Headers/TexDumper.h"
#include "../Headers/Differentiator.h"

/*int DumpPlot(void* root, FILE* out)
{
    DumpNodeTex(root, out);
    return 0;
}*/


int DumpTreeTex(void* root, FILE* out, FILE* plot, VariableTable_t* VarTable)
{
    if(root == NULL)
    {
        return NULL_POINTER_DIFF;
    }
    
    if(plot != NULL)
    {
        fprintf(plot, "plt.plot(");
        fprintf(out, "\\[");
        DumpNodeTex(root, out, plot, VarTable);
        fprintf(out, "\\]\n");
        fprintf(plot, ")\n");
    }
    else if(out != NULL)
    {
        fprintf(out, "\\[");
        DumpNodeTex(root, out, NULL, VarTable);
        fprintf(out, "\\]\n");
        //fprintf(plot, ")\nplt.legend()\nmatplotlib.pyplot.savefig('Plots.png')\n");
    }
    else
    {
        return 0;
    }

    return 0;
}

int DumpNodeTex(void* node, FILE* out, FILE* plot, VariableTable_t* VarTable)
{
    size_t type = 0;
    GET_NODE_TYPE(type, node);

    switch(type)
    {
        case CONST_VALUE_TYPE_CODE:
        {
            DumpConstTex(node, out, plot, VarTable);
            break;
        }
        case VARIABLE_TYPE_CODE:
        {
            DumpVarTex(node, out, plot, VarTable);
            break;
        }
        case OPERATION_TYPE_CODE:
        {
            DumpOperationTex(node, out, plot, VarTable);
            break;
        }
        case FUNCTION_TYPE_CODE:
        {
            DumpFunctionTex(node, out, plot, VarTable);
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
int Dump##fname##Tex(void* node, FILE* out, FILE* plot, VariableTable_t* VarTable)\
{\
    char* argument = NULL;\
    GET_NODE_DESCENDANT(argument, node, 0);\
    if(plot != NULL)\
    {\
        if(f_code == LN_F)\
        {\
            fprintf(plot, "np.log");\
        }\
        else\
        {\
            fprintf(plot, "np.%s", functions[f_code].name);\
        }\
            fprintf(out, "\\%s(", functions[f_code].name);\
            fprintf(plot, "(");\
            DumpNodeTex(argument, out, plot, VarTable);\
            fprintf(plot, ")");\
            fprintf(out, ")");\
    }\
    else\
    {\
        fprintf(out, "\\%s(", functions[f_code].name);\
        DumpNodeTex(argument, out, plot, VarTable);\
        fprintf(out, ")");\
    }\
    return 0;\
}
#include "../Headers/CodeGen/TexDumpFunctionsCodeGen.h"

int DumpFunctionTex(void* node, FILE* out, FILE* plot, VariableTable_t* VarTable)
{
    int code = 0;
    GET_NODE_DATA(code, node);
    functions[code].TexDumpFunc(node, out, plot, VarTable);
    return 0;
}

int DumpOperationTex(void* node, FILE* out, FILE* plot, VariableTable_t* VarTable)
{
    int code = 0;
    GET_NODE_DATA(code, node);
    operations[code].TexDumpFunc(node, out, plot, VarTable);
    return 0;
}

int DumpSumTex(void* node, FILE* out, FILE* plot, VariableTable_t* VarTable)
{
    char* desc1 = NULL;
    GET_NODE_DESCENDANT(desc1, node, 0);
    char* desc2 = NULL;
    GET_NODE_DESCENDANT(desc2, node, 1);

    if(plot != NULL)
    {
        fprintf(plot, "(");
        fprintf(out, "(");
        DumpNodeTex(desc1, out, plot, VarTable);
        fprintf(plot, "%c", operations[SUM_DIFF].symbol);
        fprintf(out, "%c", operations[SUM_DIFF].symbol);
        DumpNodeTex(desc2, out, plot, VarTable);
        fprintf(out, ")");
        fprintf(plot, ")");   
    }
    else
    {
        //fprintf(plot, "(");
        fprintf(out, "(");
        DumpNodeTex(desc1, out, plot, VarTable);
        //fprintf(plot, "%c", operations[SUM_DIFF].symbol);
        fprintf(out, "%c", operations[SUM_DIFF].symbol);
        DumpNodeTex(desc2, out, plot, VarTable);
        fprintf(out, ")");
        //fprintf(plot, ")");
    }
    
    return 0;
}

void FinishTexDump(FILE* outTex, FILE* Plots)
{
    fprintf(Plots, "\nplt.legend()\nmatplotlib.pyplot.savefig('Tex/Plots.png')\n");
    fclose(Plots);
    system("python Tex/Plots.py");
    fprintf(outTex, "\\includegraphics{Plots.png}\n");
    fprintf(outTex, "%s", PLOT_COMMENT);

    fprintf(outTex, "\\end{document}");

    fclose(outTex);

    system("cd Tex");
    system("pdflatex -output-directory Tex Tex/Deriv >/dev/null");
}


FILE* MakePlots()
{
    FILE* Plots = fopen("Tex/Plots.py", "w+b");
    if(Plots == NULL)
    {
        return NULL;
    }
    fprintf(Plots, "%s\n", MATPLOTLIB_HEAD);
    return Plots;
}

int DumpSubTex(void* node, FILE* out, FILE* plot, VariableTable_t* VarTable)
{
    char* desc1 = NULL;
    GET_NODE_DESCENDANT(desc1, node, 0);
    char* desc2 = NULL;
    GET_NODE_DESCENDANT(desc2, node, 1);

    if(plot != NULL)
    {
        fprintf(plot, "(");
        fprintf(out, "(");
        DumpNodeTex(desc1, out, plot, VarTable);
        fprintf(plot, "%c", operations[SUB_DIFF].symbol);
        fprintf(out, "%c", operations[SUB_DIFF].symbol);
        DumpNodeTex(desc2, out, plot, VarTable);
        fprintf(out, ")");
        fprintf(plot, ")");  
    }
    else
    {
        //fprintf(plot, "(");
        fprintf(out, "(");
        DumpNodeTex(desc1, out, plot, VarTable);
        //fprintf(plot, "%c", operations[SUB_DIFF].symbol);
        fprintf(out, "%c", operations[SUB_DIFF].symbol);
        DumpNodeTex(desc2, out, plot, VarTable);
        fprintf(out, ")");
        //fprintf(plot, ")");
    }
    
    return 0;
}

int DumpMulTex(void* node, FILE* out, FILE* plot, VariableTable_t* VarTable)
{
    char* desc1 = NULL;
    GET_NODE_DESCENDANT(desc1, node, 0);
    char* desc2 = NULL;
    GET_NODE_DESCENDANT(desc2, node, 1);

    if(plot != NULL)
    {
        fprintf(plot, "(");
        //fprintf(out, "(");
        DumpNodeTex(desc1, out, plot, VarTable);
        fprintf(out, "%c", operations[MUL_DIFF].symbol);
        fprintf(plot, "%c", operations[MUL_DIFF].symbol);
        DumpNodeTex(desc2, out, plot, VarTable);
        //fprintf(out, ")");
        fprintf(plot, ")");
    }
    else
    {
        //fprintf(plot, "(");
        //fprintf(out, "(");
        DumpNodeTex(desc1, out, plot, VarTable);
        fprintf(out, "%c", operations[MUL_DIFF].symbol);
        //fprintf(plot, "%c", operations[MUL_DIFF].symbol);
        DumpNodeTex(desc2, out, plot, VarTable);
       // fprintf(out, ")");
        //fprintf(plot, ")");
    }
    
    return 0;
}

int DumpDivTex(void* node, FILE* out, FILE* plot, VariableTable_t* VarTable)
{
    char* desc1 = NULL;
    GET_NODE_DESCENDANT(desc1, node, 0);
    char* desc2 = NULL;
    GET_NODE_DESCENDANT(desc2, node, 1);
    if(plot != NULL)
    {
        fprintf(plot, "(");
        fprintf(out, "\\frac{");
        DumpNodeTex(desc1, out, plot, VarTable);
        fprintf(plot, "%c", operations[DIV_DIFF].symbol);
        fprintf(out, "}{");
        DumpNodeTex(desc2, out, plot,  VarTable);
        fprintf(out, "}");
        fprintf(plot, ")");
    }
    else
    {
        //fprintf(plot, "(");
        fprintf(out, "\\frac{");
        DumpNodeTex(desc1, out, plot, VarTable);
        //fprintf(plot, "%c", operations[DIV_DIFF].symbol);
        fprintf(out, "}{");
        DumpNodeTex(desc2, out, plot,  VarTable);
        fprintf(out, "}");
        //fprintf(plot, ")");
    }
    
    return 0;
}

int DumpPowTex(void* node, FILE* out, FILE* plot, VariableTable_t* VarTable)
{
    char* desc1 = NULL;
    GET_NODE_DESCENDANT(desc1, node, 0);
    char* desc2 = NULL;
    GET_NODE_DESCENDANT(desc2, node, 1);

    if(plot != NULL)
    {
        fprintf(plot, "(");
        fprintf(out, "(");
        DumpNodeTex(desc1, out, plot,  VarTable);
        fprintf(plot, "**");
        fprintf(out, "%c{", operations[POW_DIFF].symbol);
        DumpNodeTex(desc2, out, plot, VarTable);
        fprintf(out, "}");
        fprintf(plot, ")");
        fprintf(out, ")");
    }
    else
    {
        //fprintf(plot, "(");
        fprintf(out, "(");
        DumpNodeTex(desc1, out, plot,  VarTable);
        //fprintf(plot, "**");
        fprintf(out, "%c{", operations[POW_DIFF].symbol);
        DumpNodeTex(desc2, out, plot, VarTable);
        fprintf(out, "}");
        fprintf(out, ")");
        //fprintf(plot, ")");
    }
    return 0;
}

/*int DumpSinTex(void* node, FILE* out, FILE* plot, VariableTable_t* VarTable)
{
    char* argument = NULL;
    GET_NODE_DESCENDANT(argument, node, 0);

    fprintf(out, "\\%s(", functions[SIN_F].name);
    DumpNodeTex(argument, out);
    fprintf(out, ")");
    return 0;
}

int DumpCosTex(void* node, FILE* out, FILE* plot, VariableTable_t* VarTable)
{
    char* argument = NULL;
    GET_NODE_DESCENDANT(argument, node, 0);

    fprintf(out, "\\%s(", functions[COS_F].name);
    DumpNodeTex(argument, out);
    fprintf(out, ")");
    return 0;
}

int DumpTgTex(void* node, FILE* out, FILE* plot, VariableTable_t* VarTable) 
{
    char* argument = NULL;
    GET_NODE_DESCENDANT(argument, node, 0);

    fprintf(out, "\\%s(", functions[SIN_F].name);
    DumpNodeTex(argument, out);
    fprintf(out, ")");
    return 0;
}*/


int DumpConstTex(void* node, FILE* out, FILE* plot, __attribute((unused)) VariableTable_t* VarTable)
{
    double value = 0;
    memcpy(&value, GetNodeData(node, DATA_FIELD_CODE, 0), sizeof(value));
    if(plot != NULL)
        fprintf(plot, "%lg", value);
    fprintf(out, "%lg", value);
    return 0;
}

int DumpVarTex(void* node, FILE* out, FILE* plot, VariableTable_t* VarTable)
{
    int Code = 0;
    memcpy(&Code, GetNodeData(node, DATA_FIELD_CODE, 0), sizeof(Code));
    if(plot != NULL)
        fprintf(plot, "x");
    fprintf(out, "%s", VarTable->Vars[Code].name);
    return 0;
}