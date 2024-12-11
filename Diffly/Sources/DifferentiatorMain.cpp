#include <assert.h>
#include <time.h>

#include "../Headers/Differentiator.h"
#include "../Headers/Tokenisation.h"
#include "../Headers/RecursiveDescensionTokens.h"
#include "../Headers/Transitions.h"
#include "../Headers/Taylor.h"

int main(int argc, char** argv)
{

// READING COMMANDLINE OPTIONS

    if(argc == 1)
    {
        ErrorParser(NO_INPUT_FILE, __LINE__, __FILE__);
    }
    FILE* inputExpression = fopen(argv[1], "a+b");
    flags_t flags = {};

    ErrorParser(parsecmd(argc, argv, &flags), __LINE__, __FILE__);

    if(flags.Taylor)
    {
        if(flags.TaylorDegree > 5)
        {
            ErrorParser(DEGREE_IS_TOO_BIG, __LINE__, __FILE__);
        }
    }

// CONVERTING EXPRESSION TO STRING

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

// GROWING EXPRESSION TREE

    tree_t tree = 
    {
        NewNodeDiff,
        CloneTreeDiff,
        NodeDump,

        NULL,
    };

    VariableTable_t VarTable = {};

    size_t   TokensNumber = 0;
    Token_t* TokenisedExpression = Tokenise(Expression, strlen(Expression), &VarTable, &TokensNumber);
    if(TokenisedExpression == NULL)
    {
        ErrorParser(TOKENISATION_FAILURE, __LINE__, __FILE__);
    }
    if(TokensNumber == 0)
    {
        ErrorParser(EMPTY_INPUT_STRING, __LINE__, __FILE__);
    }

    #ifndef NDEBUG
    PrintTokens(TokenisedExpression, TokensNumber, &VarTable);
    #endif

    ErrorParser(ReadCrocodileTokens(TokenisedExpression, &tree, &VarTable), __LINE__, __FILE__);
    free(TokenisedExpression);
    
// TREE DUMP IN GRAPH VIZ

    FILE* outDot = NULL;
    outDot = fopen(DEFAULT_OUTPUT_DOT_FILE_NAME, "w+b");
    if(outDot == NULL)
    {
        ErrorParser(FILE_OPENING_ERROR_DIFF, __LINE__, __FILE__);
    } 
    ErrorParser(PrintTreeGraphViz(outDot, tree, DEFAULT_OUTPUT_DOT_FILE_NAME), __LINE__, __FILE__);

    free(Expression);

// DIFFERENTIATION AHEAD

    tree_t diffTree = 
    {
        NewNodeDiff,
        CloneTreeDiff,
        NodeDump,
        NULL,           
    };

    char TexFileName[FILENAME_MAX] = {};
    if(flags.isOutFilename)
    {
        sprintf(TexFileName, "%s%s", TEX_FILE_FOLDER, flags.OutFileName);
    }
    else
    {
        sprintf(TexFileName, "%s", DEFAULT_TEX_FILE_NAME);
    }
    FILE* outTex = fopen(TexFileName, "w+b");
    if(outTex == NULL)
    {
        ErrorParser(FILE_OPENING_ERROR_DIFF, __LINE__, __FILE__);
    }


// MAKING PLOTS 

    FILE* Plots = MakePlots();
    if(Plots == NULL)
    {
        ErrorParser(FILE_OPENING_ERROR_DIFF, __LINE__, __FILE__);
    }
    
// DUMP HELL

    fprintf(outTex, "%s\n", TEX_HEAD);
    fprintf(outTex, "%s\n", TEX_START_LINE);
    ErrorParser(DumpTreeTex(tree.root, outTex, Plots, &VarTable), __LINE__, __FILE__);

    srand((unsigned int)time(NULL));
    if((diffTree.root = Differentiate(&tree, tree.root, outTex, Plots, &VarTable)) == NULL)
    {
        ErrorParser(NULL_POINTER_DIFF, __LINE__, __FILE__);
    }

    const char* SecondDumpFName = "Dump/DiffedTree.dot";
    FILE* SecondDump = fopen(SecondDumpFName, "w+b");
    ErrorParser(PrintTreeGraphViz(SecondDump, diffTree, DEFAULT_OUTPUT_DOT_FILE_NAME), __LINE__, __FILE__);
    fclose(SecondDump);

    ErrorParser(DumpTreeTex(diffTree.root, outTex, Plots, &VarTable), __LINE__, __FILE__);
    if(flags.Taylor)
    {
        double TPoint = 0;
        if(flags.TPointSet)
        {
            TPoint = flags.TaylorPoint;
        }
        ErrorParser(Taylor(&tree, &VarTable, outTex, Plots, flags.TaylorDegree, TPoint), __LINE__, __FILE__);
    }

    FinishTexDump(outTex, Plots);

    BurnTree(&diffTree);
    BurnTree(&tree);
    return 0;
}