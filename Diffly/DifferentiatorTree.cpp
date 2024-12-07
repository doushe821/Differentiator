#include "DifferentiatorTree.h"
#include "Differentiator.h"
#include <string.h>
#include <stdlib.h>

static void* CloneNodeDiff(const void* tree, void* node);

void* NewNodeDiff(const void* tree, const void* parent, const size_t type, const size_t datasize, const void* value, const size_t degree, ...)
{
    if(tree == NULL)
    {
        return NULL;
    }

        void* NewNode = calloc(1, DIFFERENTIATOR_BASE_NODE_SIZE + (size_t)degree * sizeof(void*) + datasize);
    
    size_t ByteIndex = 0;

    memcpy(NewNode, tree, sizeof(void*));
    ByteIndex += sizeof(void*);

    if(parent ==  NULL)
    {
        memset((char*)NewNode + ByteIndex, 0, sizeof(void*));
    }
    else
    {
        memcpy((char*)NewNode + ByteIndex, parent, sizeof(void*));
    }
    ByteIndex += sizeof(parent);

    memcpy((char*)NewNode + ByteIndex, &type, sizeof(type));
    ByteIndex += sizeof(type);

    memcpy((char*)NewNode + ByteIndex, &datasize, sizeof(datasize));
    ByteIndex += sizeof(datasize);

    if(value == NULL)
    {
        memset((char*)NewNode + ByteIndex, 0, datasize);
    }
    else
    {
        memcpy((char*)NewNode + ByteIndex, value, datasize);
    }
    ByteIndex += datasize;

    memcpy((char*)NewNode + ByteIndex, &degree, sizeof(degree));
    ByteIndex += sizeof(degree);

    va_list descendants;
    va_start(descendants, degree);
    for(size_t i = 0; i < degree; i++)
    {
        void* d = va_arg(descendants, void*);
        if(d == NULL)
        {
            memset((char*)NewNode + ByteIndex, 0, sizeof(void*));
            ByteIndex += sizeof(void*);
        }
        else
        {
            memcpy((char*)NewNode + ByteIndex, &d, sizeof(void*));
            ByteIndex += sizeof(void*);
        }
    }

    return NewNode;
}

void* GetNodeData(void* node, const int FieldCode, const size_t DescendantNumber)
{
    if(node == NULL)
    {
        return NULL;
    }

    if(FieldCode == DESCENDANTS_FIELD_CODE)
    {
        return (char*)node + CalculateByteShift(node, FieldCode) + sizeof(void*) * DescendantNumber;
    }
    return (char*)node + CalculateByteShift(node, FieldCode);
}


int NodeDump(void* node, FILE* out)
{
    if(node == NULL)
    {
        return NULL_POINTER_TREE;
    }
    if(out == NULL)
    {
        return NULL_POINTER_DIFF;
    }
    char* parent = {};

    memcpy(&parent, (const char*)node + CalculateByteShift(node, PARENT_FIELD_CODE), sizeof(void*));
    fprintf(out, "\"label\" = \"{<adr> Node Address =  %p| <type> Type = %zu|<value> Value = ",  node
                                                                                              ,*(const size_t*)((const char*)node + CalculateByteShift(node, TYPE_FIELD_CODE)));
    switch(*(const int*)GetNodeData(node, TYPE_FIELD_CODE, 0))
    {
        case CONST_VALUE_TYPE_CODE:
        {
            double value = 0;
            memcpy(&value, GetNodeData(node, DATA_FIELD_CODE, 0), sizeof(value));
            fprintf(out, "%lf", value);
            fprintf(out, "}\"\ncolor=\"black\"\nfillcolor=\"green\"\n");
            break;
        }
        case OPERATION_TYPE_CODE:
        {
            int opcode = 0;
            memcpy(&opcode, GetNodeData(node, DATA_FIELD_CODE, 0), sizeof(opcode));
            fprintf(out, "%c", operations[opcode].symbol);
            size_t degree = 0; 
            memcpy(&degree, GetNodeData(node, DEGREE_FIELD_CODE, 0), sizeof(degree));
            if(degree != 0)
            {
                fprintf(out, "|{");
            }
            for(size_t i = 0; i < degree; i++)
            {
                char* addr = {};
                memcpy(&addr, GetNodeData(node, DESCENDANTS_FIELD_CODE, i), sizeof(void*));
                if(i == degree - 1)
                {
                    fprintf(out, "<d%zu>%p}", i, addr);
                }
                else
                {
                    fprintf(out, "<d%zu>%p|", i, addr);
                }
            }
            fprintf(out, "}\"\ncolor=\"black\"\nfillcolor=\"red\"\n");
            break;
        }
        case VARIABLE_TYPE_CODE:
        {
            int value = 0;
            memcpy(&value, GetNodeData(node, DATA_FIELD_CODE, 0), sizeof(value));
            fprintf(out, "%c", value);
            fprintf(out, "}\"\ncolor=\"black\"\nfillcolor=\"grey\"\n");
            break;
        }
        case FUNCTION_TYPE_CODE:
        {
            int value = 0;
            memcpy(&value, GetNodeData(node, DATA_FIELD_CODE, 0), sizeof(value));
            fprintf(out, "%s", functions[value].name);
                    size_t degree = 0; 
            memcpy(&degree, GetNodeData(node, DEGREE_FIELD_CODE, 0), sizeof(degree));
            if(degree != 0)
            {
                fprintf(out, "|{");
            }
            for(size_t i = 0; i < degree; i++)
            {
                char* addr = {};
                memcpy(&addr, GetNodeData(node, DESCENDANTS_FIELD_CODE, i), sizeof(void*));
                if(i != degree - 1)
                {
                    fprintf(out, "<d%zu>%p |", i, addr);
                }
                else
                {
                    fprintf(out, "<d%zu>%p}", i, addr);
                }
            }
            fprintf(out, "}\"\ncolor=\"black\"\nfillcolor=\"blue\"\n");
            break;
        }
        default:
        {
            return SYNTAX_ERROR_DIFF;
        }
    }

    fprintf(out, "shape = \"record\"];");

    return 0;
}

