#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

#include <stdio.h>
#include <stdarg.h>

#include "TreeErrors.h"


typedef void* (*InitFunc)(const void*, const void*, const size_t, const size_t, const void*, const size_t, ...);
typedef void* (*CloneFunc)(const void*, void*);
typedef int   (*DumpFunc)(void*, FILE*);

enum fieldCodes
{
    PARENT_FIELD_CODE      = 1,
    TYPE_FIELD_CODE        = 2,
    DATASIZE_FIELD_CODE    = 3,
    DATA_FIELD_CODE        = 4,
    DEGREE_FIELD_CODE      = 5,
    DESCENDANTS_FIELD_CODE = 6,
};

struct tree_t
{
    InitFunc   InitNode;
    CloneFunc CloneTree;
    DumpFunc   DumpNode;

    void* root         ;
};

struct BaseNode
{
    void*  root      ;
    void*  parent    ;
    size_t type      ;
    size_t datasize  ;
    void*  value     ;
    size_t degree    ;
    void*  descendant;
};

size_t CalculateByteShift(const void* node, const int fieldcode);
int    PrintTreeGraphViz(FILE* out, const tree_t tree, const char* filename);
int    BurnBranch(void* node);
int    BurnTree(tree_t* tree);

#endif