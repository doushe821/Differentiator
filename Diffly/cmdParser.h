#ifndef CMD_PARSER_H_INCLUDED
#define CMD_PARSER_H_INCLUDED

#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>

__attribute((unused)) static const int NUMBER_OF_COMMANDS = 2;
__attribute((unused)) static const char* DEFAULT_OUTPUT_DOT_FILE_NAME = "Dump/differentiator.dot";

struct flags_t
{
    bool isOutFilename;
    char OutFileName[FILENAME_MAX];

    bool isHelp;
};

const option options[NUMBER_OF_COMMANDS]
{
    {"o", required_argument, NULL, 'o'},
    {"help", no_argument, NULL, 'h'},
};

int parsecmd(int argc, char** argv, flags_t* flags);

__attribute((unused)) static const char* HELP_MESSAGE = "This programm can differentiate mathematical expressions\n-o is a flag that is used set output .dot file name,\n";
__attribute((unused)) static const char* UNKNOWN_COMMAND_MESSAGE = "Unknown option, it will be ignored\n";
__attribute((unused)) static const char* MISSING_ARGUMENT = "Option missing argument, it will be set to default or ignored\n";
__attribute((unused)) static const char* PARSER_FAILURE = "Command line options parser failed\n";
#endif