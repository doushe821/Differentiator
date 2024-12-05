#ifndef DIFFERENTIATOR_TREE_H_INCLUDED
#define DIFFERENTIATOR_TREE_H_INCLUDED

#include <string.h>

#include "Tree/Tree.h"
#include "Differentiator.h"

const size_t DIFFERENTIATOR_BASE_NODE_SIZE = 40;

void* NewNodeDiff(const void* tree, const void* parent, const size_t type, const size_t datasize, const void* value, const size_t degree, ...);

void* GetNodeData(void* node, const int FieldCode, const size_t DescendantNumber);

int NodeDump(void* node, FILE* out);

void* CloneTreeDiff(const void* tree, void* root);

int FindFunction(const char* funcName);

#endif