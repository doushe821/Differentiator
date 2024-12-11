#include <time.h>
#include <assert.h>

#include "../Headers/Transitions.h"
#include "../Headers/Differentiator.h"
#include "../Headers/VariableTable.h"

void* Differentiate(tree_t* tree, void* node, __attribute((unused)) FILE* TexOut, FILE* PlotOut, VariableTable_t* VarTable)
{
    assert(tree);
    if(tree == NULL)
    {
        return NULL;
    }
    
    size_t type = 0;
    memcpy(&type, GetNodeData(node, TYPE_FIELD_CODE, 0), sizeof(type));

    switch(type)
    {
        case CONST_VALUE_TYPE_CODE:
        {
            //DumpTreeTex(node, TexOut, NULL, VarTable);
            //fprintf(TexOut, "\\Downarrow\n");
            void* tptr = SIMPLE(DiffConst(tree, node, TexOut, PlotOut, VarTable));
            //fprintf(TexOut, "\\end{center}");
            //fprintf(TexOut, "\\[0\\]\n");
            //fprintf(TexOut, "\\]");
            return tptr;
        }
        case VARIABLE_TYPE_CODE:
        {
            //DumpTreeTex(node, TexOut, NULL, VarTable);
            //fprintf(TexOut, "\\Downarrow\n");
            void* tptr = SIMPLE(DiffVar(tree, node, TexOut, PlotOut, VarTable));
            //fprintf(TexOut, "\\end{center}");
            

            //fprintf(TexOut, "\\]");
            return tptr;
        }
        case OPERATION_TYPE_CODE:
        {
            //DumpTreeTex(node, TexOut, NULL, VarTable);
            //fprintf(TexOut, "\\Downarrow\n");
            void* tptr = SIMPLE(DifferentiateOperation(tree, node, TexOut, PlotOut, VarTable));
            // DumpTreeTex(tptr, TexOut, NULL, VarTable);
            //fprintf(TexOut, "\\end{center}");
            //fprintf(TexOut, "\\]");
            return tptr;
        }
        case FUNCTION_TYPE_CODE:
        {
            //DumpTreeTex(node, TexOut, NULL, VarTable);
            //fprintf(TexOut, "\\Downarrow\n");
            void* tptr = SIMPLE(DifferentiateFunction(tree, node, TexOut, PlotOut, VarTable));
            //DumpTreeTex(tptr, TexOut, NULL, VarTable);
            //fprintf(TexOut, "\\end{center}");
            //fprintf(TexOut, "\\]");
            return tptr;
        }
        default:
        {
            MEOW("I'm not a chill guy %s\n", __TIME__);
            abort();
        }
        
    }
    return NULL;
}

void* DifferentiateFunction(tree_t* tree, void* node, __attribute((unused)) FILE* TexOut, __attribute((unused)) FILE* PlotOut, VariableTable_t* VarTable)
{
    int fcode = 0;
    memcpy(&fcode, GetNodeData(node, DATA_FIELD_CODE, 0), sizeof(fcode));
    for(int i = 0; i < NUMBER_OF_FUNCTIONS; i++)
    {
        if(fcode == functions[i].code)
        {
            return SIMPLE(functions[i].FDiffFunc(tree, node, TexOut, PlotOut, VarTable));
        }
    }

    return NULL;
}

void* FDiffSin(tree_t* tree, void* node, __attribute((unused)) FILE* TexOut, __attribute((unused)) FILE* PlotOut, VariableTable_t* VarTable)
{
    if(TexOut != NULL)
    {
        DumpTreeTex(node, TexOut, NULL, VarTable);
        fprintf(TexOut, "\\[\\Downarrow\\]\n");
    }

    if(TexOut != NULL)
    {
        fprintf(TexOut, "\n%s\n", TRANSITIONS[rand()%20]);
    }
    DumpTreeTex(node, TexOut, NULL, VarTable);

    void* diffedNode = tree->CloneTree(tree, node);
    memcpy(GetNodeData(diffedNode, DATA_FIELD_CODE, 0), &functions[COS_F].code, sizeof(functions[COS_F].code));

    char* argument = NULL;
    memcpy(&argument, GetNodeData(diffedNode, DESCENDANTS_FIELD_CODE, 0), sizeof(argument));
    

    return S_MUL_NODE_DIFF(SIMPLE(diffedNode), SIMPLE(Differentiate(tree, argument, TexOut,  PlotOut, VarTable)));
}

