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
    OUT_OF_KIDS = -10,
};

Node_t CreateNode(size_t type, Node_t parent, size_t datasize, void* data, size_t fertility, ...)
{

    size_t nodesize = sizeof(type) + sizeof(parent) + sizeof(fertility) + sizeof(void*) * fertility + sizeof(datasize) + datasize;

    void* node = {};
    node = calloc(1, nodesize);

    if(node == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for new node\n");
        return NULL;
    }

    size_t memIndex = 0;

    memcpy((char*)node, &type, sizeof(type));
    memIndex += sizeof(type);

    if(parent == NULL)
    {
        memcpy((char*)node + memIndex, &ZERO_ADDRESS, sizeof(u_int64_t));
    }
    else
    {
        memcpy((char*)node + memIndex, parent, sizeof(void*));
    }
    memIndex += sizeof(parent);


    memcpy((char*)node + memIndex, &fertility, sizeof(fertility));
    memIndex += sizeof(fertility);

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
            memcpy((char*)node + memIndex, &ptr, sizeof(Node_t));
        }
        memIndex += sizeof(Node_t);
    }

    memcpy((char*)node + memIndex, &datasize, sizeof(datasize));
    memIndex += sizeof(datasize);

    if(data == NULL)
    {
        memcpy((char*)node + memIndex, &ZERO_ADDRESS, datasize);
    }
    else
    {
        memcpy((char*)node + memIndex, data, datasize);
    }

    return node;
}

int AddNode(Node_t node, Node_t parent, size_t KidNum)
{
    if(node == NULL)
    {
        fprintf(stderr, "Node pointer is NULL\n");
        return NODE_NULL_POINTER;
    }
    if(parent == NULL)
    {
        fprintf(stderr, "Parent pointer is NULL\n");
        return NODE_NULL_POINTER;
    }

    if(KidNum > *(size_t*)((char*)parent + GetNodeMemShift(parent, FERTILITY_FIELD_CODE)))
    {
        fprintf(stderr, "No room for kid nodes %s:%d\n", __FILE__, __LINE__);
        return OUT_OF_KIDS;
    }
    if(*(Node_t*)((char*)parent + GetNodeMemShift(parent, KIDS_FIELD_CODE) + sizeof(Node_t) * KidNum) != NULL)
    {
        fprintf(stderr, "Node occupied %s:%d\n", __FILE__, __LINE__);
        return NODE_EXISTS;
    }
    MEOW("node = %p, parent = %p\n", node, parent);
    memcpy((char*)parent + GetNodeMemShift(parent, KIDS_FIELD_CODE) + sizeof(Node_t) * KidNum, &node, sizeof(Node_t));
    MEOW("KID: %p\n", *(Node_t*)((char*)parent + GetNodeMemShift(parent, KIDS_FIELD_CODE) + sizeof(Node_t) * KidNum));
    memcpy((char*)node + GetNodeMemShift(node, PARENT_FIELD_CODE), &parent, sizeof(Node_t));
    MEOW("PARENT: %p\n", *(Node_t*)((char*)node + GetNodeMemShift(parent, PARENT_FIELD_CODE)));

    return 0;
}

int AddKid(Node_t parent, Node_t kid, size_t kidID)
{
    if(parent == NULL)
    {
        return NODE_NULL_POINTER;
    }
    if(kid == NULL)
    {
        return NODE_NULL_POINTER;
    }

    MEOW(GREEN_COLOR_ESC_SEQ "(DEBUG) |  Parent ptr = %p;  Kid ptr = %p,  KidID = %zu  (%s:%d)\n" DEFAULT_COLOR_ESC_SEQ, parent, kid, kidID, __FILE__, __LINE__);
    memcpy((char*)parent + GetNodeMemShift(parent, KIDS_FIELD_CODE) + sizeof(Node_t) * kidID, &kid, sizeof(Node_t));
    return 0;
}

int AddData(Node_t node, void* data)
{
    if(node == NULL)
    {
        return NODE_NULL_POINTER;
    }
    if(data == NULL)
    {
        return NODE_NULL_POINTER;
    }

    memcpy((char*)node + GetNodeMemShift(node, DATA_FIELD_CODE), data, *(size_t*)((char*)node + GetNodeMemShift(node, DATASIZE_FIELD_CODE)));
    return 0;
}

int NodeDtor(Node_t node)
{
    if(node == NULL)
    {
        fprintf(stderr, "Tried to destruct uninitialised or already destructed node\n");
        return NODE_NULL_POINTER;
    }

    free(node);

    return 0;
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
    return 0;
}

//  полиморфизм плюсы

int PrintTree(Node_t root, FILE* dest, void(DumpFunc(const void* a, FILE* fp, size_t type)), const char* filename)
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
    NodePrint(root, dest, 0, &ncounter, DumpFunc);
    fprintf(dest, "}");
    char sysCmd[FILENAME_MAX] = {};
    sprintf(sysCmd, "dot -Tpdf Dumps/%s -o Dumps/dump.dpf\n", filename);
    system(sysCmd);
    return 0;
}
 
Node_t GetKidNode(Node_t node, unsigned long num)
{
    return (*(Node_t*)((char*)node + GetNodeMemShift(node, KIDS_FIELD_CODE) + sizeof(Node_t) * num));
}

void* GetNodeData(Node_t node)
{
    return (char*)node + GetNodeMemShift(node, DATA_FIELD_CODE);
}

int AddParent(Node_t node, Node_t parent)
{
    memcpy((char*)node + GetNodeMemShift(node, PARENT_FIELD_CODE), &parent, sizeof(Node_t));
    return 0;
}

void  NodePrint(Node_t node, FILE* fp, size_t rank, size_t* ncounter, void DumpFunc(const void* a, FILE* fp, size_t type))
{

    MEOW("PRINTING NODE ON ADDRESS: %p\n", node);
    MEOW("ITS DATA: %lf\n", *((double*)GetNodeData(node)));
    rank++;
    (*ncounter)++;
    size_t currentNode = *ncounter;
    if(node != NULL)
    {
        fprintf(fp, "\"node%zu\" [\n", *ncounter);
        fprintf(fp, "rank = %zu\n", rank);

        fprintf(fp, "\"label\" = \"{<parent> Parent = %p|<adr> Node Address =  %p| <type> Type = %zu|<f0> Data = ", *(Node_t*)((char*)node + GetNodeMemShift(node, PARENT_FIELD_CODE))
                                                                                                                  , node, *(size_t*)node);
        DumpFunc(((char*)node + GetNodeMemShift(node, DATA_FIELD_CODE)), fp, *(size_t*)node);



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
                NodePrint(GetKidNode(node, i), fp, rank, ncounter, DumpFunc);
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