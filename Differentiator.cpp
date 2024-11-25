#include <string.h>
#include <ctype.h>

#include "Differentiator.h"

int DifferentiateTree(List_t* list, Node_t root)
{

    return 0;
}

int DiffSin(List_t* list, )
{

}

Node_t ReadNode(const char* ExpressionString, size_t* StringIndex, List_t* List, Node_t Parent ON_DEBUG(COMMA size_t* iteration))
{
    ON_DEBUG(size_t currentIteration = *iteration);
    MEOW(CYAN_COLOR_ESC_SEQ "\n(DEBUG)  |  Iteration %zu\n" DEFAULT_COLOR_ESC_SEQ, *iteration);
    ON_DEBUG((*iteration)++);

    size_t CurrenNodeType = 0;
    Node_t CurrentNode = NULL;
    const char* OpenBrake = strchr(ExpressionString + *StringIndex, '(');
    if(OpenBrake == NULL)
    {
        fprintf(stderr, "Syntax error: %s:%d\n", __FILE__, __LINE__);
        return NULL;
    }

    (*StringIndex) += 1 + ((size_t)OpenBrake - ((size_t)ExpressionString + *StringIndex));

    const char* CloseBrake = strchr(ExpressionString + *StringIndex, ')');
    if(CloseBrake == NULL)
    {
        fprintf(stderr, "Syntax error: %s:%d\n", __FILE__, __LINE__);
        return NULL;
    }

    MEOW(CYAN_COLOR_ESC_SEQ "(DEBUG)  |  ID = %zu\n" DEFAULT_COLOR_ESC_SEQ, *StringIndex);
    const char* NextOpenBrake = strchr(ExpressionString + *StringIndex, '(');

    if((size_t)NextOpenBrake < (size_t)CloseBrake && NextOpenBrake != NULL)
    {
        size_t NodeStringLen = (size_t)NextOpenBrake - ((size_t)ExpressionString + *StringIndex) + 1;
        char* NodeString = (char*)calloc(NodeStringLen + 1, sizeof(char));
        MEOW("String len = %zu\n", NodeStringLen);
        NodeString[NodeStringLen] = '\0';
        memcpy(NodeString, ExpressionString + *StringIndex, NodeStringLen);
        int code = GetFuncCode(NodeString);

        if(code == FUNCTION_NOT_FOUND)
        {
            MEOW(GREEN_COLOR_ESC_SEQ "(DEBUG)  |  Is operation (%s:%d)\n" DEFAULT_COLOR_ESC_SEQ, __FILE__, __LINE__);

            free(NodeString);
            
            Node_t LeftNode = ReadNode(ExpressionString, StringIndex, List, NULL ON_DEBUG(COMMA iteration));
            MEOW(BLUE_COLOR_ESC_SEQ "(DEBUG)  |  Back to iteration %zu\n" DEFAULT_COLOR_ESC_SEQ, currentIteration);
            int OpCode = 0;
            while((OpCode = *(ExpressionString + *StringIndex)) == ' ')
            {
                (*StringIndex)++;
            }
            MEOW(CYAN_COLOR_ESC_SEQ "(DEBUG)  |  OpCode = %d\n" DEFAULT_COLOR_ESC_SEQ, OpCode);
            if((OpCode = GetOpCode(OpCode)) == OPERATION_NOT_FOUND)
            {
                fprintf(stderr, "StringIndex = %zu\n", *StringIndex);
                fprintf(stderr, "Syntax error %s:%d\n", __FILE__, __LINE__);
                return NULL;
            }


            CurrentNode = CreateNodeList(List, OPERATION_TYPE_CODE, Parent, &OpCode, NULL, NULL);
            const char* RightBrake = strchr(ExpressionString + *StringIndex, '(');


            (*StringIndex) += (size_t)RightBrake - ((size_t)ExpressionString + *StringIndex);


            Node_t RightNode = ReadNode(ExpressionString, StringIndex, List, CurrentNode ON_DEBUG(COMMA iteration));
            
            MEOW(BLUE_COLOR_ESC_SEQ "(DEBUG)  |  Back to iteration %zu\n" DEFAULT_COLOR_ESC_SEQ, currentIteration);

            AddParent(LeftNode, CurrentNode);
            AddKid(CurrentNode, LeftNode, 0);
            AddKid(CurrentNode, RightNode, 1);
            (*StringIndex)++;
        }
        else
        {
            MEOW(GREEN_COLOR_ESC_SEQ "(DEBUG)  |  Is function (%s:%d)\n" DEFAULT_COLOR_ESC_SEQ, __FILE__, __LINE__);
            free(NodeString);
            CurrenNodeType = FUNCTION_TYPE_CODE;
            CurrentNode = CreateNodeList(List, CurrenNodeType, Parent, &code, NULL, NULL);

            (*StringIndex) += NodeStringLen - 1;

            Node_t LeftNode = ReadNode(ExpressionString, StringIndex, List, CurrentNode ON_DEBUG(COMMA iteration));
            MEOW(BLUE_COLOR_ESC_SEQ "(DEBUG)  |  Back to iteration %zu\n" DEFAULT_COLOR_ESC_SEQ, currentIteration);
            AddKid(CurrentNode, LeftNode, 0);
            (*StringIndex)++;
        }
    }
    else
    {
        if(isdigit(*(ExpressionString + *StringIndex)))
        {

            MEOW(GREEN_COLOR_ESC_SEQ "(DEBUG)  |  Is constant (%s:%d)\n" DEFAULT_COLOR_ESC_SEQ, __FILE__, __LINE__);

            double value = atof(ExpressionString + *StringIndex);
            MEOW(MAGENTA_COLOR_ESC_SEQ "(DEBUG)  |  Constant value = %lf\n" DEFAULT_COLOR_ESC_SEQ, value);

            CurrentNode = CreateNodeList(List, CONST_VALUE_TYPE_CODE, Parent, &value, NULL, NULL);
            (*StringIndex) += (size_t)CloseBrake - ((size_t)ExpressionString + *StringIndex) + 1;
        }
        else if(((size_t)CloseBrake - ((size_t)ExpressionString + *StringIndex)) == 1)
        {
            MEOW(GREEN_COLOR_ESC_SEQ "(DEBUG)  |  Is variable (%s:%d)\n" DEFAULT_COLOR_ESC_SEQ, __FILE__, __LINE__);
            int variable = *(ExpressionString + *StringIndex);
            CurrentNode = CreateNodeList(List, VARIABLE_VALUE_TYPE_CODE, Parent, &variable, NULL, NULL);
            (*StringIndex) += (size_t)CloseBrake - ((size_t)ExpressionString + *StringIndex) + 1;
        }
        else
        {
            fprintf(stderr, "Syntax error %s:%d\n", __FILE__, __LINE__);
            return NULL;
        }
    }

    MEOW(MAGENTA_COLOR_ESC_SEQ "(DEBUG)  |  Current node adress: %p (%s:%d)\n\n" DEFAULT_COLOR_ESC_SEQ, CurrentNode, __FILE__, __LINE__);
    return CurrentNode;

}