void* FDiffCos(tree_t* tree, void* node, __attribute((unused)) FILE* TexOut, FILE* PlotOut, VariableTable_t* VarTable)
{
    if(TexOut != NULL)
    {
        DumpTreeTex(node, TexOut, NULL, VarTable);
        fprintf(TexOut, "\\[\\Downarrow\\]\n");
    }

    double minus_one_n = -1;
    void* diffedNode = tree->CloneTree(tree, node);
    memcpy(GetNodeData(diffedNode, DATA_FIELD_CODE, 0), &functions[SIN_F].code, sizeof(functions[SIN_F].code));
    void* diffedFunc = S_MUL_NODE_DIFF(INIT_C_NODE(&minus_one_n), SIMPLE(diffedNode));

    char* argument = NULL;
    memcpy(&argument, GetNodeData(node, DESCENDANTS_FIELD_CODE, 0), sizeof(argument));

    return S_MUL_NODE_DIFF(SIMPLE(Differentiate(tree, SIMPLE(argument), TexOut, PlotOut, VarTable)), diffedFunc);
}

void* FDiffArcsin(tree_t* tree, void* node, __attribute((unused)) FILE* TexOut, FILE* PlotOut, VariableTable_t* VarTable)
{
    if(TexOut != NULL)
    {
        DumpTreeTex(node, TexOut, NULL, VarTable);
        fprintf(TexOut, "\\[\\Downarrow\\]\n");
    }

    double one_n = 1;
    double two_n = 2;
    double half_n = 0.5;
    void* diffedNode = tree->CloneTree(tree, node);
    char* argument = NULL;
    memcpy(&argument, GetNodeData(diffedNode, DESCENDANTS_FIELD_CODE, 0), sizeof(argument));
    free(diffedNode);

    return MUL_NODE_DIFF(Differentiate(tree, argument, TexOut,  PlotOut, VarTable), DIV_NODE_DIFF(INIT_C_NODE(&one_n), POW_NODE_DIFF(SUB_NODE_DIFF(INIT_C_NODE(&one_n), POW_NODE_DIFF(argument, INIT_C_NODE(&two_n))), INIT_C_NODE(&half_n))));
}

void* FDiffArccos(tree_t* tree, void* node, __attribute((unused)) FILE* TexOut, FILE* PlotOut, VariableTable_t* VarTable)
{
    if(TexOut != NULL)
    {
        DumpTreeTex(node, TexOut, NULL, VarTable);
        fprintf(TexOut, "\\[\\Downarrow\\]\n");
    }

    double minus_one_n = -1;
    double one_n = 1;
    double two_n = 2;
    double half_n = 0.5;

    void* diffedNode = tree->CloneTree(tree, node);
    char* argument = NULL;
    memcpy(&argument, GetNodeData(diffedNode, DESCENDANTS_FIELD_CODE, 0), sizeof(argument));
    free(diffedNode);

    return MUL_NODE_DIFF(Differentiate(tree, argument, TexOut,  PlotOut, VarTable), DIV_NODE_DIFF(INIT_C_NODE(&minus_one_n), POW_NODE_DIFF(SUB_NODE_DIFF(INIT_C_NODE(&one_n), POW_NODE_DIFF(argument, INIT_C_NODE(&two_n))), INIT_C_NODE(&half_n))));
}


void* FDiffTg(tree_t* tree, void* node, __attribute((unused)) FILE* TexOut, __attribute((unused)) FILE* PlotOut, VariableTable_t* VarTable)
{
    if(TexOut != NULL)
    {
        DumpTreeTex(node, TexOut, NULL, VarTable);
        fprintf(TexOut, "\\[\\Downarrow\\]\n");
    }

    double one_n = 1;
    double two_n = 2;

    void* diffedNode = tree->CloneTree(tree, node);
    char* argument = NULL;
    memcpy(&argument, GetNodeData(diffedNode, DESCENDANTS_FIELD_CODE, 0), sizeof(argument));
    free(diffedNode);  

    return MUL_NODE_DIFF(Differentiate(tree, argument, TexOut,  PlotOut, VarTable), DIV_NODE_DIFF(INIT_C_NODE(&one_n), POW_NODE_DIFF(COS_NODE_DIFF(argument), INIT_C_NODE(&two_n))));
}

