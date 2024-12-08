#ifndef TAYLOR_H_INCLUDED
#define TAYLOR_H_INCLUDED
#include "Differentiator.h"
double CalculateNode(void* node, VariableTable_t* VarTable);
double CalculateTree(void* root, VariableTable_t* VarTable);
int Taylor(tree_t* tree, VariableTable_t* VarTable, FILE* outTex, size_t degree);

#endif