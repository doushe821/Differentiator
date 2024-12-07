#include "Tokenisation.h"
#include "ctype.h"

Token_t GetConstToken(const char* Buffer, size_t* index, __attribute((unused)) VariableTable_t* VarTable);
Token_t GetFunctionToken(const char* Buffer, size_t* index, __attribute((unused)) VariableTable_t* VarTable);
Token_t GetVariableToken(const char* Buffer, size_t* index, __attribute((unused)) VariableTable_t* VarTable);
Token_t GetOperationToken(const char* Buffer, size_t* index, __attribute((unused)) VariableTable_t* VarTable);

__attribute__((noreturn)) void SyntaxErrorTok(size_t* Index, char Literal, const char* func, const char* file, const int line);


// DEBUG

void PrintTokens(Token_t* tokens, size_t number, VariableTable_t* VarTable)
{
    MEOW("Number of tokens = %zu\n", number);
    for(size_t i = 0; i < number; i++)
    {
        if(tokens[i].type == CONST_TOKEN)
        {
            fprintf(stderr, "Node№ %zu:\nType = %d\nvalue = %lg\n\n", i, tokens[i].type, tokens[i].Data.Float);
        }
        else if(tokens[i].type == VARIABLE_TOKEN)
        {
            fprintf(stderr, "Node№ %zu:\nType = %d\nVarName = %s\n\n", i, tokens[i].type, VarTable->Vars[tokens[i].Data.Decimal].name);
        }
        else if(tokens[i].type == FUNCTION_TOKEN)
        {
            fprintf(stderr, "Node№ %zu:\nType = %d\nFuncName = %s\n\n", i, tokens[i].type, functions[tokens[i].Data.Decimal].name);
        }
        else
        {
            fprintf(stderr, "Node№ %zu:\nType = %d\ncode = %d\ncode(symbol) = %c\n\n", i, tokens[i].type, tokens[i].Data.Decimal, tokens[i].Data.Decimal);    
        }
    }
}

int PrintToken(const Token_t token, const VariableTable_t* VarTable)
{
    if(token.type == CONST_TOKEN)
    {
        fprintf(stderr, "Type = %d\nvalue = %lg\n\n",token.type, token.Data.Float);
    }
    else if(token.type == VARIABLE_TOKEN)
    {
        fprintf(stderr, "Type = %d\nVarIndex = %s\n\n", token.type, VarTable->Vars[token.Data.Decimal].name);
    }
    else if(token.type == FUNCTION_TOKEN)
    {
        fprintf(stderr, "Type = %d\nFuncName = %s\n\n", token.type, functions[token.Data.Decimal].name);
    }
    else
    {
        fprintf(stderr, "Type = %d\ncode = %d\ncode(symbol) = %c\n\n", token.type, token.Data.Decimal, token.Data.Decimal);    
    }
    return 0;
}

// DEBUG


Token_t* Tokenise(const char* Buffer, size_t length, __attribute((unused)) VariableTable_t* VarTable, size_t* TCounter)
{
    Token_t* Tokens = (Token_t*)calloc(length, sizeof(Token_t));
    size_t TokenIndex = 0;
    for(size_t i = 0; i < length;)
    {
        if(isspace(Buffer[i]))
        {
            MEOW("Space (rubbish) on %zu\n", i);
            continue;
        }

        if(isdigit(Buffer[i]))
        {
            MEOW("Constant on %zu\n", i);
            Tokens[TokenIndex++] = GetConstToken(Buffer, &i, VarTable);
            (*TCounter)++;
            continue;
        }

        if(Buffer[i] == '(')
        {
            MEOW("Opening brake on %zu\n", i);
            Tokens[TokenIndex++] = {BRAKE_TOKEN, '('};
            (*TCounter)++;
            i++;
            continue;
        }

        if(Buffer[i] == ')')
        {
            MEOW("Closing brake on %zu\n", i);
            Tokens[TokenIndex++] = {BRAKE_TOKEN, ')'};
            (*TCounter)++;
            i++;
            continue;
        }

        if(isalpha(Buffer[i]))
        {
            MEOW("Function or variable on %zu\n", i);
            Tokens[TokenIndex++] = GetFunctionToken(Buffer, &i, VarTable);
            (*TCounter)++;
            continue;
        }
        
        MEOW("Operation brake on %zu\n", i);
        Tokens[TokenIndex++] = GetOperationToken(Buffer, &i, VarTable);
        (*TCounter)++;
    }
    return Tokens;
}

Token_t GetOperationToken(const char* Buffer, size_t* index, __attribute((unused)) VariableTable_t* VarTable)
{
    Token_t tok = {OPERATION_TOKEN, Buffer[*index]};
    (*index)++;
    return tok;
}

Token_t GetFunctionToken(const char* Buffer, size_t* index, __attribute((unused)) VariableTable_t* VarTable)
{
    Token_t tok = {};
    char funcName[FUNCTION_NAME_MAX] = {};
    size_t startIndex = *index;
    while(isalnum(Buffer[*index]))
    {
        funcName[*index - startIndex] = Buffer[*index];
        (*index)++;
    }
    funcName[*index - startIndex] = '\0';
    int fCode = 0;
    if((fCode = FindFunction(funcName)) != -1)
    {
        tok.type = FUNCTION_TOKEN;
        tok.Data.Decimal = fCode;
        return tok;
    }
    else
    {
        tok = {VARIABLE_TOKEN, VarTable->Index};
        memcpy(VarTable->Vars[VarTable->Index].name, funcName, strlen(funcName) + 1);
        VarTable->Index++;
        return tok;
    }
    SyntaxErrorTok(index, Buffer[*index], __func__, __FILE__, __LINE__);
}


Token_t GetConstToken(const char* Buffer, size_t* index, __attribute((unused)) VariableTable_t* VarTable)
{
    Token_t tok = {CONST_VALUE_TYPE_CODE, 0};
    size_t DigitsCounter = 0;
    while(('0' <= Buffer[*index] && '9' >= Buffer[*index]) || '.' == Buffer[*index])
    {
        MEOW("iter %s%d\n", __FILE__, __LINE__);
        if(DigitsCounter > 0)
        {
            DigitsCounter++;
        }
        if(Buffer[*index] == '.')
        {
            DigitsCounter++;
            (*index)++;
            continue;
        }
        tok.Data.Float = tok.Data.Float*10 + Buffer[*index] - '0';
        (*index)++;
    }
    if(DigitsCounter > 0)
    {
        if(DigitsCounter == 1)
        {
            SyntaxErrorTok(index, Buffer[*index], __func__, __FILE__, __LINE__);
        }
        for(size_t i = 0; i < DigitsCounter - 1; i++)
        {
            tok.Data.Float /= 10;
        }
    }
    return tok;
}

__attribute__((noreturn)) void SyntaxErrorTok(size_t* Index, char Literal, const char* func, const char* file, const int line)
{
    fprintf(stderr, "Syntax error on p = %zu, symbol is %c, in function %s in %s:%d\n", *Index, Literal, func, file, line);
    abort();
}