void* FDiffCtg(tree_t* tree, void* node, __attribute((unused)) FILE* TexOut, __attribute((unused)) FILE* PlotOut, VariableTable_t* VarTable)
{
    double minus_one_n = -1;
    double two_n = 2;

    void* diffedNode = tree->CloneTree(tree, node);
    char* argument = NULL;
    memcpy(&argument, GetNodeData(diffedNode, DESCENDANTS_FIELD_CODE, 0), sizeof(argument));
    free(diffedNode);  

    return SIMPLE(MUL_NODE_DIFF(Differentiate(tree, argument, TexOut,  PlotOut, VarTable), DIV_NODE_DIFF(INIT_C_NODE(&minus_one_n), POW_NODE_DIFF(SIN_NODE_DIFF(argument), INIT_C_NODE(&two_n)))));
}

void* FDiffArctg(tree_t* tree, void* node, __attribute((unused)) FILE* TexOut, __attribute((unused)) FILE* PlotOut, VariableTable_t* VarTable)
{
    void* diffedNode = tree->CloneTree(tree, node);
    char* argument = NULL;
    memcpy(&argument, GetNodeData(diffedNode, DESCENDANTS_FIELD_CODE, 0), sizeof(argument));
    free(diffedNode);  
    
    double one_n = 1;
    double two_n = 2;

    return SIMPLE(MUL_NODE_DIFF(Differentiate(tree, argument, TexOut,  PlotOut, VarTable), DIV_NODE_DIFF(INIT_C_NODE(&one_n), SUM_NODE_DIFF(INIT_C_NODE(&one_n), POW_NODE_DIFF(argument, INIT_C_NODE(&two_n))))));
}

void* FDiffArcctg(tree_t* tree, void* node, __attribute((unused)) FILE* TexOut, __attribute((unused)) FILE* PlotOut, VariableTable_t* VarTable)
{
    void* diffedNode = tree->CloneTree(tree, node);
    char* argument = NULL;
    memcpy(&argument, GetNodeData(diffedNode, DESCENDANTS_FIELD_CODE, 0), sizeof(argument));
    free(diffedNode);  
    
    double one_n = 1;
    double two_n = 2;
    double minus_one_n = -1;

    return SIMPLE(MUL_NODE_DIFF(Differentiate(tree, argument, TexOut,  PlotOut, VarTable), DIV_NODE_DIFF(INIT_C_NODE(&minus_one_n), SUM_NODE_DIFF(INIT_C_NODE(&one_n), POW_NODE_DIFF(argument, INIT_C_NODE(&two_n))))));
}

void* FDiffSh(tree_t* tree, void* node, __attribute((unused)) FILE* TexOut, __attribute((unused)) FILE* PlotOut, VariableTable_t* VarTable)
{
    if(TexOut != NULL)
    {
        DumpTreeTex(node, TexOut, NULL, VarTable);
        fprintf(TexOut, "\\[\\Downarrow\\]\n");
    }

    void* diffedNode = tree->CloneTree(tree, node);
    memcpy(GetNodeData(diffedNode, DATA_FIELD_CODE, 0), &functions[COSH_F].code, sizeof(functions[COSH_F].code));

    char* argument = NULL;
    memcpy(&argument, GetNodeData(diffedNode, DESCENDANTS_FIELD_CODE, 0), sizeof(argument));
    

    return SIMPLE(MUL_NODE_DIFF(diffedNode, Differentiate(tree, argument, TexOut,  PlotOut, VarTable)));
}

