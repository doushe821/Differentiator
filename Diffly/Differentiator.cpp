#include "Differentiator.h"
#include <assert.h>

void* Differentiate(tree_t* tree, void* node)
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
            return DiffConst(tree, node);
        }
        case VARIABLE_TYPE_CODE:
        {
            return DiffVar(tree, node);
        }
        case OPERATION_TYPE_CODE:
        {
           return DifferentiateOperation(tree, node);
        }
        case FUNCTION_TYPE_CODE:
        {
            return DifferentiateFunction(tree, node);
        }
        default:
        {
            abort();
        }
        
    }
    return NULL;
}

void* DifferentiateFunction(tree_t* tree, void* node)
{
    MEOW(RED_COLOR_ESC_SEQ"%s summoned" DEFAULT_COLOR_ESC_SEQ, __func__);
    int fcode = 0;
    memcpy(&fcode, GetNodeData(node, DATA_FIELD_CODE, 0), sizeof(fcode));
    for(int i = 0; i < NUMBER_OF_FUNCTIONS; i++)
    {
        if(fcode == functions[i].code)
        {
            return functions[i].FDiffFunc(tree, node);
        }
    }
    abort();
    return NULL;
}

void* FDiffSin(tree_t* tree, void* node)
{
    void* diffedNode = tree->CloneTree(tree, node);
    memcpy(GetNodeData(diffedNode, DATA_FIELD_CODE, 0), &functions[COS_F].code, sizeof(functions[COS_F].code));

    char* argument = NULL;
    memcpy(&argument, GetNodeData(diffedNode, DESCENDANTS_FIELD_CODE, 0), sizeof(argument));
    

    return MUL_NODE_DIFF(diffedNode, Differentiate(tree, argument));
}

void* FDiffCos(tree_t* tree, void* node)
{
    double minus_one_n = -1;
    void* diffedNode = tree->CloneTree(tree, node);
    memcpy(GetNodeData(diffedNode, DATA_FIELD_CODE, 0), &functions[SIN_F].code, sizeof(functions[SIN_F].code));
    void* diffedFunc = MUL_NODE_DIFF(INIT_C_NODE(&minus_one_n), diffedNode);

    char* argument = NULL;
    memcpy(&argument, GetNodeData(node, DESCENDANTS_FIELD_CODE, 0), sizeof(argument));

    return SIMPLE(MUL_NODE_DIFF(Differentiate(tree, argument), diffedFunc));
}

void* FDiffArcsin(tree_t* tree, void* node)
{
    double one_n = 1;
    double two_n = 2;
    double half_n = 0.5;
    void* diffedNode = tree->CloneTree(tree, node);
    char* argument = NULL;
    memcpy(&argument, GetNodeData(diffedNode, DESCENDANTS_FIELD_CODE, 0), sizeof(argument));
    free(diffedNode);

    return MUL_NODE_DIFF(Differentiate(tree, argument), DIV_NODE_DIFF(INIT_C_NODE(&one_n), POW_NODE_DIFF(SUB_NODE_DIFF(INIT_C_NODE(&one_n), POW_NODE_DIFF(argument, INIT_C_NODE(&two_n))), INIT_C_NODE(&half_n))));
}

void* FDiffArccos(tree_t* tree, void* node)
{
    double minus_one_n = -1;
    double one_n = 1;
    double two_n = 2;
    double half_n = 0.5;

    void* diffedNode = tree->CloneTree(tree, node);
    char* argument = NULL;
    memcpy(&argument, GetNodeData(diffedNode, DESCENDANTS_FIELD_CODE, 0), sizeof(argument));
    free(diffedNode);

    return MUL_NODE_DIFF(Differentiate(tree, argument), DIV_NODE_DIFF(INIT_C_NODE(&minus_one_n), POW_NODE_DIFF(SUB_NODE_DIFF(INIT_C_NODE(&one_n), POW_NODE_DIFF(argument, INIT_C_NODE(&two_n))), INIT_C_NODE(&half_n))));
}


void* FDiffTg(tree_t* tree, void* node)
{
    double one_n = 1;
    double two_n = 2;

    void* diffedNode = tree->CloneTree(tree, node);
    char* argument = NULL;
    memcpy(&argument, GetNodeData(diffedNode, DESCENDANTS_FIELD_CODE, 0), sizeof(argument));
    free(diffedNode);  

    return MUL_NODE_DIFF(Differentiate(tree, argument), DIV_NODE_DIFF(INIT_C_NODE(&one_n), POW_NODE_DIFF(COS_NODE_DIFF(argument), INIT_C_NODE(&two_n))));
}

