#ifndef TOKENISATION_H_INCLUDED
#define TOKENISATION_H_INCLUDED

#include "Differentiator.h"

union DataUnion
{
    int Decimal;
    double Float;
    char Char;
};
struct Token_t
{
    int type;
    DataUnion Data;
};

enum TOKEN_TYPES
{
    CONST_TOKEN = 0,
    OPERATION_TOKEN = 1,
    VARIABLE_TOKEN = 2,
    FUNCTION_TOKEN = 3,
    BRAKE_TOKEN = 4,
};

Token_t* Tokenise(const char* Buffer, size_t length, __attribute((unused)) VariableTable_t* VarTable, size_t* TCounter);
int PrintToken(const Token_t token, const VariableTable_t* VarTable);

// DEBUG
void PrintTokens(Token_t* tokens, size_t number, VariableTable_t* VarTable);
// DEBUG

#endif