void* FDiffCh(tree_t* tree, void* node, __attribute((unused)) FILE* TexOut, __attribute((unused)) FILE* PlotOut, VariableTable_t* VarTable)
{
    void* diffedNode = tree->CloneTree(tree, node);
    memcpy(GetNodeData(diffedNode, DATA_FIELD_CODE, 0), &functions[SINH_F].code, sizeof(functions[SINH_F].code));

    char* argument = NULL;
    memcpy(&argument, GetNodeData(diffedNode, DESCENDANTS_FIELD_CODE, 0), sizeof(argument));

    return SIMPLE(MUL_NODE_DIFF(diffedNode, Differentiate(tree, argument, TexOut,  PlotOut, VarTable)));
}


void* FDiffArcsh(tree_t* tree, void* node, __attribute((unused)) FILE* TexOut, __attribute((unused)) FILE* PlotOut, VariableTable_t* VarTable)
{
    double one_n = 1;
    double two_n = 2;
    double half_n = 0.5;
    void* diffedNode = tree->CloneTree(tree, node);
    char* argument = NULL;
    memcpy(&argument, GetNodeData(diffedNode, DESCENDANTS_FIELD_CODE, 0), sizeof(argument));
    free(diffedNode);

    return MUL_NODE_DIFF(Differentiate(tree, argument, TexOut,  PlotOut, VarTable), DIV_NODE_DIFF(INIT_C_NODE(&one_n), POW_NODE_DIFF(SUM_NODE_DIFF(INIT_C_NODE(&one_n), POW_NODE_DIFF(argument, INIT_C_NODE(&two_n))), INIT_C_NODE(&half_n))));
}

void* FDiffArcch(tree_t* tree, void* node, __attribute((unused)) FILE* TexOut, __attribute((unused)) FILE* PlotOut, VariableTable_t* VarTable)
{
    double one_n = 1;
    double two_n = 2;
    double half_n = 0.5;

    void* diffedNode = tree->CloneTree(tree, node);
    char* argument = NULL;
    memcpy(&argument, GetNodeData(diffedNode, DESCENDANTS_FIELD_CODE, 0), sizeof(argument));
    free(diffedNode);

    return MUL_NODE_DIFF(Differentiate(tree, argument, TexOut, PlotOut, VarTable), DIV_NODE_DIFF(INIT_C_NODE(&one_n), POW_NODE_DIFF(SUB_NODE_DIFF(POW_NODE_DIFF(argument, INIT_C_NODE(&two_n)), INIT_C_NODE(&one_n)), INIT_C_NODE(&half_n))));
}


void* FDiffTh(tree_t* tree, void* node, __attribute((unused)) FILE* TexOut, __attribute((unused)) FILE* PlotOut, VariableTable_t* VarTable)
{
    double one_n = 1;
    double two_n = 2;

    void* diffedNode = tree->CloneTree(tree, node);
    char* argument = NULL;
    memcpy(&argument, GetNodeData(diffedNode, DESCENDANTS_FIELD_CODE, 0), sizeof(argument));
    free(diffedNode);  

    return MUL_NODE_DIFF(Differentiate(tree, argument, TexOut,  PlotOut, VarTable), DIV_NODE_DIFF(INIT_C_NODE(&one_n), POW_NODE_DIFF(COSH_NODE_DIFF(argument), INIT_C_NODE(&two_n))));
}

void* FDiffCth(tree_t* tree, void* node, __attribute((unused)) FILE* TexOut, __attribute((unused)) FILE* PlotOut, VariableTable_t* VarTable)
{
    double minus_one_n = -1;
    double two_n = 2;

    void* diffedNode = tree->CloneTree(tree, node);
    char* argument = NULL;
    memcpy(&argument, GetNodeData(diffedNode, DESCENDANTS_FIELD_CODE, 0), sizeof(argument));
    free(diffedNode);  

    return SIMPLE(MUL_NODE_DIFF(Differentiate(tree, argument, TexOut,  PlotOut, VarTable), DIV_NODE_DIFF(INIT_C_NODE(&minus_one_n), POW_NODE_DIFF(SINH_NODE_DIFF(argument), INIT_C_NODE(&two_n)))));
}

