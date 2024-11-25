#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Differentiator.h"


char* FReadBuffer(FILE* fp);
void DumpNode(const void* a, FILE* fp, size_t type);

const size_t INITIAL_LIST_SIZE = 32;

int main()
{
    FILE* fp = fopen("test1.mxp", "r+b");
    if(fp == NULL)
    {
        fprintf(stderr, "Unable to open file %s:%d\n", __FILE__, __LINE__);
    }


    List_t* NodeList = NULL;
    if(ListInit(&NodeList, INITIAL_LIST_SIZE, DIFF_NODE_SIZE) != 0)
    {
        fprintf(stderr, "Failed to initialize list.\n");
        EmergentDtor(fp, NULL, NULL, NULL, NULL);
        return LIST_INITIALIZATION_FAILURE;
    }

    char* ExpressionString = FReadBuffer(fp);
    if(ExpressionString == NULL)
    {
        fprintf(stderr, "Failed to read from file %s:%d\n", __FILE__, __LINE__);
        EmergentDtor(fp, NodeList, NULL, NULL, NULL);
        return READING_ERROR;
    }

    MEOW(MAGENTA_COLOR_ESC_SEQ "(DEBUG) | %s\n" DEFAULT_COLOR_ESC_SEQ, ExpressionString);

    size_t StringIndex = 0;

    ON_DEBUG(size_t iteration);

    Node_t root = ReadNode(ExpressionString, &StringIndex, NodeList, NULL ON_DEBUG(COMMA &iteration));
    if(root == NULL)
    {
        EmergentDtor(fp, NodeList, ExpressionString, NULL, NULL);
        fprintf(stderr, RED_COLOR_ESC_SEQ "Failed to create tree %s:%d\n" DEFAULT_COLOR_ESC_SEQ, __FILE__, __LINE__);
        return READING_ERROR;
    }

    MEOW(YELLOW_COLOR_ESC_SEQ "(DEBUG)  |  All nodes read (%s:%d)\n" DEFAULT_COLOR_ESC_SEQ, __FILE__, __LINE__);
    MEOW("root left = %p, root right = %p, number of nodes = %zu\n", GetKidNode(root, 0), GetKidNode(root, 0), GetFreeInd(NodeList));

    FILE* dmp = fopen("dmp.dot", "w+b");
    if(dmp == NULL)
    {
        EmergentDtor(fp, NodeList, ExpressionString, root, NULL);
        fprintf(stderr, "Cant open dump file\n");
        return -1;
    }
    const char* filename = "dmp.dot";

    PrintTree(root, dmp, DumpNode, filename);
    
    fclose(dmp);
    free(ExpressionString);
    ListDstr(NodeList);
    fclose(fp);
    return 0;
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