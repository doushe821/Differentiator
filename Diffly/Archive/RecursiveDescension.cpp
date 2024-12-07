#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#include "DBTools/DBTools.h"
#include "Differentiator.h"
#include "doublecmp/doublecmp.h"
#include "RecursiveDescension.h"

__attribute__((noreturn)) void SyntaxError(int* Index, char Literal, const char* func, const char* file, const int line);
static void* GetG(const char* ExpressionString, int* Index, tree_t* tree);
static void* GetE(const char* ExpressionString, int* Index, tree_t* tree);
static void* GetT(const char* ExpressionString, int* Index, tree_t* tree);
static void* GetP(const char* ExpressionString, int* Index, tree_t* tree);
static void* GetB(const char* ExpressionString, int* Index, tree_t* tree);
static void* GetF(const char* ExpressionString, int* Index, tree_t* tree);
static void* GetN(const char* ExpressionString, int* Index, tree_t* tree);
static void* GetV(const char* ExpressionString, int* Index, tree_t* tree);

int ReadCrocodile(const char* ExpressionString, tree_t* tree)
{
    int index = 0;
    tree->root = GetG(ExpressionString, &index, tree);
    if(tree->root == NULL)
    {
        return UNKNOWN_ERROR_DIFF;
    }
    return 0;
}

static void* GetG(const char* ExpressionString, int* Index, tree_t* tree)
{
    MEOW("//// %s summoned ////\n", __func__);
    void* root = GetE(ExpressionString, Index, tree);
    if(ExpressionString[*Index] != '$')
    {
        SyntaxError(Index, ExpressionString[*Index], __func__, __FILE__, __LINE__);
    }
    (*Index)++;
    return root;
}

