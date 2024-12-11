#ifndef DIFFERENTIATOR_TREE_H_INCLUDED
#define DIFFERENTIATOR_TREE_H_INCLUDED

#include <string.h>

#include "../Libs/Tree/Tree.h"

const size_t DIFFERENTIATOR_BASE_NODE_SIZE = 40;

void* NewNodeDiff(const void* tree, const void* parent, const size_t type, const size_t datasize, const void* value, const size_t degree, ...);

int NodeDump(void* node, FILE* out);

void* CloneTreeDiff(const void* tree, void* root);

int FindFunction(const char* funcName); // Кнут, Седжвик, Дэниал Гриач "Конструирование компиляторов", Хант "Компиляторы", Ахо Ульман "Два тома какиъ-то книжек"

#endif