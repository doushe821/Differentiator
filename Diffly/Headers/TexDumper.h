#ifndef TEX_DUMPER_H_INCLUDED
#define TEX_DUMPER_H_INCLUDED

#include "VariableTable.h"
#include <stdio.h>


__attribute((unused)) static const char* TEX_HEAD = "\\documentclass{article}\n\\usepackage[utf8]{inputenc}\n\\usepackage{amssymb}\n\\usepackage[russian]{babel}\n\\usepackage{amsmath}\n\\usepackage{graphicx}\n\\begin{document}\n";
__attribute((unused)) static const char* TEX_START_LINE = "Я всегда тяготел к экзотическим животным — змеям, черепахам. А жаб любил с самого детства. Ведь у них такой «улыбчивый» разрез рта, замечательные глаза с серебристой или золотистой крапинкой — куда там человеческим! Очень миролюбивы и неторопливы — незаменимые качества для хорошего соседа.\\newline\n";

__attribute((unused)) static const char* MATPLOTLIB_HEAD = "import matplotlib.pyplot as plt\nimport matplotlib\nimport numpy as np\n\nx=np.linspace(0, 5, 10000)\nfig, ax = plt.subplots(figsize=(5, 4))\nax.set_ylim([-5, 5])\n";
__attribute((unused)) static const char* PLOT_COMMENT = "\\begin{center}\nFunction plot is blue, Derivative plot is orange, MacLoren series plot is green\n\\end{center}\n";

int DumpTreeTex(void* root, FILE* out, FILE* plot, VariableTable_t* VarTable);
int DumpNodeTex(void* node, FILE* out, FILE* plot, VariableTable_t* VarTable);
int DumpVarTex(void* node, FILE* out, FILE* plot, VariableTable_t* VarTable);
int DumpConstTex(void* node, FILE* out, FILE* plot, VariableTable_t* VarTable);
int DumpOperationTex(void* node, FILE* out, FILE* plot, VariableTable_t* VarTable);
int DumpFunctionTex(void* node, FILE* out, FILE* plot, VariableTable_t* VarTable);
FILE* MakePlots();
void FinishTexDump(FILE* outTex, FILE* Plots);

#endif