#include <string.h>
#include <ctype.h>

#include "Differentiator.h"

Node_t ReadNode(const char* ExpressionString, size_t* StringIndex, List_t* List, Node_t Parent ON_DEBUG(COMMA size_t* iteration))
{

    MEOW(CYAN_COLOR_ESC_SEQ "\n(DEBUG) | func: %s ## %s:%d, iteration number %zu\n" DEFAULT_COLOR_ESC_SEQ, __func__, __FILE__, __LINE__, (*iteration)++);
    MEOW(CYAN_COLOR_ESC_SEQ "(DEBUG) | func: %s ## Parameters: %s, (StringIndex) %zu, (List ptr) %p, (Parent ptr) %p\n" DEFAULT_COLOR_ESC_SEQ, __func__, ExpressionString, *StringIndex, List, Parent);

    size_t CurrentNodeID = GetFreeInd(List);
    int BrakeCheck = *(ExpressionString + (*StringIndex)++);
    if(BrakeCheck == '(')
    {
        MEOW(MAGENTA_COLOR_ESC_SEQ "(DEBUG) |  Found new node (%s:%d)\n" DEFAULT_COLOR_ESC_SEQ, __FILE__, __LINE__);

        PushFront(List, NewNode(0, NULL, NULL, NULL, NULL));
        AddKid(GetDataInd(List, CurrentNodeID), ReadNode(ExpressionString, StringIndex, List, Parent ON_DEBUG(COMMA iteration)), 0);
        (*StringIndex)++;
    }

    (*StringIndex)--;

    NodeTypeIdentifier CurNodeTI = {};
    const char* NodeEnd = strchr(ExpressionString + *StringIndex, ')');
    const char* NextOpen = strchr(ExpressionString + *StringIndex, '(');
    if(NodeEnd == NULL)
    {
        fprintf(stderr, "Syntax error: %s:%d\n", __FILE__, __LINE__);
        return NULL;
    }

    MEOW(MAGENTA_COLOR_ESC_SEQ "(DEBUG) |  String index = %zu\n", *StringIndex);

    MEOW(MAGENTA_COLOR_ESC_SEQ "(DEBUG) | \')\' index = %zu\n", (size_t)NodeEnd - (size_t)ExpressionString);


    CurNodeTI = GetNodeType(ExpressionString + *StringIndex);
    if(CurNodeTI.Type == CONST_VALUE_TYPE_CODE)
    {

        MEOW(MAGENTA_COLOR_ESC_SEQ "(DEBUG) | Node contains constant value (%s:%d)\n" DEFAULT_COLOR_ESC_SEQ, __FILE__, __LINE__);

        double value = atof(ExpressionString + *StringIndex);

        MEOW(MAGENTA_COLOR_ESC_SEQ "(DEBUG) | Value = %lf\n" DEFAULT_COLOR_ESC_SEQ, value);

        PushFront(List, NewNode(CONST_VALUE_TYPE_CODE, Parent, &value, NULL, NULL));

        (*StringIndex) += (size_t)NodeEnd - ((size_t)ExpressionString + *StringIndex) + 1;
        return GetDataInd(List, CurrentNodeID);
    }

    if(CurNodeTI.Type == VARIABLE_VALUE_TYPE_CODE)
    {

        MEOW(MAGENTA_COLOR_ESC_SEQ "(DEBUG) |  Node contains variable (%s:%d)\n" DEFAULT_COLOR_ESC_SEQ, __FILE__, __LINE__);
        MEOW(MAGENTA_COLOR_ESC_SEQ "(DEBUG) |  Variable ASCII code & symbol: %d, %c\n" DEFAULT_COLOR_ESC_SEQ, CurNodeTI.VarName, CurNodeTI.VarName);

        PushFront(List, NewNode(VARIABLE_VALUE_TYPE_CODE, Parent, &CurNodeTI.VarName, NULL, NULL));
        (*StringIndex) += (size_t)NodeEnd - ((size_t)ExpressionString + *StringIndex) + 1;
        return GetDataInd(List, CurrentNodeID);
    }

    if(CurNodeTI.Type == FUNCTION_TYPE_CODE)
    {

        MEOW(MAGENTA_COLOR_ESC_SEQ "(DEBUG) |  Node contains constant funtion (%s:%d)\n" DEFAULT_COLOR_ESC_SEQ, __FILE__, __LINE__);

        const char* ValuePtr = strchr(ExpressionString + *StringIndex, '('); 
        PushFront(List, NewNode(FUNCTION_TYPE_CODE, Parent, NULL, NULL, NULL));
        (*StringIndex) += (size_t)ValuePtr - ((size_t)ExpressionString + *StringIndex) + 1;
        AddKid(GetDataInd(List, CurrentNodeID), ReadNode(ExpressionString, StringIndex, List, Parent ON_DEBUG(COMMA iteration)), 0);
        return GetDataInd(List, CurrentNodeID);
    }

    double OpSymbol = 0;
    while((OpSymbol = *(ExpressionString + (*StringIndex)++)) == ' ')
    {
        continue;
    }

    const char* SecondArg = strchr(ExpressionString + *StringIndex, '(');
    if(SecondArg == NULL)
    {

        MEOW(MAGENTA_COLOR_ESC_SEQ "(DEBUG) |  Second argument found which means node is an operand (%s:%d)\n" DEFAULT_COLOR_ESC_SEQ, __FILE__, __LINE__);

        fprintf(stderr, "Syntax error: %s:%d\n", __FILE__, __LINE__);
        return NULL;
    }
    AddData(GetDataInd(List, CurrentNodeID), &OpSymbol);
    (*StringIndex) += (size_t)SecondArg - ((size_t)ExpressionString + *StringIndex) + 1;
    AddKid(GetDataInd(List, CurrentNodeID), ReadNode(ExpressionString, StringIndex, List, GetDataInd(List, CurrentNodeID) ON_DEBUG(COMMA iteration)), 1);

    return GetDataInd(List, CurrentNodeID);
}

NodeTypeIdentifier GetNodeType(const char* SubExpression, size_t length)
{
    NodeTypeIdentifier nti = {};
    if(isdigit(*SubExpression))
    {
        nti.Code = CONST_VALUE_TYPE_CODE;
        nti.Value = atof(SubExpression);
        return nti;
    }
    int code = 0;
    if((code = GetFuncCode(SubExpression)) != FUNCTION_NOT_FOUND)
    {
        nti.Type = FUNCTION_TYPE_CODE;
        nti.Code = code;
        return nti;
    }
    nti.Type = VARIABLE_VALUE_TYPE_CODE;
    int varName = *SubExpression;
    memcpy(&nti.VarName, &varName, sizeof(int));
    return nti;
}

int GetFuncCode(const char* buffer)
{
    for(int i = 1; i < NUMBER_OF_FUNTIONS; i++)
    {
        if(strncmp(buffer, functions[i].name, strlen(functions[i].name)) == 0)
        {
            return i;
        }
    }
    return FUNCTION_NOT_FOUND;
}

size_t GetFileSize(FILE* fp) //TODO separate file managing lib
{
    
    fseek(fp, 0L, SEEK_END);
    
    size_t size = (size_t)ftell(fp);
    
    rewind(fp);
    return size;
}

void EmergentDtor(FILE* fp, List_t* NodeList, char* ExpressionString, Node_t root, FILE* dmp)
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
    if(root != NULL)
    {
        BurnTree(root);
    }
    if(dmp != NULL)
    {
        fclose(dmp);
    }
}