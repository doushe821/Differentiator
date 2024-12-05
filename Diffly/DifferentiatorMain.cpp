#include "DifferentiatorTree.h"

#include <assert.h>

int PrintTreeTex(tree_t* tree, FILE* out);

int main(int argc, char** argv)
{
    flags_t flags = {};
    ErrorParser(parsecmd(argc, argv, &flags), __LINE__);

    FILE* inputExpression = fopen(argv[1], "a+b");
    if(inputExpression == NULL)
    {
        ErrorParser(FILE_OPENING_ERROR_DIFF, __LINE__);
    }

    char* Expression = FileToString(inputExpression);
    if(Expression == NULL)
    {
        ErrorParser(FILE_READING_ERROR_DIFF, __LINE__);
    }
    fclose(inputExpression);

    tree_t tree = 
    {
        NewNodeDiff,
        CloneTreeDiff,
        NodeDump,

        NULL,
    };

    ErrorParser(ReadCrocodile(Expression, &tree), __LINE__);

    FILE* outDot = NULL;
    if(flags.isOutFilename)
    {
        outDot = fopen(flags.OutFileName, "w+b");
        if(outDot == NULL)
        {
            ErrorParser(FILE_OPENING_ERROR_DIFF, __LINE__);
        }
        ErrorParser(PrintTreeGraphViz(outDot, tree, flags.OutFileName), __LINE__);
    }
    else
    {
        outDot = fopen(DEFAULT_OUTPUT_DOT_FILE_NAME, "w+b");
        if(outDot == NULL)
        {
            ErrorParser(FILE_OPENING_ERROR_DIFF, __LINE__);
        } 
        ErrorParser(PrintTreeGraphViz(outDot, tree, DEFAULT_OUTPUT_DOT_FILE_NAME), __LINE__);
    }   

    free(Expression);
    //BurnTree(&tree);
    //return 0;


    tree_t diffTree = 
    {
        NewNodeDiff,
        CloneTreeDiff,
        NodeDump,
        NULL,           
    };

    if((diffTree.root = Differentiate(&tree, tree.root)) == NULL)
    {
        ErrorParser(NULL_POINTER_DIFF, __LINE__);
    }

    const char* SecondDumpFName = "Dump/DiffedTree.dot";
    FILE* SecondDump = fopen(SecondDumpFName, "w+b");
    ErrorParser(PrintTreeGraphViz(SecondDump, diffTree, DEFAULT_OUTPUT_DOT_FILE_NAME), __LINE__);


    FILE* outTex = fopen("deriv.tex", "w+b");
    assert(outTex);
    //ErrorParser(PrintTreeTex(&tree, outTex), __LINE__);
    fclose(outTex);

    BurnTree(&diffTree);
    BurnTree(&tree);
    return 0;
}

int PrintTreeTex(tree_t* tree, FILE* out)
{
    if(tree == NULL || out == NULL)
    {
        return NULL_POINTER_DIFF;
    }

    return -1;
}