void* FDiffCtg(tree_t* tree, void* node)
{
    double minus_one_n = -1;
    double two_n = 2;

    void* diffedNode = tree->CloneTree(tree, node);
    char* argument = NULL;
    memcpy(&argument, GetNodeData(diffedNode, DESCENDANTS_FIELD_CODE, 0), sizeof(argument));
    free(diffedNode);  

    return SIMPLE(MUL_NODE_DIFF(Differentiate(tree, argument), DIV_NODE_DIFF(INIT_C_NODE(&minus_one_n), POW_NODE_DIFF(SIN_NODE_DIFF(argument), INIT_C_NODE(&two_n)))));
}

void* FDiffArctg(tree_t* tree, void* node)
{
    void* diffedNode = tree->CloneTree(tree, node);
    char* argument = NULL;
    memcpy(&argument, GetNodeData(diffedNode, DESCENDANTS_FIELD_CODE, 0), sizeof(argument));
    free(diffedNode);  
    
    double one_n = 1;
    double two_n = 2;

    return SIMPLE(MUL_NODE_DIFF(Differentiate(tree, argument), DIV_NODE_DIFF(INIT_C_NODE(&one_n), SUM_NODE_DIFF(INIT_C_NODE(&one_n), POW_NODE_DIFF(argument, INIT_C_NODE(&two_n))))));
}

void* FDiffArcctg(tree_t* tree, void* node)
{
    void* diffedNode = tree->CloneTree(tree, node);
    char* argument = NULL;
    memcpy(&argument, GetNodeData(diffedNode, DESCENDANTS_FIELD_CODE, 0), sizeof(argument));
    free(diffedNode);  
    
    double one_n = 1;
    double two_n = 2;
    double minus_one_n = -1;

    return SIMPLE(MUL_NODE_DIFF(Differentiate(tree, argument), DIV_NODE_DIFF(INIT_C_NODE(&minus_one_n), SUM_NODE_DIFF(INIT_C_NODE(&one_n), POW_NODE_DIFF(argument, INIT_C_NODE(&two_n))))));
}





void* FDiffSh(tree_t* tree, void* node)
{
    void* diffedNode = tree->CloneTree(tree, node);
    memcpy(GetNodeData(diffedNode, DATA_FIELD_CODE, 0), &functions[COSH_F].code, sizeof(functions[COSH_F].code));

    char* argument = NULL;
    memcpy(&argument, GetNodeData(diffedNode, DESCENDANTS_FIELD_CODE, 0), sizeof(argument));
    

    return SIMPLE(MUL_NODE_DIFF(diffedNode, Differentiate(tree, argument)));
}

void* FDiffCh(tree_t* tree, void* node)
{
    void* diffedNode = tree->CloneTree(tree, node);
    memcpy(GetNodeData(diffedNode, DATA_FIELD_CODE, 0), &functions[SINH_F].code, sizeof(functions[SINH_F].code));

    char* argument = NULL;
    memcpy(&argument, GetNodeData(diffedNode, DESCENDANTS_FIELD_CODE, 0), sizeof(argument));

    return SIMPLE(MUL_NODE_DIFF(diffedNode, Differentiate(tree, argument)));
}


void* FDiffArcsh(tree_t* tree, void* node)
{
    double one_n = 1;
    double two_n = 2;
    double half_n = 0.5;
    void* diffedNode = tree->CloneTree(tree, node);
    char* argument = NULL;
    memcpy(&argument, GetNodeData(diffedNode, DESCENDANTS_FIELD_CODE, 0), sizeof(argument));
    free(diffedNode);

    return MUL_NODE_DIFF(Differentiate(tree, argument), DIV_NODE_DIFF(INIT_C_NODE(&one_n), POW_NODE_DIFF(SUM_NODE_DIFF(INIT_C_NODE(&one_n), POW_NODE_DIFF(argument, INIT_C_NODE(&two_n))), INIT_C_NODE(&half_n))));
}

void* FDiffArcch(tree_t* tree, void* node)
{
    double one_n = 1;
    double two_n = 2;
    double half_n = 0.5;

    void* diffedNode = tree->CloneTree(tree, node);
    char* argument = NULL;
    memcpy(&argument, GetNodeData(diffedNode, DESCENDANTS_FIELD_CODE, 0), sizeof(argument));
    free(diffedNode);

    return MUL_NODE_DIFF(Differentiate(tree, argument), DIV_NODE_DIFF(INIT_C_NODE(&one_n), POW_NODE_DIFF(SUB_NODE_DIFF(POW_NODE_DIFF(argument, INIT_C_NODE(&two_n)), INIT_C_NODE(&one_n)), INIT_C_NODE(&half_n))));
}