void* FDiffArcth(tree_t* tree, void* node, __attribute((unused)) FILE* TexOut, __attribute((unused)) FILE* PlotOut, VariableTable_t* VarTable)
{
    void* diffedNode = tree->CloneTree(tree, node);
    char* argument = NULL;
    memcpy(&argument, GetNodeData(diffedNode, DESCENDANTS_FIELD_CODE, 0), sizeof(argument));
    free(diffedNode);  
    
    double one_n = 1;
    double two_n = 2;

    return S_MUL_NODE_DIFF(SIMPLE(Differentiate(tree, argument, TexOut,  PlotOut, VarTable)), S_DIV_NODE_DIFF(INIT_C_NODE(&one_n), S_SUM_NODE_DIFF(INIT_C_NODE(&one_n), S_POW_NODE_DIFF(argument, INIT_C_NODE(&two_n)))));
}

void* FDiffArccth(tree_t* tree, void* node, __attribute((unused)) FILE* TexOut, __attribute((unused)) FILE* PlotOut, VariableTable_t* VarTable)
{
    void* diffedNode = tree->CloneTree(tree, node);
    char* argument = NULL;
    memcpy(&argument, GetNodeData(diffedNode, DESCENDANTS_FIELD_CODE, 0), sizeof(argument));
    free(diffedNode);  
    
    double one_n = 1;
    double two_n = 2;

    return S_MUL_NODE_DIFF(SIMPLE(Differentiate(tree, argument, TexOut,  PlotOut, VarTable)), S_DIV_NODE_DIFF(INIT_C_NODE(&one_n), S_SUM_NODE_DIFF(INIT_C_NODE(&one_n), S_POW_NODE_DIFF(argument, INIT_C_NODE(&two_n)))));
}



void* FDiffLn(tree_t* tree, void* node, __attribute((unused)) FILE* TexOut, __attribute((unused)) FILE* PlotOut, VariableTable_t* VarTable)
{
    if(TexOut != NULL)
    {
        DumpTreeTex(node, TexOut, NULL, VarTable);
        fprintf(TexOut, "\\[\\Downarrow\\]\n");
    }

    void* diffedNode = tree->CloneTree(tree, node);
    char* argument = NULL;
    memcpy(&argument, GetNodeData(diffedNode, DESCENDANTS_FIELD_CODE, 0), sizeof(argument));
    free(diffedNode);  

    return DIV_NODE_DIFF(Differentiate(tree, argument, TexOut,  PlotOut, VarTable), argument);
}



void* DifferentiateOperation(tree_t* tree, void* node, __attribute((unused)) FILE* TexOut, __attribute((unused)) FILE* PlotOut, VariableTable_t* VarTable)
{
    
    int opcode = 0;
    memcpy(&opcode, GetNodeData(node, DATA_FIELD_CODE, 0), sizeof(opcode));
    for(int i = 0; i< NUMBER_OF_OPERATIONS; i++)
    {
        if(opcode == operations[i].code)
        {
            if(TexOut != NULL)
            {
                DumpTreeTex(node, TexOut, NULL, VarTable);
                fprintf(TexOut, "\\[\\Downarrow\\]\n");
            }
            return operations[i].DiffFunc(tree, node, TexOut, PlotOut, VarTable);
        }
    }
    MEOW(GREEN_COLOR_ESC_SEQ"I'm just a chill guy %s\n" DEFAULT_COLOR_ESC_SEQ, __TIME__);
    return NULL;
}

