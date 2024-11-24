#ifndef NODE_STRUCT_H_INCLUDED
#define NODE_STRUCT_H_INCLUDED

const size_t BASE_NODE_SIZE = 32;

typedef void* Node_t;

struct BaseNode // VOID* ptr (Base node struct)
{
    size_t type;
    Node_t parent;
    size_t fertility;
    void* kids;
    size_t datasize;
    void* data;
};

const int NUMBER_OF_FIELDS = 6;

struct field
{
    const char* fieldName;
    int codeVal;
};

enum fieldCodes
{
    TYPE_FIELD_CODE = 0,
    PARENT_FIELD_CODE = 1,
    FERTILITY_FIELD_CODE = 2,
    KIDS_FIELD_CODE = 3,
    DATASIZE_FIELD_CODE = 4,
    DATA_FIELD_CODE = 5,
};

const field BaseNodeFields[NUMBER_OF_FIELDS] = 
{
    {"type", TYPE_FIELD_CODE},
    {"parent", PARENT_FIELD_CODE},
    {"fertility", FERTILITY_FIELD_CODE},
    {"kids", KIDS_FIELD_CODE},
    {"datasize", DATASIZE_FIELD_CODE},
    {"data", DATA_FIELD_CODE},
};


#endif