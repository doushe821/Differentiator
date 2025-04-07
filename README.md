# Mathematical expression differentiation program
**About**
This program can calculate any derivative algebraically or numerically. It is also able to approximate functions with Taylor row. Results are saved in LaTex format.

**Functionality**
- Can calculate derivative of any algebraic expression with 1 variable (made so user can use literas instead of constants).
- Makes a .tex and .pdf file with results (there is step-by-step calcultation, result and Taylor's row coefficients with a plot). There is also tree dump version
- It supports command line instructions:

--help 

--pdf [Output file name]

--point [double value] - point in which Taylor will be calculated (program can identify and prohibit actions such as dividing by zero)

--Taylor [integer value] - degree of Taylor's row approximation

- Simplifies expressions by: constant convolution

- Has a tree dump function for debug purposes.

**Implementation details**
Program has lexer module to tokenize user's input and head tokens array to syntaxer. Expression is divide into Variables, Operations, Constants and Functions. Syntaxer is implemented by using recursive descent with following grammatics:

// TODO
Expression
Sum
Multiplication 
Power
Bracket
Function
Number / Variable value
// TODO

