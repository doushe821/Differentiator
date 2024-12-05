#include <string.h>
#include "ExpressionReader.h"

int GetOpCode(int code)
{
    for(size_t i = 0; i < NUMBER_OF_OPERATIONS; i++)
    {
        if(code == operations[i].name)
        {
            return operations[i].code;
        }
    }
    return OPERATION_NOT_FOUND;
}

size_t GetFileSize(FILE* fp) //TODO separate file managing lib
{
    
    fseek(fp, 0L, SEEK_END);
    
    size_t size = (size_t)ftell(fp);
    
    rewind(fp);
    return size;
}

void EmergentDtor(FILE* fp, List_t* NodeList, char* ExpressionString)
{
    if(fp != NULL)
    {
        fclose(fp);
    }
    if(NodeList != NULL)
    {
        ListDstr(NodeList);
    }
    if(ExpressionString != NULL)
    {
        free(ExpressionString);
    }
}

char* FReadBuffer(FILE* fp)
{
    if(fp == NULL)
    {
        return NULL;
    }

    size_t bsize = GetFileSize(fp);
    char* buffer = (char*)calloc(bsize + 1, sizeof(char));
    if(buffer == NULL)
    {
        return NULL;
    }
    buffer[bsize] = '\0';
    if(fread(buffer, 1, bsize, fp) != bsize)
    {
        return NULL;
    }

    return buffer;
}

void DumpNode(const void* a, FILE* fp, size_t type)
{
    switch(type)
    {
        case CONST_VALUE_TYPE_CODE:
        {
            double value = 0;
            memcpy(&value, a, sizeof(value));
            fprintf(fp, "%lf", value);
            return;
        }
        case OPERATION_TYPE_CODE:
        {
            int op = 0;
            memcpy(&op, a, sizeof(op));
            fprintf(fp, "%c", operations[op - 1].name);
            return;
        }
        case VARIABLE_VALUE_TYPE_CODE:
        {
            int var = 0;
            memcpy(&var, a, sizeof(var));
            fprintf(fp, "%c", var);
            return;
        }
        case FUNCTION_TYPE_CODE:
        {
            int func = 0;
            memcpy(&func, a, sizeof(func));
            fprintf(fp, "%s", functions[func - 1].name);
            return;
        }
        default:
        {
            return;
        }
    }
}