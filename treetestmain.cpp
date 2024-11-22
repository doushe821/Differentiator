#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Differentiator.h"

void DumpDoubleNode(void* a, FILE* fp);

int main()
{
    //FILE* source = fopen("test1.mxp", "a+b");
    double a = 1;
    double b = 2;
    double c = 3;
    Node_t node1 = CreateNode(0, NULL, sizeof(double), &a, 4, NewNode(0, NULL, &b, NULL, NULL), NewNode(0, NULL, &c, NULL, NULL), NewNode(0, NULL, &b, NULL, NULL), NewNode(0, NULL, &b, NULL, NULL));

    MEOW("\033[32mnode1 left data = %lf\n\033[0m", *(double*)GetNodeData(node1));
    MEOW("\033[32mnode1 left data = %lf\n\033[0m", GetNodeData(GetKidNode(node1, 0)));

    FILE* dump = fopen("dump1", "w+b");
    const char* dddd = "dump1";
    PrintTree(node1, dump, DumpDoubleNode, dddd);
    BurnTree(node1);
    fclose(dump);
    return 0;
}

void DumpDoubleNode(void* a, FILE* fp)
{
    double val = 0;
    memcpy(&val, a, sizeof(double));
    fprintf(fp, "%lf", val);
}