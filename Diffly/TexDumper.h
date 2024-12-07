#ifndef TEX_DUMPER_H_INCLUDED
#define TEX_DUMPER_H_INCLUDED

#include "VariableTable.h"

int DumpTreeTex(void* root, FILE* out, VariableTable_t* VarTable);
int DumpNodeTex(void* node, FILE* out, VariableTable_t* VarTable);
int DumpVarTex(void* node, FILE* out, VariableTable_t* VarTable);
int DumpConstTex(void* node, FILE* out, VariableTable_t* VarTable);
int DumpOperationTex(void* node, FILE* out, VariableTable_t* VarTable);
int DumpFunctionTex(void* node, FILE* out, VariableTable_t* VarTable);



#endif