#include "Differentiator.h"

Node_t Differentiate(List_t* List, Node_t Node)
{
    Status st = {};

    size_t NodeType = *((size_t*)Node);

    switch(NodeType)
    {
        case CONST_VALUE_TYPE_CODE:
        {
            size_t zero = 0;
            memcpy((char*)Node + GetNodeMemShift(Node, DATA_FIELD_CODE), &zero, sizeof(zero));

            return Node;

            MEOW("Node: %p; is constant value\n", Node);
        }

        case OPERATION_TYPE_CODE:
        {
            operations[*((int*)((char*)Node + GetNodeMemShift(Node, DATA_FIELD_CODE)))].DiffFunc(List, Node);
        }
    }
}



Status DiffSum(List_t* List, Node_t Node)
{
    Status st = {};


}