void* DiffPow(tree_t* tree, void* node, __attribute((unused)) FILE* TexOut, __attribute((unused)) FILE* PlotOut, VariableTable_t* VarTable)
{

    void* diffedNode = tree->CloneTree(tree, node);
    char* desc1 = {};
    memcpy(&desc1, GetNodeData(diffedNode, DESCENDANTS_FIELD_CODE, 0), sizeof(desc1));
    size_t type1 = 0;
    memcpy(&type1, GetNodeData(desc1, TYPE_FIELD_CODE, 0), sizeof(type1));

    char* desc2 = {};
    memcpy(&desc2, GetNodeData(diffedNode, DESCENDANTS_FIELD_CODE, 1), sizeof(desc2));
    size_t type2 = 0;
    memcpy(&type2, GetNodeData(desc2, TYPE_FIELD_CODE, 0), sizeof(type2));

    if(type1 != CONST_VALUE_TYPE_CODE)
    {
        if(type2 == CONST_VALUE_TYPE_CODE)
        {
            double value2 = 0;
            memcpy(&value2, GetNodeData(desc2, DATA_FIELD_CODE, 0), sizeof(value2));

            double newVal = value2 - 1;


            memcpy(GetNodeData(desc2, DATA_FIELD_CODE, 0), &newVal, sizeof(newVal));
            return S_MUL_NODE_DIFF(INIT_C_NODE(&value2), S_MUL_NODE_DIFF(SIMPLE(Differentiate(tree, desc1, TexOut, PlotOut, VarTable)), SIMPLE(diffedNode)));

            /*return SimplifyExpression(tree, 
            tree->InitNode(tree, NULL, OPERATION_TYPE_CODE, sizeof(operations[MUL_DIFF].code), &operations[MUL_DIFF].code, 2,
            tree->InitNode(tree, NULL, CONST_VALUE_TYPE_CODE, sizeof(value2), &value2, 0), 
            SIMPLE(tree->InitNode(tree, NULL, OPERATION_TYPE_CODE, sizeof(operations[MUL_DIFF].code), &operations[MUL_DIFF].code, 2, 
            3fyExpression(tree, Differentiate(tree, desc1)), diffedNode))));*/
        }
        else
        {
            abort();// under construction
        }
    }
    else
    {
        if(type2 != CONST_VALUE_TYPE_CODE)
        {
            void* diffedDesc1 = tree->CloneTree(tree, desc1);
            //void* diffedDesc2 = tree->CloneTree(tree, desc2);
            return S_MUL_NODE_DIFF(SIMPLE(Differentiate(tree, desc2, TexOut, PlotOut, VarTable)), S_MUL_NODE_DIFF(SIMPLE(diffedNode), S_LN_NODE_DIFF(diffedDesc1)));
        }
    }
    return 0;
}

void* DiffMul(tree_t* tree, void* node, __attribute((unused)) FILE* TexOut, __attribute((unused)) FILE* PlotOut, VariableTable_t* VarTable)
{
    
    char* desc1 = {};
    memcpy(&desc1, GetNodeData(node, DESCENDANTS_FIELD_CODE, 0), sizeof(desc1));
    void* desc1Copy = SIMPLE(tree->CloneTree(tree, desc1));
    void* node1 = SIMPLE(Differentiate(tree, desc1, TexOut, PlotOut, VarTable));

    char* desc2 = {};
    memcpy(&desc2, GetNodeData(node, DESCENDANTS_FIELD_CODE, 1), sizeof(desc2));
    void* desc2Copy = SIMPLE(tree->CloneTree(tree, desc2));
    void* node2 = SIMPLE(Differentiate(tree, desc2, TexOut, PlotOut, VarTable));

    void* term1 = S_MUL_NODE_DIFF(node1, desc2Copy);

    void* term2 = S_MUL_NODE_DIFF(node2, desc1Copy);
    
    return SimplifyExpression(tree, SUM_NODE_DIFF(term1, term2), VarTable);
}

void* DiffSum(tree_t* tree, void* node, __attribute((unused)) FILE* TexOut, __attribute((unused)) FILE* PlotOut, VariableTable_t* VarTable)
{
    int opCode = 0;
    memcpy(&opCode, GetNodeData(node, DATA_FIELD_CODE, 0), sizeof(opCode));

    char* desc1 = {};
    memcpy(&desc1, GetNodeData(node, DESCENDANTS_FIELD_CODE, 0), sizeof(desc1));
    void* node1 = SIMPLE(Differentiate(tree, desc1, TexOut, PlotOut, VarTable));
                                        
    char* desc2 = {};
    memcpy(&desc2, GetNodeData(node, DESCENDANTS_FIELD_CODE, 1), sizeof(desc2));
    void* node2 = SIMPLE(Differentiate(tree, desc2, TexOut, PlotOut, VarTable));

    if(opCode == SUM_DIFF)
    {
        return SIMPLE(SUM_NODE_DIFF(node1, node2));
    }
    else
    {
        return SIMPLE(SUB_NODE_DIFF(node1, node2));
    }
}

