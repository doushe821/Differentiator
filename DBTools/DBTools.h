#ifndef DEBUG_TOOLS_INCLUDED
#define DEBUG_TOOLS_INCLUDED

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int meow(FILE* buffer, const char* format, ...);

#define  GREEN_COLOR_ESC_SEQ   "\033[32m"
#define    RED_COLOR_ESC_SEQ   "\033[31m"
#define YELLOW_COLOR_ESC_SEQ   "\033[33m"
#define DEFAULT_COLOR_ESC_SEQ  "\033[0m" 
#define    BLUE_COLOR_ESC_SEQ  "\033[34m"
#define MAGENTA_COLOR_ESC_SEQ  "\033[35m"
#define    CYAN_COLOR_ESC_SEQ  "\033[36m"





#ifndef NDEBUG
#define COMMA ,
#define ON_DEBUG(code) code
#define $ fprintf(stderr, "%s:%d\n", __FILE__, __LINE__)
#define MEOW(message, ...) meow(stderr, message, __VA_ARGS__)
#else
#define $
#define COMMA
#define ON_DEBUG(code)
#define MEOW(message, ...)
#endif

#endif