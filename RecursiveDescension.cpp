#include <stdio.h>
#include <stdlib.h>

void SyntaxError(int* Index, char Literal);
double GetG(const char* ExpressionString, int* Index);
double GetT(const char* ExpressionString, int* Index);
double GetP(const char* ExpressionString, int* Index);
double GetN(const char* ExpressionString, int* Index);
double GetE(const char* ExpressionString, int* Index);

int main()
{
    int index = 0;
    const char* Expression = "11.11$";
    printf("%lf\n", GetG(Expression, &index));
    return 0;

}

double GetG(const char* ExpressionString, int* Index)
{
    int val = GetE(ExpressionString, Index);
    if(ExpressionString[*Index] != '$')
    {
        SyntaxError(Index, val);
    }
    (*Index)++;
    fprintf(stderr, "%s returning %d\n", __func__, val);
    return val;
}

double GetE(const char* ExpressionString, int* Index)
{
    int val = GetT(ExpressionString, Index);
    while(ExpressionString[*Index] == '+' || ExpressionString[*Index] == '-')
    {
        int op = ExpressionString[*Index];
        (*Index)++;
        int val2 = GetT(ExpressionString, Index);
        if(op == '+')
        {
            val += val2;
        }
        else
        {
            val -= val2;
        }
    }
    fprintf(stderr, "%s returning %d\n", __func__, val);
    return val;
}

double GetT(const char* ExpressionString, int* Index)
{  
    int val = GetP(ExpressionString, Index);
    while(ExpressionString[*Index] == '*' || ExpressionString[*Index] == '/')
    {
        int op = ExpressionString[*Index];
        (*Index)++;
        int val2 = GetP(ExpressionString, Index);
        if(op == '*')
        {
            val *= val2;
        }
        else
        {
            val /= val2;
        }
    }
    fprintf(stderr, "%s returning %d\n", __func__, val);
    return val;
} 

double GetP(const char* ExpressionString, int* Index)
{
    if(ExpressionString[*Index] == '(')
    {
        (*Index)++;
        int val = GetE(ExpressionString, Index);
        {
            if(ExpressionString[*Index] != ')')
            {
                SyntaxError(Index, val);
            }
            (*Index)++;
            fprintf(stderr, "%s returning %d\n", __func__, val);
            return val;
        }
    }
    else
    {
        return GetN(ExpressionString, Index);
    }
}

double GetN(const char* ExpressionString, int* Index)
{
    double val = 0;
    int oldIndex = *Index;
    int DigitsCounter = 0;
    while('0' <= ExpressionString[*Index] && '9' >= ExpressionString[*Index] || '.' == ExpressionString[*Index])
    {
        if(DigitsCounter > 0)
        {
            DigitsCounter++;
        }
        if(ExpressionString[*Index] == '.')
        {
            DigitsCounter++;
            (*Index)++;
            continue;
        }
        fprintf(stderr, "%s current cymbol code = %d\n", __func__, ExpressionString[*Index]);
        val = val*10 + ExpressionString[*Index] - '0';
        (*Index)++;
    }
    if(*Index == oldIndex)
    {
        SyntaxError(Index, val);
    }
    if(DigitsCounter > 0)
    {
        if(DigitsCounter == 1)
        {
            SyntaxError(Index, ExpressionString[*Index]);
        }
        for(int i = 0; i < DigitsCounter - 1; i++)
        {
            val /= 10;
        }
    }
    fprintf(stderr, "%s returning %lg\n", __func__, val);
    return val;
}

void SyntaxError(int* Index, char Literal)
{
    fprintf(stderr, "Syntax error on p = %d, symbol is %c\n", *Index, Literal);
    abort();
}