void* DiffDiv(tree_t* tree, void* node, __attribute((unused)) FILE* TexOut, __attribute((unused)) FILE* PlotOut, VariableTable_t* VarTable)
{
    char* desc1 = {};
    memcpy(&desc1, GetNodeData(node, DESCENDANTS_FIELD_CODE, 0), sizeof(desc1));
    void* desc1Copy = SIMPLE(tree->CloneTree(tree, desc1));
    void* node1 = SIMPLE(Differentiate(tree, desc1Copy, TexOut, PlotOut, VarTable));
                            
    char* desc2 = {};
    memcpy(&desc2, GetNodeData(node, DESCENDANTS_FIELD_CODE, 1), sizeof(desc2));
    void* desc2Copy = SIMPLE(tree->CloneTree(tree, desc2));
    void* desc2SecondCopy = SIMPLE(tree->CloneTree(tree, desc2));
    void* node2 = SIMPLE(Differentiate(tree, desc2Copy, TexOut, PlotOut, VarTable));

    void* term1 = S_MUL_NODE_DIFF(node1, desc2Copy);

    void* term2 = S_MUL_NODE_DIFF(node2, desc1Copy);

    double two_n = 2;
    void* dividend = S_POW_NODE_DIFF(desc2SecondCopy, INIT_C_NODE(&two_n));

    void* sub = S_SUB_NODE_DIFF(term1, term2);

    return S_DIV_NODE_DIFF(sub, dividend);
}

void* DiffVar(tree_t* tree, __attribute((unused))void* node, __attribute((unused)) FILE* TexOut, __attribute((unused)) FILE* PlotOut, __attribute((unused)) VariableTable_t* VarTable) 
{
    if(TexOut != NULL)
    {
        DumpTreeTex(node, TexOut, NULL, VarTable);
        fprintf(TexOut, "\\[\\Downarrow\\]\n");
        fprintf(TexOut, "\\[1\\]\n");
    }
    double value = 1;

    return tree->InitNode(tree, NULL, CONST_VALUE_TYPE_CODE, sizeof(value), &value, 0);
}

void* DiffConst(tree_t* tree, __attribute((unused))void* node, __attribute((unused)) FILE* TexOut, __attribute((unused)) FILE* PlotOut, __attribute((unused)) VariableTable_t* VarTable)
{
    if(TexOut != NULL)
    {
        DumpTreeTex(node, TexOut, NULL, VarTable);
        fprintf(TexOut, "\\[\\Downarrow\\]\n");
        fprintf(TexOut, "\\[0\\]\n");
    }
    double value = 0;
    return tree->InitNode(tree, NULL, CONST_VALUE_TYPE_CODE, sizeof(value), &value, 0);
}

void ErrorParser(const int code, const int line, const char* file)
{
    switch(code)
    {
        case 0:
        {
            return;
        }
        case FILE_OPENING_ERROR_DIFF:
        {
            fprintf(stderr, "Can't open file: %s:%d\n", file, line);
            break;
        }
        case NULL_POINTER_DIFF:
        {
            fprintf(stderr, "Something that shouldn't return NULL returned it on line %s:%d\n", file, line);
            abort();
        }
        case NO_INPUT_FILE:
        {
            fprintf(stderr, "No input file detected: you need to write name of the file with your expression %s:%d\n", file, line);
            abort();
        }
        case DEGREE_IS_TOO_BIG:
        {
            fprintf(stderr, "Given degree for Taylor series is too big, please, select a number that ranges from 1 to 5.\n");
            abort();
        }
        case EMPTY_INPUT_STRING:
        {
            fprintf(stderr, "Input file is empty: %s:%d\n", file, line);
            abort();
        }
        case TOKENISATION_FAILURE:
        {
            fprintf(stderr, "Inhuman horrors happened while tokenising: %s:%d\n", file, line);
            abort();
        }
        default:
        {
            fprintf(stderr, "Unknown error: %s:%d\n", file, line);
            break;
        }
    }
    fprintf(stderr, "%d\n", code);
    abort();
}