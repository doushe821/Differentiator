#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Differentiator.h" 

const size_t INITIAL_LIST_SIZE = 32;

int main()
{

    FILE* fp = fopen("tests/test1.mxp", "r+b");
    if(fp == NULL)
    {
        fprintf(stderr, "Unable to open file %s:%d\n", __FILE__, __LINE__);
    }

    List_t* NodeList = NULL;
    if(ListInit(&NodeList, INITIAL_LIST_SIZE, DIFF_NODE_SIZE) != 0)
    {
        fprintf(stderr, "Failed to initialize list.\n");
        EmergentDtor(fp, NULL, NULL);
        return LIST_INITIALIZATION_FAILURE;
    }

    char* ExpressionString = FReadBuffer(fp);
    if(ExpressionString == NULL)
    {
        fprintf(stderr, "Failed to read from file %s:%d\n", __FILE__, __LINE__);
        EmergentDtor(fp, NodeList, NULL);
        return READING_ERROR;
    }

    MEOW(MAGENTA_COLOR_ESC_SEQ "(DEBUG) | %s\n" DEFAULT_COLOR_ESC_SEQ, ExpressionString);

    size_t StringIndex = 0;

    ON_DEBUG(size_t iteration);

    Node_t root = ReadNode(ExpressionString, &StringIndex, NodeList, NULL ON_DEBUG(COMMA &iteration));
    if(root == NULL)
    {
        EmergentDtor(fp, NodeList, ExpressionString);
        fprintf(stderr, RED_COLOR_ESC_SEQ "Failed to create tree %s:%d\n" DEFAULT_COLOR_ESC_SEQ, __FILE__, __LINE__);
        return READING_ERROR;
    }

    MEOW(YELLOW_COLOR_ESC_SEQ "(DEBUG)  |  All nodes read (%s:%d)\n" DEFAULT_COLOR_ESC_SEQ, __FILE__, __LINE__);
    MEOW("root left = %p, root right = %p, number of nodes = %zu\n", GetKidNode(root, 0), GetKidNode(root, 0), GetFreeInd(NodeList));

    FILE* dmp = fopen("Dumps/dmp.dot", "w+b");
    if(dmp == NULL)
    {
        EmergentDtor(fp, NodeList, ExpressionString);
        fprintf(stderr, "Cant open dump file\n");
        return -1;
    }
    const char* filename = "dmp.dot";


    PrintTree(root, dmp, DumpNode, filename); // error handler
    
    
    /*List_t* NodeListDiffed = NULL;
    if(ListInit(&NodeListDiffed, INITIAL_LIST_SIZE, DIFF_NODE_SIZE) != 0)
    {
        fprintf(stderr, "Failed to initialize list.\n");
        EmergentDtor(fp, NULL, NULL);
        return LIST_INITIALIZATION_FAILURE;
    }*/

    //Differentiate(NodeListDiffed, root);

    fclose(dmp);
    free(ExpressionString);
    ListDstr(NodeList);
    fclose(fp);
    return 0;
}