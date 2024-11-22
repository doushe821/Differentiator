#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include <assert.h>

#include "Tree.h"
#include "NodeStruct.h"

enum ErrCodes
{
    NO_ERRORS = 0,
    UNKNOWN_ERROR = -1,
    ALLOCATION_FAILURE = -2,
    NODE_NULL_POINTER = -3,
    ROOT_NULL_POINTER = -4,
    NODE_EXISTS = -5,
    NODE_CREATION_FAILURE = -6,
    FILE_CREATION_ERROR = -7,
    FILE_NULL_PTR = -8,
    FIELD_CODE_NOT_FOUND = -9,
};

Node_t CreateNode(size_t type, Node_t parent, size_t datasize, void* data, size_t fertility, ...)
{
    const u_int64_t ZERO_ADDRESS = 0;

    size_t nodesize = sizeof(type) + sizeof(parent) + sizeof(fertility) + sizeof(void*) * fertility + sizeof(datasize) + datasize;
    
    MEOW("Nodesize = %zu\n", nodesize);

    void* node = {};
    node = calloc(1, nodesize);

    if(node == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for new node\n");
        return NULL;
    }

    size_t memIndex = 0;
    
    MEOW("Node = %p\n", node);

    memcpy((char*)node, &type, sizeof(type));
    memIndex += sizeof(type);

    MEOW("node: type = %d\n", *((int*)node));
    MEOW("MemIndex = %zu\n", memIndex);

    if(parent == NULL)
    {
        MEOW("node + memindex = %p\n", (char*)node + memIndex);
        memcpy((char*)node + memIndex, &ZERO_ADDRESS, sizeof(u_int64_t));
    }
    else
    {
        memcpy((char*)node + memIndex, parent, sizeof(void*));
    }
    memIndex += sizeof(parent);

    MEOW("MemIndex = %zu\n", memIndex);

    memcpy((char*)node + memIndex, &fertility, sizeof(fertility));
    memIndex += sizeof(fertility);

    MEOW("MemIndex = %zu\n", memIndex);

    va_list kidPTRs;
    va_start(kidPTRs, fertility);
    for(size_t i = 0; i < fertility; i++)
    {
        void* ptr = va_arg(kidPTRs, void*);
        if(ptr == NULL)
        {
            memcpy((char*)node + memIndex, &ZERO_ADDRESS, sizeof(void*));
        }
        else
        {
            MEOW("\033[31mPTR is : %p\n\033[0m", ptr);
            MEOW("MEMINDEX = %zu\n", memIndex);
            memcpy((char*)node + memIndex, &ptr, sizeof(Node_t));
        }
        memIndex += sizeof(Node_t);
    }

    MEOW("MemIndex = %zu\n", memIndex);

    memcpy((char*)node + memIndex, &datasize, sizeof(datasize));
    memIndex += sizeof(datasize);

    MEOW("MemIndex = %zu\n", memIndex);

    memcpy((char*)node + memIndex, data, datasize);

    MEOW("\033[33mReturning: %p\n\033[0m", node);
    return node;
}

int NodeDtor(Node_t node)
{
    if(node == NULL)
    {
        fprintf(stderr, "Tried to destruct uninitialised or already destructed node\n");
        return NODE_NULL_POINTER;
    }

    free(node);

    return NO_ERRORS;
}

int BurnTree(Node_t root)
{
    if(root == NULL)
    {
        fprintf(stderr, "Tried to destruct uninitialised or already destructed node\n");
        return NODE_NULL_POINTER;
    }

    for(size_t i = 0; i < *(size_t*)((char*)root + GetNodeMemShift(root, FERTILITY_FIELD_CODE)); i++)
    {
        if(GetKidNode(root, i) != NULL)
        {
            NodeDtor(GetKidNode(root, i));
        }
    }

    free(root);
    return NO_ERRORS;
}

//  полиморфизм плюсы

int PrintTree(Node_t root, FILE* dest, void(DumpFunc(void* a, FILE* fp)), const char* filename)
{
    if(dest == NULL)
    {
        fprintf(stderr, "Can't write in file pointed by NULL pointer\n");
        return FILE_NULL_PTR;
    }

    fprintf(dest, 
    "digraph g {\n"
    "fontname=\"Helvetica,Arial,sans-serif\""
    "node [fontname=\"Helvetica,Arial,sans-serif\", color = \"green\"]\n"
    "graph [\n"
    "rankdir = \"TB\"\n"
    "];\n"
    "node [\n"
    "fontisze = \"16\"\n"
    "shape = \"ellipse\"\n"
    "];\n"                );

    size_t ncounter = 0;
    NodePrint(root, DumpFunc, dest, 0, &ncounter);
    fprintf(dest, "}");
    char sysCmd[FILENAME_MAX] = {};
    sprintf(sysCmd, "dot -Tpdf Dumps/%s -o Dumps/dump.dpf\n", filename);
    system(sysCmd);
    return NO_ERRORS;
}
 
