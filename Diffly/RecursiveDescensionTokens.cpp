#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#include "RecursiveDescensionTokens.h"
#include "DBTools/DBTools.h"
#include "doublecmp/doublecmp.h"

__attribute__((noreturn)) void SyntaxError(int* Index, Token_t token, const char* func, const char* file, const int line, const VariableTable_t* VarTable);
static void* GetG(const Token_t* const Tokens, int* Index, tree_t* tree , VariableTable_t* VarTable);
static void* GetE(const Token_t* const Tokens, int* Index, tree_t* tree , VariableTable_t* VarTable);
static void* GetT(const Token_t* const Tokens, int* Index, tree_t* tree , VariableTable_t* VarTable);
static void* GetP(const Token_t* const Tokens, int* Index, tree_t* tree , VariableTable_t* VarTable);
static void* GetB(const Token_t* const Tokens, int* Index, tree_t* tree , VariableTable_t* VarTable);
static void* GetF(const Token_t* const Tokens, int* Index, tree_t* tree , VariableTable_t* VarTable);
static void* GetN(const Token_t* const Tokens, int* Index, tree_t* tree , VariableTable_t* VarTable);
static void* GetV(const Token_t* const Tokens, int* Index, tree_t* tree , VariableTable_t* VarTable);

int ReadCrocodileTokens(const Token_t* const Tokens, tree_t* tree , VariableTable_t* VarTable)
{
    int index = 0;
    tree->root = GetG(Tokens, &index, tree, VarTable);
    if(tree->root == NULL)
    {
        return UNKNOWN_ERROR_DIFF;
    }
    return 0;

}

static void* GetG(const Token_t* const Tokens, int* Index, tree_t* tree , VariableTable_t* VarTable)
{
    void* root = GetE(Tokens, Index, tree, VarTable);
    if(Tokens[*Index].Data.Decimal != '$')
    {
        SyntaxError(Index, Tokens[*Index], __func__, __FILE__, __LINE__, VarTable);
    }
    (*Index)++;
    return root;
}

static void* GetE(const Token_t* const Tokens, int* Index, tree_t* tree , VariableTable_t* VarTable)
{
    void* node1 = GetT(Tokens, Index, tree, VarTable);
    void* node2 = NULL;
    int op = 0;
    double value = 0;
    void* root = node1;

    while(Tokens[*Index].Data.Decimal == '+' || Tokens[*Index].Data.Decimal == '-')
    {
        op = Tokens[*Index].Data.Decimal;
        (*Index)++;
        node2 = GetT(Tokens, Index, tree, VarTable);
        
        size_t type1 = 0;
        size_t type2 = 0;
        memcpy(&type1, GetNodeData(node1, TYPE_FIELD_CODE, 0), sizeof(type1));
        memcpy(&type2, GetNodeData(node2, TYPE_FIELD_CODE, 0), sizeof(type2));
        if(type1 == CONST_VALUE_TYPE_CODE && type2 == CONST_VALUE_TYPE_CODE)
        {
            double value1 = 0;
            double value2 = 0;
            memcpy(&value1, GetNodeData(node1, DATA_FIELD_CODE, 0), sizeof(value1));
            memcpy(&value2, GetNodeData(node2, DATA_FIELD_CODE, 0), sizeof(value2));
            if(op == '+')
            {
                value = value1 + value2;
            }
            else
            {
                value = value1 - value2;
            }
            free(node1);
            free(node2);
            node1 = NewNodeDiff(tree, NULL, CONST_VALUE_TYPE_CODE, sizeof(double), &value, 0);
            root = node1;
            continue;
        }
        else
        {
            int opcode = FindOperation(op);
            root = NewNodeDiff(tree, NULL, OPERATION_TYPE_CODE, sizeof(int), &opcode, 2, root, node2);
        }
    }
    return root;
}