Node_t CreateNodeList(List_t* list, size_t type, Node_t parent, void* data, Node_t left, Node_t right)
{
    if(list == NULL)
    {
        fprintf(stderr, "Invalid list: %s:%d\n", __FILE__, __LINE__);
        return NULL;
    }

    size_t curIndex = GetFreeInd(list);
    DiffBaseNode FillerNode = {};
    PushFront(list, &FillerNode);
    size_t memIndex = 0;

    memcpy((char*)GetDataInd(list, curIndex) + memIndex, &type, sizeof(type));
    memIndex += sizeof(type);

    if(parent != NULL)
    {
        memcpy((char*)GetDataInd(list, curIndex) + memIndex, &parent, sizeof(type));
    }
    memIndex += sizeof(parent);

    size_t fertility = 2;
    memcpy((char*)GetDataInd(list, curIndex) + memIndex, &fertility, sizeof(fertility));
    memIndex += sizeof(fertility);

    if(left != NULL)
    {
        memcpy((char*)GetDataInd(list, curIndex) + memIndex, &left, sizeof(type));
    }
    memIndex += sizeof(left);

    if(right != NULL)
    {
        memcpy((char*)GetDataInd(list, curIndex) + memIndex, &right, sizeof(type));
    }
    memIndex += sizeof(right);

    size_t datasize = 8;
    memcpy((char*)GetDataInd(list, curIndex) + memIndex, &datasize, sizeof(datasize));
    memIndex += sizeof(datasize);

    if(data != NULL)
    {   
        if(type == FUNCTION_TYPE_CODE || type == VARIABLE_VALUE_TYPE_CODE || type == OPERATION_TYPE_CODE)
        {
            datasize = sizeof(int);
        }
        memcpy((char*)GetDataInd(list, curIndex) + memIndex, data, datasize);
    }
    MEOW("GetDatinmd : %p, %lf\n", GetDataInd(list, curIndex), *((double*)((char*)GetDataInd(list, curIndex) + 48)));
    MEOW("inc ase 4 vyteL : %d\n", *(int*)((char*)GetDataInd(list, curIndex) + 48));
    MEOW("Type : %zu\n", *(size_t*)((char*)GetDataInd(list, curIndex)));
    return GetDataInd(list, curIndex);
}

int GetFuncCode(const char* buffer)
{
    fprintf(stderr, "ff : %s\n", buffer);
    for(int i = 0; i < (int)NUMBER_OF_FUNTIONS; i++)
    {
        if(strncmp(buffer, functions[i].name, strlen(functions[i].name)) == 0)
        {
            MEOW("FUNC NAME = %s\n", functions[i].name);
            MEOW("GET FUNC RETURNING %d\n", functions[i].code);
            return functions[i].code;
        }
    }
    return FUNCTION_NOT_FOUND;
}

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