Node_t GetKidNode(Node_t node, unsigned long num)
{
    MEOW(" MEJE SHIF = %zu\n", GetNodeMemShift(node, KIDS_FIELD_CODE));
    void* tptr = (char*)node + ((unsigned long)GetNodeMemShift(node, KIDS_FIELD_CODE) + sizeof(Node_t) * num);
    MEOW("\n\n\033[32m%p\n\n\n\033[0m", (void*)(*(u_int64_t*)tptr));

    return (*(Node_t*)((char*)node + GetNodeMemShift(node, KIDS_FIELD_CODE) + sizeof(Node_t) * num));
}

void* GetNodeData(Node_t node)
{
    return (char*)node + GetNodeMemShift(node, DATA_FIELD_CODE);
}

int AddParent(Node_t node, Node_t parent)
{
    memcpy((char*)node + GetNodeMemShift(node, PARENT_FIELD_CODE), &parent, sizeof(Node_t));
    return NO_ERRORS;
}

void  NodePrint(Node_t node, void DumpFunc(void* a, FILE* fp), FILE* fp, size_t rank, size_t* ncounter)
{
    rank++;
    (*ncounter)++;
    size_t currentNode = *ncounter;
    if(node != NULL)
    {
        fprintf(fp, "\"node%zu\" [\n", *ncounter);
        fprintf(fp, "rank = %zu\n", rank);
        fprintf(fp, "\"label\" = \"{<parent> parent = %p|<adr> node address =  %p|<f0> DATA = ", *(Node_t*)((char*)node + GetNodeMemShift(node, PARENT_FIELD_CODE)), node);

        DumpFunc(((char*)node + GetNodeMemShift(node, DATA_FIELD_CODE)), fp);
        fprintf(fp, "|{");
        for(size_t i = 0; i < *(size_t*)((char*)node + GetNodeMemShift(node, FERTILITY_FIELD_CODE)); i++)
        {
            fprintf(fp, "<f%zu> Kid№ %zu = %p ", i + 2, i, GetKidNode(node, i));
            if(i != *(size_t*)((char*)node + GetNodeMemShift(node, FERTILITY_FIELD_CODE)) - 1)
            {
                fprintf(fp, "|");
            }

        }

        fprintf(fp, "}}\"\n");
        fprintf(fp, "shape = \"record\"\n");
        fprintf(fp, "];\n");

        for(size_t i = 0; i < *(size_t*)((char*)node + GetNodeMemShift(node, FERTILITY_FIELD_CODE)); i++)
        {
            if(GetKidNode(node, i) != NULL)
            {
                fprintf(fp, "node%zu: <f%zu> -> node%zu[color = \"brown\"];\n", currentNode, i + 2, *ncounter + 1);
                NodePrint(GetKidNode(node, i), DumpFunc, fp, rank, ncounter);
            }
        }    
    }
    else
    {
        fprintf(stderr, "Tried printing NULL-pointed node\n");
        return;
    }
}

int GetNodeMemShift(Node_t node, int fieldCode)
{
    BaseNode bn = {};
    MEOW("node adress is %p\n", node);
    MEOW("field code = %d\n", fieldCode);
    switch(fieldCode)
    {
        case TYPE_FIELD_CODE:
        {
            return TYPE_FIELD_CODE;
        }
        case PARENT_FIELD_CODE:
        {
            return (int)sizeof(bn.type);
        }
        case FERTILITY_FIELD_CODE:
        {
            return GetNodeMemShift(node, PARENT_FIELD_CODE) + (int)sizeof(bn.parent);
        }
        case KIDS_FIELD_CODE:
        {
            return GetNodeMemShift(node, FERTILITY_FIELD_CODE) + (int)sizeof(bn.fertility);
        }
        case DATASIZE_FIELD_CODE:
        {
            MEOW("Number of kids: %zu\n", *(size_t*)((char*)node + GetNodeMemShift(node, FERTILITY_FIELD_CODE)));
            return GetNodeMemShift(node, KIDS_FIELD_CODE) + (int)(sizeof(bn.kids) * (*(size_t*)((char*)node + GetNodeMemShift(node, FERTILITY_FIELD_CODE))));
        }
        case DATA_FIELD_CODE:
        {
            return GetNodeMemShift(node, DATASIZE_FIELD_CODE) + (int)sizeof(bn.datasize);
        }
        default:
        {
            return FIELD_CODE_NOT_FOUND;
        }
    }

    return UNKNOWN_ERROR;
}