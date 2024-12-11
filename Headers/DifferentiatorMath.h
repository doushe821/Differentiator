#ifndef DIFFERENTIATOR_MATH_H_INCLUDED
#define DIFFERENTIATOR_MATH_H_INCLUDED

#include <stdlib.h>

double cot(double a);
double acot(double a);
double log_a(double a, double b);

const double EULER = 2.718;

double CalcSum(double a, double b);
double CalcSub(double a, double b);
double CalcMul(double a, double b);
double CalcDiv(double a, double b);
double CalcPow(double a, double b);

size_t factorialD(size_t a);

#endif