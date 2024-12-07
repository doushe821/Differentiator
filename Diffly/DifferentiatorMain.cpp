#include "Differentiator.h"
#include "Tokenisation.h"
#include "RecursiveDescensionTokens.h"
#include "Taylor.h"

#include <assert.h>
#include <time.h>

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
    MEOW(RED_COLOR_ESC_SEQ"String read: %s\n" DEFAULT_COLOR_ESC_SEQ, Expression);

    tree_t tree = 
    {
        NewNodeDiff,
        CloneTreeDiff,
        NodeDump,

        NULL,
    };

    VariableTable_t VarTable = {};
    VarTable.Vars = (Variable_t*)calloc(MAX_VARIABLES, sizeof(Variable_t));
    size_t TokensNumber = 0;
    Token_t* TokenisedExpression = Tokenise(Expression, strlen(Expression), &VarTable, &TokensNumber);
    PrintTokens(TokenisedExpression, TokensNumber, &VarTable);
    ErrorParser(ReadCrocodileTokens(TokenisedExpression, &tree, &VarTable), __LINE__);
    //ErrorParser(ReadCrocodile(Expression, &tree), __LINE__);
    free(TokenisedExpression);
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

    tree_t diffTree = 
    {
        NewNodeDiff,
        CloneTreeDiff,
        NodeDump,
        NULL,           
    };

    FILE* outTex = fopen("deriv.tex", "w+b");
    assert(outTex);

    fprintf(outTex, "\\documentclass{article}\n\\usepackage[utf8]{inputenc}\n\\usepackage{amssymb}\n\\usepackage[russian]{babel}\n\\usepackage{amsmath}\n\\begin{document}\n");

    srand((unsigned int)time(NULL));
    if((diffTree.root = Differentiate(&tree, tree.root, outTex, &VarTable)) == NULL)
    {
        ErrorParser(NULL_POINTER_DIFF, __LINE__);
    }

    const char* SecondDumpFName = "Dump/DiffedTree.dot";
    FILE* SecondDump = fopen(SecondDumpFName, "w+b");
    ErrorParser(PrintTreeGraphViz(SecondDump, diffTree, DEFAULT_OUTPUT_DOT_FILE_NAME), __LINE__);

    ErrorParser(DumpTreeTex(diffTree.root, outTex, &VarTable), __LINE__);
    Taylor(&tree, &VarTable, outTex);
    fprintf(outTex, "\\end{document}");
    fclose(outTex);

    free(VarTable.Vars);
    BurnTree(&diffTree);
    BurnTree(&tree);
    return 0;
}

/*int PrintTreeTex(tree_t* tree, FILE* out)
{
    if(tree == NULL || out == NULL)
    {
        return NULL_POINTER_DIFF;
    }

    return -1;
}*/
