#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Differentiator.h"

void DumpDoubleNode(void* a, FILE* fp);

int main()
{
    
    return 0;
}

void DumpDoubleNode(void* a, FILE* fp)
{
    double val = 0;
    memcpy(&val, a, sizeof(double));
    fprintf(fp, "%lf", val);
}