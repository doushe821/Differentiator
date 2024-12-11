#include <string.h>

#include "../Headers/cmdParser.h"
#include "../Libs/DBTools/DBTools.h"

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
        MEOW("OptID = %d\n", optid);
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
                        MEOW("RT RT RT %s:%d\n", __FILE__, __LINE__);
                        strncpy(flags->OutFileName, optarg, strlen(optarg));
                    }
                    optid++;
                    MEOW(YELLOW_COLOR_ESC_SEQ"%s%d ## Read output PDF file name: %s\n" DEFAULT_COLOR_ESC_SEQ, __FILE__, __LINE__, flags->OutFileName);
                    break;
                }
                case 'T':
                {
                    flags->Taylor = true;
                    flags->TaylorDegree = (size_t)atoi(optarg);

                    break;
                }
                case 'P':
                {
                    flags->TPointSet = true;
                    flags->TaylorPoint = atof(optarg);

                    break;
                }
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
    return 0;
}

