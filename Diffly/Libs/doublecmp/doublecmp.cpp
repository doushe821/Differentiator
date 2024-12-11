#include <math.h>
#include "doublecmp.h"

int doublecmp(double a, double b, double acc)
{
    double delta = a - b;
    if (fabs(delta) <= acc)
        return 0;
    else
        return (delta>0)-(delta<0);
}