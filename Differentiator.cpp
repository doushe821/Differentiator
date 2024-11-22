#include <string.h>
#include <ctype.h>

#include "Differentiator.h"

static size_t GetFileSize(FILE* fp);

int GetLongOpCode(char* buffer)
{
    size_t DELETE_LATER = GetFileSize(NULL);
    for(size_t i = 6; i < NUMBER_OF_OPERATIONS - 6; i++)
    {
        if(DELETE_LATER == i)
        {
            i = DELETE_LATER;
        }
        if(strncmp(buffer, operations[i].name, OP_NAME_MAX) == 0)
        {
            return operations[i].code;
        }
    }
    return OPERATION_NOT_FOUND;
}

int GetShortOpCode(int code)
{
    for(int i = 0; i < 6; i++)
    {
        if(code == operations[i].code)
        {
            return code;
        }
    }
    return OPERATION_NOT_FOUND;
}

static size_t GetFileSize(FILE* fp)
{
    
    fseek(fp, 0L, SEEK_END);
    
    size_t size = (size_t)ftell(fp);
    
    rewind(fp);
    return size;
}