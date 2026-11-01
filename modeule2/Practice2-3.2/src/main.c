#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <stat/Statistick.h>
#include <ui/Commands.h>
#include <ui/InputArgs.h>

int main(int argc, char **argv)
{
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif
    InputArgs args = iaInit();

    Commands cmds = cmdFillCommands(
        3,
        "-c", "--count", cmdInitCount,
        "-m", "--mask", cmdInitMask,
        "-g", "--gateway", cmdInitGateway,
        "-s", "--seed", cmdInitSeed);

    ArgsParseResult res;
    if ((res = cmdInitArgsWithFree(argc, argv, cmds, &args)) != ArgsParseResultOk)
    {
        printf("Ошибка: %s\n", convertArgsParseResult(res));
        return EXIT_FAILURE;
    }

    if (args.seedInputed)
        srand(args.seed);
    else
        srand(time(NULL));

    Statistick st;
    if (!fillStatistick(args, &st))
    {
        puts("Не все аргументы заполнены.");
        return EXIT_FAILURE;
    }
    statPrint(&st);

    return EXIT_SUCCESS;
}