void* CloneTreeDiff(const void* tree, void* root)
{
    if(root == NULL)
    {
        return NULL;
    }
    return CloneNodeDiff(tree, root);
}

static void* CloneNodeDiff(const void* tree, void* node)
{
    size_t degree = 0;
    memcpy(&degree, GetNodeData(node, DEGREE_FIELD_CODE, 0), sizeof(degree));
    size_t type = 0;
    memcpy(&type, GetNodeData(node, TYPE_FIELD_CODE, 0), sizeof(type));
    size_t dataSize = 0;
    memcpy(&dataSize, GetNodeData(node, DATASIZE_FIELD_CODE, 0), sizeof(dataSize));

    void* clonedNode = 0;
    if(type == CONST_VALUE_TYPE_CODE)
    {
        double value = 0;
        memcpy(&value, GetNodeData(node, DATA_FIELD_CODE, 0), sizeof(value));
        clonedNode = NewNodeDiff(tree, NULL, type, dataSize, &value, degree);
    }
    else
    {
        int varc = 0;
        memcpy(&varc, GetNodeData(node, DATA_FIELD_CODE, 0), sizeof(varc));
        if(type == VARIABLE_TYPE_CODE)
        {
            clonedNode = NewNodeDiff(tree, NULL, type, dataSize, &varc, degree);
        }
        else if(type == OPERATION_TYPE_CODE)
        {
            clonedNode = NewNodeDiff(tree, NULL, type, dataSize, &varc, degree, NULL, NULL);
        }
        else
        {
            clonedNode = NewNodeDiff(tree, NULL, type, dataSize, &varc, degree, NULL);
        }
    }

    for(size_t i = 0; i < degree; i++)
    {
        char* DescAddr = {};
        memcpy(&DescAddr, GetNodeData(node, DESCENDANTS_FIELD_CODE, i), sizeof(DescAddr));
        if(DescAddr == NULL)
        {
            return NULL;
        }
        MEOW("%p\n", DescAddr);
        char* CopiedDAddr = (char*)CloneNodeDiff(tree, DescAddr);
        memcpy(GetNodeData(clonedNode, DESCENDANTS_FIELD_CODE, i), &CopiedDAddr, sizeof(void*));
    }
    return clonedNode;
}

int FindFunction(const char* funcName)
{
    for(int i = 0; i < NUMBER_OF_FUNCTIONS; i++)    
    {
        if(strncmp(funcName, functions[i].name, sizeof(functions[i].name)) == 0)
        {
            MEOW("Found function: %s, code = %d\n", funcName, i);
            return functions[i].code;
        }
    }
    return -1;
}

int FindOperation(const int symbol)
{
    for(int i = 0; i < NUMBER_OF_OPERATIONS; i++)    
    {
        if(symbol == operations[i].symbol)
        {
            return operations[i].code;
        }
    }
    return -1;
}