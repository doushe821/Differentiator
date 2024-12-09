#include "Differentiator.h"
#include "Tokenisation.h"
#include "RecursiveDescensionTokens.h"
#include "Transitions.h"

#include <assert.h>
#include <time.h>

int PrintTreeTex(tree_t* tree, FILE* out);

int main(int argc, char** argv)
{
    flags_t flags = {};
    ErrorParser(parsecmd(argc, argv, &flags), __LINE__, __FILE__);

    FILE* inputExpression = fopen(argv[1], "a+b");
    if(inputExpression == NULL)
    {
        ErrorParser(FILE_OPENING_ERROR_DIFF, __LINE__, __FILE__);
    }

    char* Expression = FileToString(inputExpression);
    if(Expression == NULL)
    {
        ErrorParser(FILE_READING_ERROR_DIFF, __LINE__, __FILE__);
    }
    fclose(inputExpression);
    MEOW(RED_COLOR_ESC_SEQ"String read: %s\n" DEFAULT_COLOR_ESC_SEQ, Expression);

    tree_t tree = 
    {
        NewNodeDiff,
        CloneTreeDiff,
        NodeDump,

        NULL,
    };

    VariableTable_t VarTable = {};

    size_t TokensNumber = 0;
    Token_t* TokenisedExpression = Tokenise(Expression, strlen(Expression), &VarTable, &TokensNumber);


    PrintTokens(TokenisedExpression, TokensNumber, &VarTable);

    ErrorParser(ReadCrocodileTokens(TokenisedExpression, &tree, &VarTable), __LINE__, __FILE__);
    free(TokenisedExpression);
    
    FILE* outDot = NULL;
    if(flags.isOutFilename)
    {
        outDot = fopen(flags.OutFileName, "w+b");
        if(outDot == NULL)
        {
            ErrorParser(FILE_OPENING_ERROR_DIFF, __LINE__, __FILE__);
        }
        ErrorParser(PrintTreeGraphViz(outDot, tree, flags.OutFileName, &VarTable), __LINE__, __FILE__);
    }
    else
    {
        outDot = fopen(DEFAULT_OUTPUT_DOT_FILE_NAME, "w+b");
        if(outDot == NULL)
        {
            ErrorParser(FILE_OPENING_ERROR_DIFF, __LINE__, __FILE__);
        } 
        //ErrorParser(PrintTreeGraphViz(outDot, tree, DEFAULT_OUTPUT_DOT_FILE_NAME, &VarTable), __LINE__, __FILE__);
    }   

    free(Expression);

    tree_t diffTree = 
    {
        NewNodeDiff,
        CloneTreeDiff,
        NodeDump,
        NULL,           
    };

    FILE* outTex = fopen("deriv.tex", "w+b");
    assert(outTex);

    fprintf(outTex, "%s\n", TEX_HEAD);
    fprintf(outTex, "%s\n", TEX_START_LINE);
    ErrorParser(DumpTreeTex(tree.root, outTex, &VarTable), __LINE__, __FILE__);
    srand((unsigned int)time(NULL));
    if((diffTree.root = Differentiate(&tree, tree.root, outTex, &VarTable)) == NULL)
    {
        ErrorParser(NULL_POINTER_DIFF, __LINE__, __FILE__);
    }

    const char* SecondDumpFName = "Dump/DiffedTree.dot";
    FILE* SecondDump = fopen(SecondDumpFName, "w+b");
    //ErrorParser(PrintTreeGraphViz(SecondDump, diffTree, DEFAULT_OUTPUT_DOT_FILE_NAME, &VarTable), __LINE__, __FILE__);

    ErrorParser(DumpTreeTex(diffTree.root, outTex, &VarTable), __LINE__, __FILE__);
    //Taylor(&tree, &VarTable, outTex, 8);
    fprintf(outTex, "\\end{document}");
    fclose(outTex);

    BurnTree(&diffTree);
    BurnTree(&tree);
    return 0;
}
