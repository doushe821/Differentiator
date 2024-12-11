#include <stdlib.h>
#include <math.h>

#include "../Headers/DifferentiatorMath.h"

size_t factorialD(size_t a)
{
    if(a == 1 || a == 0)
    {
        return 1;
    }

    return a*factorialD(a-1);
}

double cot(double a)
{
    return 1/tan(a);
}

double acot(double a)
{
    return atan(1/a);
}

double log_a(double a, double b)
{
    return log(a) / log(b);
}

double CalcSum(double a, double b)
{
    return a+b;
}
double CalcSub(double a, double b)
{
    return a-b;
}
double CalcMul(double a, double b)
{
    return a*b;
}
double CalcDiv(double a, double b)
{
    return a/b;
}
double CalcPow(double a, double b)
{
    return pow(a, b);
}