static void* GetT(const Token_t* const Tokens, int* Index, tree_t* tree , VariableTable_t* VarTable)
{  
    void* node1 = GetP(Tokens, Index, tree, VarTable);
    void* node2 = NULL;
    void* root = node1;
    int op = 0;
    while(Tokens[*Index].Data.Decimal == '*' || Tokens[*Index].Data.Decimal == '/')
    {
        MEOW("%d\n\n\n\n\n\n\n", __LINE__);
        op = Tokens[*Index].Data.Decimal;
        (*Index)++;
        node2 = GetP(Tokens, Index, tree, VarTable);
        int opcode = FindOperation(op);
        root = NewNodeDiff(tree, NULL, OPERATION_TYPE_CODE, sizeof(int), &opcode, 2, root, node2);
        

        /*size_t type1 = 0;
        size_t type2 = 0;
        memcpy(&type1, GetNodeData(node1, TYPE_FIELD_CODE, 0), sizeof(type1));
        memcpy(&type2, GetNodeData(node2, TYPE_FIELD_CODE, 0), sizeof(type2));
        if(type1 == CONST_VALUE_TYPE_CODE && type2 == CONST_VALUE_TYPE_CODE)
        {
            double value1 = 0;
            double value2 = 0;
            memcpy(&value1, GetNodeData(node1, DATA_FIELD_CODE, 0), sizeof(value1));
            memcpy(&value2, GetNodeData(node2, DATA_FIELD_CODE, 0), sizeof(value2));
            if(op == '*')
            {
                value = value1 * value2;
            }
            else
            {
                value = value1 / value2;
            }
            free(node1);
            free(node2);
            node1 = NewNodeDiff(tree, NULL, CONST_VALUE_TYPE_CODE, sizeof(double), &value, 0);
            root = node1;
            continue;
        }
        else if((type1 == VARIABLE_TYPE_CODE && type2 == CONST_VALUE_TYPE_CODE) || (type1 == CONST_VALUE_TYPE_CODE && type2 == VARIABLE_TYPE_CODE))
        {
            double value1 = 0;
            int varc = 0;
            if(type1 == CONST_VALUE_TYPE_CODE)
            {
                memcpy(&value1, GetNodeData(node1, DATA_FIELD_CODE, 0), sizeof(value1));
                memcpy(&varc, GetNodeData(node2, DATA_FIELD_CODE, 0), sizeof(varc));
            }
            else
            {
                memcpy(&value1, GetNodeData(node2, DATA_FIELD_CODE, 0), sizeof(value1));
                memcpy(&varc, GetNodeData(node1, DATA_FIELD_CODE, 0), sizeof(varc));
            }

            if(doublecmp(value1, 1, EPS) == 0)
            {
                free(node1);
                free(node2);
                node1 = NewNodeDiff(tree, NULL, VARIABLE_TYPE_CODE, sizeof(varc), &varc, 0);
                root = node1;
                continue;
            }
            else if(doublecmp(value1, 0, EPS) == 0)
            {
                free(node1);
                free(node2);
                node1 = NewNodeDiff(tree, NULL, CONST_VALUE_TYPE_CODE, sizeof(value1), &value1, 0);
                root = node1;
                continue;
            }
            int opcode = FindOperation(op);
            root = NewNodeDiff(tree, NULL, OPERATION_TYPE_CODE, sizeof(int), &opcode, 2, root, node2);
        }
        else
        {
            int opcode = FindOperation(op);
            root = NewNodeDiff(tree, NULL, OPERATION_TYPE_CODE, sizeof(int), &opcode, 2, root, node2);
        }*/
    }
    return root;
} 

