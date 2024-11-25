#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDEED

#ifndef NDEBUG
#include "../DBTools/DBTools.h"
#endif

#include "NodeStruct.h"

const u_int64_t ZERO_ADDRESS = 0;

int  PrintTree      (Node_t root, FILE* dest, void(DumpFunc(const void* a, FILE* fp, size_t type)), const char* filename);
int  GetNodeMemShift(Node_t node, int fieldCode);
int  NodeDtor       (Node_t node);
int  BurnTree       (Node_t root);

void  NodePrint(Node_t node, FILE* fp, size_t rank, size_t* ncounter, void DumpFunc(const void* a, FILE* fp, size_t type));

Node_t CreateNode(size_t type, Node_t parent, size_t datasize, void* data, size_t fertility, ...);
int AddNode(Node_t node, Node_t parent, size_t KidNum);

int AddKid(Node_t parent, Node_t kid, size_t kidID);
Node_t GetKidNode(Node_t node, unsigned long num);
void* GetNodeData(Node_t node);
int AddParent(Node_t node, Node_t parent);
int AddData(Node_t node, void* data);

#endif