void* FDiffTh(tree_t* tree, void* node)
{
    double one_n = 1;
    double two_n = 2;

    void* diffedNode = tree->CloneTree(tree, node);
    char* argument = NULL;
    memcpy(&argument, GetNodeData(diffedNode, DESCENDANTS_FIELD_CODE, 0), sizeof(argument));
    free(diffedNode);  

    return MUL_NODE_DIFF(Differentiate(tree, argument), DIV_NODE_DIFF(INIT_C_NODE(&one_n), POW_NODE_DIFF(COSH_NODE_DIFF(argument), INIT_C_NODE(&two_n))));
}

void* FDiffCth(tree_t* tree, void* node)
{
    double minus_one_n = -1;
    double two_n = 2;

    void* diffedNode = tree->CloneTree(tree, node);
    char* argument = NULL;
    memcpy(&argument, GetNodeData(diffedNode, DESCENDANTS_FIELD_CODE, 0), sizeof(argument));
    free(diffedNode);  

    return SIMPLE(MUL_NODE_DIFF(Differentiate(tree, argument), DIV_NODE_DIFF(INIT_C_NODE(&minus_one_n), POW_NODE_DIFF(SINH_NODE_DIFF(argument), INIT_C_NODE(&two_n)))));
}

void* FDiffArcth(tree_t* tree, void* node)
{
    void* diffedNode = tree->CloneTree(tree, node);
    char* argument = NULL;
    memcpy(&argument, GetNodeData(diffedNode, DESCENDANTS_FIELD_CODE, 0), sizeof(argument));
    free(diffedNode);  
    
    double one_n = 1;
    double two_n = 2;

    return S_MUL_NODE_DIFF(SIMPLE(Differentiate(tree, argument)), S_DIV_NODE_DIFF(INIT_C_NODE(&one_n), S_SUM_NODE_DIFF(INIT_C_NODE(&one_n), S_POW_NODE_DIFF(argument, INIT_C_NODE(&two_n)))));
}

void* FDiffArccth(tree_t* tree, void* node)
{
    void* diffedNode = tree->CloneTree(tree, node);
    char* argument = NULL;
    memcpy(&argument, GetNodeData(diffedNode, DESCENDANTS_FIELD_CODE, 0), sizeof(argument));
    free(diffedNode);  
    
    double one_n = 1;
    double two_n = 2;

    return S_MUL_NODE_DIFF(SIMPLE(Differentiate(tree, argument)), S_DIV_NODE_DIFF(INIT_C_NODE(&one_n), S_SUM_NODE_DIFF(INIT_C_NODE(&one_n), S_POW_NODE_DIFF(argument, INIT_C_NODE(&two_n)))));
}



void* FDiffLn(tree_t* tree, void* node)
{
    void* diffedNode = tree->CloneTree(tree, node);
    char* argument = NULL;
    memcpy(&argument, GetNodeData(diffedNode, DESCENDANTS_FIELD_CODE, 0), sizeof(argument));
    free(diffedNode);  

    return DIV_NODE_DIFF(Differentiate(tree, argument), argument);
}



void* DifferentiateOperation(tree_t* tree, void* node)
{
    MEOW(RED_COLOR_ESC_SEQ"%s summoned" DEFAULT_COLOR_ESC_SEQ, __func__);
    int opcode = 0;
    memcpy(&opcode, GetNodeData(node, DATA_FIELD_CODE, 0), sizeof(opcode));
    for(int i = 0; i< NUMBER_OF_OPERATIONS; i++)
    {
        if(opcode == operations[i].code)
        {
            return operations[i].DiffFunc(tree, node);
        }
    }
    MEOW(GREEN_COLOR_ESC_SEQ"I'm just a chill guy %s\n" DEFAULT_COLOR_ESC_SEQ, __TIME__);
    return NULL;
}

void* DiffPow(tree_t* tree, void* node)
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
            return S_MUL_NODE_DIFF(INIT_C_NODE(&value2), S_MUL_NODE_DIFF(SIMPLE(Differentiate(tree, desc1)), SIMPLE(diffedNode)));

            /*return SimplifyExpression(tree, 
            tree->InitNode(tree, NULL, OPERATION_TYPE_CODE, sizeof(operations[MUL_DIFF].code), &operations[MUL_DIFF].code, 2,
            tree->InitNode(tree, NULL, CONST_VALUE_TYPE_CODE, sizeof(value2), &value2, 0), 
            SIMPLE(tree->InitNode(tree, NULL, OPERATION_TYPE_CODE, sizeof(operations[MUL_DIFF].code), &operations[MUL_DIFF].code, 2, 
            SimplifyExpression(tree, Differentiate(tree, desc1)), diffedNode))));*/
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
            return S_MUL_NODE_DIFF(SIMPLE(Differentiate(tree, desc2)), S_MUL_NODE_DIFF(diffedNode, S_LN_NODE_DIFF(diffedDesc1)));
        }
    }
    return 0;
}

