#ifndef DIFFERENTIATOR_H_INCLUDED
#define DIFFERENTIATOR_H_INCLUDED

#include "ExpressionReader.h"

#ifndef NDEBUG
#include "DBTools/DBTools.h"
#endif

Node_t Differentiate(List_t* List, Node_t Node);

#endif