static void* GetE(const char* ExpressionString, int* Index, tree_t* tree)
{
    MEOW("//// %s summoned ////\n", __func__);
    void* node1 = GetT(ExpressionString, Index, tree);
    void* node2 = NULL;
    int op = 0;
    double value = 0;
    void* root = node1;

    while(ExpressionString[*Index] == '+' || ExpressionString[*Index] == '-')
    {
        op = ExpressionString[*Index];
        (*Index)++;
        node2 = GetT(ExpressionString, Index, tree);
        

        size_t type1 = 0;
        size_t type2 = 0;
        memcpy(&type1, GetNodeData(node1, TYPE_FIELD_CODE, 0), sizeof(type1));
        memcpy(&type2, GetNodeData(node2, TYPE_FIELD_CODE, 0), sizeof(type2));
        MEOW("tpy1 %d, typ2 %d\n", type1, type2);
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

static void* GetT(const char* ExpressionString, int* Index, tree_t* tree)
{  
    MEOW("//// %s summoned, Index = %d ////\n", __func__, *Index);
    void* node1 = GetP(ExpressionString, Index, tree);
    void* node2 = NULL;
    void* root = node1;
    int op = 0;
    double value = 0;
    while(ExpressionString[*Index] == '*' || ExpressionString[*Index] == '/')
    {
        op = ExpressionString[*Index];
        (*Index)++;
        node2 = GetP(ExpressionString, Index, tree);
        

        size_t type1 = 0;
        size_t type2 = 0;
        memcpy(&type1, GetNodeData(node1, TYPE_FIELD_CODE, 0), sizeof(type1));
        memcpy(&type2, GetNodeData(node2, TYPE_FIELD_CODE, 0), sizeof(type2));
        MEOW("tpy1 %d, typ2 %d\n", type1, type2);
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
                MEOW("meow %d\n", __LINE__);
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
        }
    }
    return root;
} 

static void* GetP(const char* ExpressionString, int* Index, tree_t* tree)
{
    MEOW("//// %s summoned, Index = %d ////\n", __func__, *Index);
    void* node1 = GetB(ExpressionString, Index, tree);
    void* node2 = NULL;
    void* root = node1;
    double value = 0;
    while(ExpressionString[*Index] == '^')
    {
        (*Index)++;
        node2 = GetB(ExpressionString, Index, tree);
        

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

static void* GetB(const char* ExpressionString, int* Index, tree_t* tree)
{
    MEOW("//// %s summoned ////\n", __func__);
    if(ExpressionString[*Index] == '(')
    {
        (*Index)++;
        void* root = GetE(ExpressionString, Index, tree);
        {
            if(ExpressionString[*Index] != ')')
            {
                SyntaxError(Index, ExpressionString[*Index], __func__, __FILE__, __LINE__);
            }
            (*Index)++;
            return root;
        }
    }
    else
    {
        return GetF(ExpressionString, Index, tree);
    }
}

static void* GetF(const char* ExpressionString, int* Index, tree_t* tree)
{
    MEOW("//// %s summoned ////\n", __func__);
    int localIndex = *Index;
    char funcName[FUNCTION_NAME_MAX] = {};
    void* retNode = 0;
    while(isalpha(ExpressionString[localIndex]))
    {
        funcName[localIndex - *Index] = ExpressionString[localIndex];
        localIndex++;
    }
    MEOW("%s\n", funcName);
    int fCode = 0;
    if((fCode = FindFunction(funcName)) != -1)
    {
        *Index += localIndex - *Index;
        if(ExpressionString[*Index] == '(')
        {
            (*Index)++;
            void* node = GetE(ExpressionString, Index, tree);
            if(ExpressionString[*Index] != ')')
            {
                SyntaxError(Index, ExpressionString[*Index], __func__, __FILE__, __LINE__);
            }
            (*Index)++;
            if(fCode == functions[LOG_F].code)
            {
                if(ExpressionString[*Index] == '(')
                {
                    (*Index)++;
                    void* node1 = GetE(ExpressionString, Index, tree);
                    if(ExpressionString[*Index] != ')')
                    {
                        SyntaxError(Index, ExpressionString[*Index], __func__, __FILE__, __LINE__);
                    }
                    (*Index)++;
                    int type1 = 0;
                    memcpy(&type1, GetNodeData(node, TYPE_FIELD_CODE, 0), sizeof(type1));
                    int type2 = 0;
                    memcpy(&type2, GetNodeData(node1, TYPE_FIELD_CODE, 0), sizeof(type2));
                    return DIV_NODE_DIFF(LN_NODE_DIFF(node), LN_NODE_DIFF(node1));
                }
                else
                {
                    SyntaxError(Index, ExpressionString[*Index], __func__, __FILE__, __LINE__);
                }

            }

            retNode = tree->InitNode(tree, NULL, FUNCTION_TYPE_CODE, sizeof(fCode), &fCode, 1, node);
        } 
        else
        {
            SyntaxError(Index, ExpressionString[*Index], __func__, __FILE__, __LINE__);
        }
    }
    else
    {
        retNode = GetN(ExpressionString, Index, tree);
    }   
    return retNode;
}


static void* GetN(const char* ExpressionString, int* Index, tree_t* tree)
{
    MEOW("//// %s summoned ////\n", __func__);
    int oldIndex = *Index;
    int DigitsCounter = 0;
    double val = 0;
    if(isdigit(ExpressionString[*Index]))
    {
        while(('0' <= ExpressionString[*Index] && '9' >= ExpressionString[*Index]) || '.' == ExpressionString[*Index])
        {
            if(DigitsCounter > 0)
            {
                DigitsCounter++;
            }
            if(ExpressionString[*Index] == '.')
            {
                DigitsCounter++;
                (*Index)++;
                continue;
            }
            MEOW("%s current cymbol code = %d\n", __func__, ExpressionString[*Index]);
            val = val*10 + ExpressionString[*Index] - '0';
            (*Index)++;
        }
        if(*Index == oldIndex)
        {
            SyntaxError(Index, ExpressionString[*Index], __func__, __FILE__, __LINE__);
        }
        if(DigitsCounter > 0)
        {
            if(DigitsCounter == 1)
            {
                SyntaxError(Index, ExpressionString[*Index], __func__, __FILE__, __LINE__);
            }
            for(int i = 0; i < DigitsCounter - 1; i++)
            {
                val /= 10;
            }
        }
        MEOW("%s returning %lf\n", __func__, val);
        return tree->InitNode(tree, NULL, 0, sizeof(val), &val, 0);
    }

    else
    {
        return GetV(ExpressionString, Index, tree);
    }

}

static void* GetV(const char* ExpressionString, int* Index, tree_t* tree)
{
    MEOW("//// GetV summonded Index = %d/ ////\n", *Index);
    int oldIndex = *Index;
    int varc = 0;
    while(isalpha(ExpressionString[*Index]))
    {
        varc = ExpressionString[*Index];
        (*Index)++; 
    }
    if(*Index - oldIndex != 1)
    {
        SyntaxError(Index, ExpressionString[*Index], __func__, __FILE__, __LINE__);
    }
    return tree->InitNode(tree, NULL, (size_t)VARIABLE_TYPE_CODE, sizeof(int), &varc, 0);

}

__attribute__((noreturn)) void SyntaxError(int* Index, char Literal, const char* func, const char* file, const int line)
{
    fprintf(stderr, "Syntax error on p = %d, symbol is %c, in function %s in %s:%d\n", *Index, Literal, func, file, line);
    abort();
}