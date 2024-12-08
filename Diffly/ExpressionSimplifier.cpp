#include "Differentiator.h"

void* SimplifyExpression(tree_t* tree, void* node, __attribute((unused)) VariableTable_t* VarTable)
{
    if(tree == NULL || node == NULL)
    {
        return NULL;
    }

    size_t type = 0;
    memcpy(&type, GetNodeData(node, TYPE_FIELD_CODE, 0), sizeof(type));

    if(type == CONST_VALUE_TYPE_CODE)
    {
        return node;
    }
    
    int opcode = 0;
    memcpy(&opcode, GetNodeData(node, DATA_FIELD_CODE, 0), sizeof(opcode));

    /*if(type == FUNCTION_TYPE_CODE)
    {
        char* descAddr = NULL;
        memcpy(&descAddr, GetNodeData(node, DESCENDANTS_FIELD_CODE, 0), sizeof(descAddr));

        if(descAddr == NULL)
        {
            return NULL;
        }

        size_t descType = 0;
        memcpy(&descType, GetNodeData(descAddr, TYPE_FIELD_CODE, 0), sizeof(descType));

        if(descType == CONST_VALUE_TYPE_CODE)
        {
            double value = 0;
            memcpy(&value, GetNodeData(descAddr, DATA_FIELD_CODE, 0), sizeof(value));

            free(node);

            value = functions[opcode].calcFunction(value);
            return tree->InitNode(tree, NULL, CONST_VALUE_TYPE_CODE, sizeof(value), &value, 0);
        }
    }*/

    if(type == OPERATION_TYPE_CODE)
    {
        char* descendant1 = NULL;
        char* descendant2 = NULL;
        memcpy(&descendant1, GetNodeData(node, DESCENDANTS_FIELD_CODE, 0), sizeof(descendant1));
        memcpy(&descendant2, GetNodeData(node, DESCENDANTS_FIELD_CODE, 1), sizeof(descendant2));

        size_t type1 = 0;
        size_t type2 = 0;
        memcpy(&type1, GetNodeData(descendant1, TYPE_FIELD_CODE, 0), sizeof(type1));
        memcpy(&type2, GetNodeData(descendant2, TYPE_FIELD_CODE, 0), sizeof(type2));

        if(type1 == CONST_VALUE_TYPE_CODE)
        {
            double value1 = 0;
            memcpy(&value1, GetNodeData(descendant1, DATA_FIELD_CODE, 0), sizeof(value1));
            if(type2 == CONST_VALUE_TYPE_CODE)
            {
                double value2 = 0;
                memcpy(&value2, GetNodeData(descendant2, DATA_FIELD_CODE, 0), sizeof(value2));
                BurnBranch(node);
                double value = operations[opcode].CalcFunc(value1, value2);
                return INIT_C_NODE(&value);
            }

            if(doublecmp(value1, 0, EPS) == 0)
            {
                if(opcode == MUL_DIFF || opcode == POW_DIFF || opcode == DIV_DIFF)
                {
                    BurnBranch(node);
                    return INIT_C_NODE(&value1);
                }
                
                if(opcode == SUM_DIFF)
                {
                    free(descendant1);
                    free(node);
                    return descendant2;
                }
                
                if(opcode == SUB_DIFF)
                {
                    free(descendant1);
                    free(node);
                    double val = -1;
                    return MUL_NODE_DIFF(INIT_C_NODE(&val), descendant2);
                }
            }
            
            if(doublecmp(value1, 1, EPS) == 0)
            {
                if(opcode == MUL_DIFF)
                {
                    free(descendant1);
                    free(node);
                    return descendant2;
                }

                if(opcode == POW_DIFF)
                {
                    BurnBranch(descendant2);
                    return descendant1;
                }
            }
        }

        if(type2 == CONST_VALUE_TYPE_CODE)
        {
            double value2 = 0;
            memcpy(&value2, GetNodeData(descendant2, DATA_FIELD_CODE, 0), sizeof(value2));
            if(type1 == CONST_VALUE_TYPE_CODE)
            {
                MEOW("BOTH ARE CONST %d\n", __LINE__);
                double value1 = 0;
                memcpy(&value1, GetNodeData(descendant1, DATA_FIELD_CODE, 0), sizeof(value1));
                BurnBranch(node);
                double value = operations[opcode].CalcFunc(value1, value2);
                return INIT_C_NODE(&value);
            }

            if(doublecmp(value2, 0, EPS) == 0)
            {
                if(opcode == MUL_DIFF)
                {
                    BurnBranch(descendant1);
                    free(node);
                    return descendant2;
                }

                if(opcode == DIV_DIFF)
                {
                    return NULL;
                }

                if(opcode == POW_DIFF)
                {
                    BurnBranch(node);
                    double one_n = 1;
                    return INIT_C_NODE(&one_n);
                }
                
                if(opcode == SUM_DIFF)
                {
                    free(descendant2);
                    free(node);
                    return descendant1;
                }
            }
            
            if(doublecmp(value2, 1, EPS) == 0)
            {
                if(opcode == MUL_DIFF)
                {
                    free(descendant2);
                    free(node);
                    return descendant1;
                }

                if(opcode == POW_DIFF)
                {
                    free(node);
                    free(descendant2);
                    return descendant1;
                }
            }
        }
    }
    return node;
}