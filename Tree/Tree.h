#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDEED

#ifndef NDEBUG
#include "../DBTools/DBTools.h"
#endif

#include "NodeStruct.h"

int  PrintTree      (Node_t root, FILE* dest, void(DumpFunc(void* a, FILE* fp)), const char* filename);
int  GetNodeMemShift(Node_t node, int fieldCode);
int  NodeDtor       (Node_t node);
int  BurnTree       (Node_t root);

void NodePrint     (Node_t node, void DumpFunc(void* a, FILE* fp), FILE* fp, size_t rank, size_t* ncounter);

Node_t CreateNode(size_t type, Node_t parent, size_t datasize, void* data, size_t fertility, ...);

Node_t GetKidNode(Node_t node, unsigned long num);
void* GetNodeData(Node_t node);
int AddParent(Node_t node, Node_t parent);

#endif