void* DiffMul(tree_t* tree, void* node)
{
    MEOW(RED_COLOR_ESC_SEQ"%s summoned" DEFAULT_COLOR_ESC_SEQ, __func__);
    char* desc1 = {};
    memcpy(&desc1, GetNodeData(node, DESCENDANTS_FIELD_CODE, 0), sizeof(desc1));
    void* desc1Copy = tree->CloneTree(tree, desc1);
    void* node1 = SIMPLE(Differentiate(tree, desc1));

    char* desc2 = {};
    memcpy(&desc2, GetNodeData(node, DESCENDANTS_FIELD_CODE, 1), sizeof(desc2));
    void* desc2Copy = tree->CloneTree(tree, desc2);
    void* node2 = SIMPLE(Differentiate(tree, desc2));

    void* term1 = SIMPLE(MUL_NODE_DIFF(node1, desc2Copy));

    void* term2 = SIMPLE(MUL_NODE_DIFF(node2, desc1Copy));

    return SIMPLE(SUM_NODE_DIFF(term1, term2));
}

void* DiffSum(tree_t* tree, void* node)
{
    int opCode = 0;
    memcpy(&opCode, GetNodeData(node, DATA_FIELD_CODE, 0), sizeof(opCode));

    char* desc1 = {};
    memcpy(&desc1, GetNodeData(node, DESCENDANTS_FIELD_CODE, 0), sizeof(desc1));
    void* node1 = SIMPLE(Differentiate(tree, desc1));
                                            // complex diff
    char* desc2 = {};
    memcpy(&desc2, GetNodeData(node, DESCENDANTS_FIELD_CODE, 1), sizeof(desc2));
    void* node2 = SIMPLE(Differentiate(tree, desc2));

    if(opCode == SUM_DIFF)
    {
        return SIMPLE(SUM_NODE_DIFF(node1, node2));
    }
    else
    {
        return SIMPLE(SUB_NODE_DIFF(node1, node2));
    }
}

void* DiffDiv(tree_t* tree, void* node)
{
    char* desc1 = {};
    memcpy(&desc1, GetNodeData(node, DESCENDANTS_FIELD_CODE, 0), sizeof(desc1));
    void* desc1Copy = tree->CloneTree(tree, desc1);
    void* node1 = SIMPLE(Differentiate(tree, desc1));
                            
    char* desc2 = {};
    memcpy(&desc2, GetNodeData(node, DESCENDANTS_FIELD_CODE, 1), sizeof(desc2));
    void* desc2Copy = tree->CloneTree(tree, desc2);
    void* desc2SecondCopy = tree->CloneTree(tree, desc2);
    void* node2 = SIMPLE(Differentiate(tree, desc2));

    void* term1 = SIMPLE(MUL_NODE_DIFF(node1, desc2Copy));

    void* term2 = SIMPLE(MUL_NODE_DIFF(node2, desc1Copy));

    double two_n = 2;
    void* dividend = SIMPLE(POW_NODE_DIFF(desc2SecondCopy, INIT_C_NODE(&two_n)));

    void* sub = SIMPLE(SUB_NODE_DIFF(term1, term2));

    return SIMPLE(DIV_NODE_DIFF(sub, dividend));
}

void* DiffVar(tree_t* tree, __attribute((unused))void* node) 
{
    MEOW(RED_COLOR_ESC_SEQ"%s summoned" DEFAULT_COLOR_ESC_SEQ, __func__);
    double value = 1;

    return tree->InitNode(tree, NULL, CONST_VALUE_TYPE_CODE, sizeof(value), &value, 0);
}

void* DiffPowF(__attribute((unused))tree_t* tree, __attribute((unused))void* node)
{

    return NULL;
}

void* DiffConst(tree_t* tree, __attribute((unused))void* node)
{
    MEOW(RED_COLOR_ESC_SEQ"%s summoned" DEFAULT_COLOR_ESC_SEQ, __func__);
    double value = 0;
    return tree->InitNode(tree, NULL, CONST_VALUE_TYPE_CODE, sizeof(value), &value, 0);
}

void ErrorParser(int code, int line)
{
    switch(code)
    {
        case FILE_OPENING_ERROR_DIFF:
        {
            fprintf(stderr, "Can't open file: %d\n", line);
            break;
        }
        case 0:
        {
            return;
        }
        case NULL_POINTER_DIFF:
        {
            fprintf(stderr, "Something that shouldn't return NULL returned it on line %d\n", line);
            abort();
        }
        default:
        {
            fprintf(stderr, "Unknown error: %d\n", line);
            break;
        }
    }
    fprintf(stderr, "%d\n", code);
    abort();
}