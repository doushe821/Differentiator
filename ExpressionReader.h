#ifndef EXPRESSION_READER_H_INCLUDED
#define EXPRESSION_READER_H_INCLUDED

#include <stdlib.h>

#include "DBTools/DBTools.h"
#include "DifferentiatorStructures.h"

const size_t DIFF_NODE_SIZE = BASE_NODE_SIZE + 16 + 8;

int GetFuncCode(const char* buffer);

size_t GetFileSize(FILE* fp);

void EmergentDtor(FILE* fp, List_t* NodeList, char* ExpressionString);

int GetOpCode(int code);

char* FReadBuffer(FILE* fp);

void DumpNode(const void* a, FILE* fp, size_t type);

#endif