static void* GetP(const Token_t* const Tokens, int* Index, tree_t* tree , VariableTable_t* VarTable)
{
    MEOW("//// %s summoned, Index = %d ////\n", __func__, *Index);
    void* node1 = GetB(Tokens, Index, tree, VarTable);
    void* node2 = NULL;
    void* root = node1;
    double value = 0;
    while(Tokens[*Index].Data.Decimal == '^')
    {
        (*Index)++;
        node2 = GetB(Tokens, Index, tree, VarTable);
        

        size_t type1 = 0;
        size_t type2 = 0;
        memcpy(&type1, GetNodeData(node1, TYPE_FIELD_CODE, 0), sizeof(type1));
        memcpy(&type2, GetNodeData(node2, TYPE_FIELD_CODE, 0), sizeof(type2));
        MEOW("tpy1 %d, typ2 %d\n", type1, type2);
        if(type1 == CONST_VALUE_TYPE_CODE)
        {
            double value1 = 0;
            memcpy(&value1, GetNodeData(node1, DATA_FIELD_CODE, 0), sizeof(value1));
            if(type2 == CONST_VALUE_TYPE_CODE)
            {
                double value2 = 0;
                memcpy(&value2, GetNodeData(node2, DATA_FIELD_CODE, 0), sizeof(value2));
                value = pow(value1, value2);
                free(node1);
                free(node2);
                node1 = NewNodeDiff(tree, NULL, CONST_VALUE_TYPE_CODE, sizeof(double), &value, 0);
                root = node1;
                continue;
            }
            else if(type2 == VARIABLE_TYPE_CODE)
            {
                int varc = 0;
                memcpy(&varc, GetNodeData(node2, DATA_FIELD_CODE, 0), sizeof(varc));
                if((doublecmp(value1, 1, EPS) == 0) || (doublecmp(value1, 0, EPS) == 0))
                {
                    free(node1);
                    free(node2);
                    node1 = tree->InitNode(tree, NULL, CONST_VALUE_TYPE_CODE, sizeof(value1), &value1, 0);
                    root = node1;
                    continue;
                }
            }
        }
        else if(type1 == VARIABLE_TYPE_CODE)
        {
            int varc = 0;
            memcpy(&varc, GetNodeData(node1, DATA_FIELD_CODE, 0), sizeof(varc));
            if(type2 == CONST_VALUE_TYPE_CODE)
            {
                double value2 = 0;
                memcpy(&value2, GetNodeData(node2, DATA_FIELD_CODE, 0), sizeof(value2));
                if(doublecmp(value2, 0, EPS) == 0)
                {
                    double nodeval = 1;
                    free(node1);
                    free(node2);
                    node1 = tree->InitNode(tree, NULL, CONST_VALUE_TYPE_CODE, sizeof(nodeval), &nodeval, 0);
                    root = node1;
                    continue;
                }
                else if(doublecmp(value2, 1, EPS) == 0)
                {
                    free(node1);
                    free(node2);
                    tree->InitNode(tree, NULL, VARIABLE_TYPE_CODE, sizeof(varc), &varc, 0);
                    continue;
                }
            }
            else
            {
                root = POW_NODE_DIFF(INIT_C_NODE(&EULER), MUL_NODE_DIFF(node1, LN_NODE_DIFF(node2)));
                continue;
            }
        }
        root = POW_NODE_DIFF(root, node2);
    }
    return root;
}

static void* GetB(const Token_t* const Tokens, int* Index, tree_t* tree , VariableTable_t* VarTable)
{
    
    if(Tokens[*Index].Data.Decimal == '(')
    {
        (*Index)++;
        void* root = GetE(Tokens, Index, tree, VarTable);
        {
            if(Tokens[*Index].Data.Decimal != ')')
            {
                SyntaxError(Index, Tokens[*Index], __func__, __FILE__, __LINE__, VarTable);
            }
            (*Index)++;
            return root;
        }
    }
    else
    {
        return GetF(Tokens, Index, tree, VarTable);
    }
}

static void* GetF(const Token_t* const Tokens, int* Index, tree_t* tree , VariableTable_t* VarTable)
{
    void* retNode = NULL;
    if(Tokens[*Index].type == FUNCTION_TOKEN)
    {
        int fCode = Tokens[*Index].Data.Decimal;
        (*Index)++;
        if(Tokens[*Index].Data.Decimal != '(')
        {
            SyntaxError(Index, Tokens[*Index], __func__, __FILE__, __LINE__, VarTable);
        }
        (*Index)++;
        retNode = tree->InitNode(tree, NULL, FUNCTION_TYPE_CODE, sizeof(int), &fCode, 1, GetE(Tokens, Index, tree, VarTable));
        if(Tokens[*Index].Data.Decimal != ')')
        {
            SyntaxError(Index, Tokens[*Index], __func__, __FILE__, __LINE__, VarTable);   
        }
        (*Index)++;
        return retNode;
    }
    else
    {
        return GetN(Tokens, Index, tree, VarTable);
    }
    return retNode;
}


static void* GetN(const Token_t* const Tokens, int* Index, tree_t* tree , VariableTable_t* VarTable)
{
    if(Tokens[*Index].type == CONST_TOKEN)
    {
        return INIT_C_NODE(&Tokens[(*Index)++].Data.Float);
    }
    else
    {
        return GetV(Tokens, Index, tree, VarTable);
    }

}

static void* GetV(const Token_t* const Tokens, int* Index, tree_t* tree , __attribute((unused)) VariableTable_t* VarTable)
{
    return tree->InitNode(tree, NULL, VARIABLE_TYPE_CODE, sizeof(int), &Tokens[(*Index)++].Data.Decimal, 0);
}

__attribute__((noreturn)) void SyntaxError(int* Index, Token_t token, const char* func, const char* file, const int line, const VariableTable_t* VarTable)
{
    fprintf(stderr, "Syntax error on p = %d, in function %s in %s:%d\n", *Index, func, file, line);
    PrintToken(token, VarTable);
    abort();
}