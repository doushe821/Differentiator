#include <string.h>

#include "cmdParser.h"

int parsecmd(int argc, char** argv, flags_t* flags)
{
    if(argc <= 2)
    {
        return 0;
    }

    int opt = 0;
    int optid = 0;
    for(int i = 0; i < argc - 1; i++)
    {
        if((opt = getopt_long(argc, argv, "", options, &optid)) != -1)
        {
            switch(opt)
            {
                case 'h':
                {
                    flags->isHelp = true;
                    break;
                }
                case 'o':
                {
                    flags->isOutFilename = true;
                    if(optarg != NULL)
                    {
                        strncpy(flags->OutFileName, optarg, strlen(optarg));
                    }
                    break;
                    case '?':
                    {
                        fprintf(stderr, "%s", UNKNOWN_COMMAND_MESSAGE);
                        break;
                    }
                    case ':':
                    {
                        fprintf(stderr, "%s", MISSING_ARGUMENT);
                        break;
                    }
                    default:
                    {
                        fprintf(stderr, "%s", PARSER_FAILURE);
                        return -1;
                    }
                }
            }
        }
    }
    return -1;
}