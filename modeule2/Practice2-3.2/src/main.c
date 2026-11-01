#include <corecrt_memory.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vadefs.h>

uint32_t randU32(void)
{
    uint32_t r = 0;
    r = (r << 15) | ((uint32_t)rand() & 0x7FFFu);
    r = (r << 15) | ((uint32_t)rand() & 0x7FFFu);
    r = (r << 15) | ((uint32_t)rand() & 0x7FFFu);
    return r;
}

typedef struct
{
    uint32_t gateway;
    uint32_t mask;
    uint32_t count;

    bool gatewayInputed;
    bool maskInputed;
    bool countInputed;
} InputArgs;

InputArgs iaInit()
{
    InputArgs res;
    res.countInputed = false;
    res.maskInputed = false;
    res.gatewayInputed = false;
    return res;
}

bool iaIsValid(InputArgs *const self)
{
    return self->countInputed && self->gatewayInputed && self->maskInputed;
}

typedef struct
{
    uint32_t fillCount;                   // Число сгенерированных IP
    uint32_t countToMyNetwork;            // Число IP в сети
    uint32_t countToOtherNetwork;         // Число IP не в сети
    double   countToMyNetworkPercents;    // Процент IP в сети
    double   countToOtherNetworkPercents; // Процент IP не в сети
} Statistick;

typedef struct
{
    uint32_t *array;
    uint32_t  size;
} UInt32Array;

UInt32Array ui32aFillRand(const uint32_t size)
{
    UInt32Array res;
    res.size = size;
    res.array = malloc(size * sizeof(uint32_t));
    for (uint32_t i = 0; i < size; ++i)
        res.array[i] = randU32();
    return res;
}
void ui32aFree(UInt32Array *self)
{
    if (!self)
        return;
    free(self->array);
    self->size = 0;
    self->array = NULL;
}

bool fillStatistick(InputArgs args, Statistick *st)
{
    if (!iaIsValid(&args) || !st)
        return false;
    st->fillCount = args.count;
    st->countToMyNetwork = 0;
    st->countToOtherNetwork = 0;
    st->countToMyNetworkPercents = 0.;
    st->countToOtherNetworkPercents = 0.;
    UInt32Array array = ui32aFillRand(args.count);
    uint32_t    network = args.gateway & args.mask;
    for (uint32_t i = 0; i < args.count; ++i)
    {
        uint32_t prNwtwork = args.mask & array.array[i];
        if (prNwtwork == network)
            ++st->countToMyNetwork;
    }
    ui32aFree(&array);
    st->countToOtherNetwork = st->fillCount - st->countToMyNetwork;
    
    st->countToMyNetworkPercents = 100. * st->countToMyNetwork / st->fillCount;
    st->countToOtherNetworkPercents = 100. * st->countToOtherNetwork / st->fillCount;

    return true;
}

void statPrint(Statistick *st)
{
    if (!st)
        return;
    printf("Число сгенерированных ip:        %u\n", st->fillCount);
    printf("Число ip в указанной подсети:    %u (%f %%)\n", st->countToMyNetwork, st->countToMyNetworkPercents);
    printf("Число ip не в указанной подсети: %u (%f %%)\n", st->countToOtherNetwork, st->countToOtherNetworkPercents);
}

typedef enum
{
    ArgsParseResultOk,
    ArgsParseResultFlagAtEnd,
    ArgsParseResultInvalidArg,
    ArgsParseResultDoubleInit,
    ArgsParseResultDevError
} ArgsParseResult;

typedef ArgsParseResult (*FillerFunction)(const char *arg, void *fill);
typedef struct
{
    const char    *cmdLong;
    const char    *cmdShor;
    FillerFunction filler;
} CommandElement;

typedef struct
{
    CommandElement *cmds;
    uint32_t        size;
} Commands;

Commands cmdFillCommands(uint32_t count, ...)
{
    Commands cmds;
    cmds.size = count;
    cmds.cmds = malloc(count * sizeof(CommandElement));
    va_list lst;
    va_start(lst, count);
    for (uint32_t i = 0; i < count; ++i)
    {
        cmds.cmds[i].cmdShor = va_arg(lst, char *);
        cmds.cmds[i].cmdLong = va_arg(lst, char *);
        cmds.cmds[i].filler = va_arg(lst, FillerFunction);
    }
    va_end(lst);
    return cmds;
}
void cmdFree(Commands *self)
{
    if (!self)
        return;
    free(self->cmds);
    self->cmds = NULL;
    self->size = 0;
}

ArgsParseResult cmdInitArgsWithFree(int argc, char **argv, Commands cmds, void *context)
{
    for (uint32_t i = 1; i < argc; ++i)
    {
        char *arg = argv[i];
        for (uint32_t j = 0; j < cmds.size; ++j)
        {
            CommandElement *elem = cmds.cmds + j;
            if (strcmp(argv[i], elem->cmdShor) == 0 || strcmp(argv[i], elem->cmdLong) == 0)
            {
                if (i == argc - 1)
                    return ArgsParseResultFlagAtEnd;
                ArgsParseResult res;
                if ((res = elem->filler(argv[i + 1], context) != 0) != ArgsParseResultOk)
                    return res;
                ++i;
                break;
            }
        }
    }
    cmdFree(&cmds);
    return ArgsParseResultOk;
}

bool parseUInt32(const char *arg, uint32_t *val)
{
    char              *end;
    unsigned long long input = strtoull(arg, &end, 10);
    if (*end != '\0' || input > (unsigned long long)UINT32_MAX)
        return false;
    *val = input;
    return true;
}

bool parseIp(const char *str, uint32_t *ip, const char **end)
{
    if (!str || !ip)
        return false;

    uint8_t     ipRes[4];
    const char *p = str;

    for (int i = 0; i < 4; ++i)
    {
        if (*p < '0' || *p > '9')
            return false;

        if (p[0] == '0' && p[1] >= '0' && p[1] <= '9')
            return false;

        unsigned v = 0;
        int      n = 0;
        while (*p >= '0' && *p <= '9')
        {
            if (++n > 3) // > 3 цифр — заведомо > 255
                return false;
            v = v * 10 + (unsigned)(*p - '0');
            ++p;
        }
        if (v > 255)
            return false;
        ipRes[i] = (uint8_t)v;

        if (i < 3)
        {
            if (*p != '.') // после октетов 1..3 нужна точка
                return false;
            ++p;
        }
    }

    if (end)
        *end = p;
    memcpy(ip, ipRes, sizeof(ipRes));
    return true;
}

ArgsParseResult cmdInitCount(const char *arg, void *context)
{
    if (!context)
        return ArgsParseResultDevError;
    InputArgs *args = context;
    if (args->countInputed)
        return ArgsParseResultDoubleInit;

    uint32_t val;
    if (!parseUInt32(arg, &val))
        return ArgsParseResultInvalidArg;

    args->count = val;
    args->countInputed = true;
    return ArgsParseResultOk;
}

ArgsParseResult cmdInitMask(const char *arg, void *context)
{
    if (!context)
        return ArgsParseResultDevError;
    InputArgs *args = context;
    if (args->maskInputed)
        return ArgsParseResultDoubleInit;

    uint32_t    mask;
    const char *end = NULL;
    if (parseIp(arg, &mask, &end))
    {
        if (*end != '\0')
            return ArgsParseResultInvalidArg;
        args->mask = mask;
        args->maskInputed = true;
        return ArgsParseResultOk;
    }
    if (!parseUInt32(arg, &mask))
        return ArgsParseResultInvalidArg;
    args->mask = mask;
    args->maskInputed = true;
    return ArgsParseResultOk;
}

ArgsParseResult cmdInitGateway(const char *arg, void *context)
{
    if (!context)
        return ArgsParseResultDevError;
    InputArgs *args = context;
    if (args->gatewayInputed)
        return ArgsParseResultDoubleInit;

    const char *end = NULL;
    uint32_t    gateway;
    if (!parseIp(arg, &gateway, &end))
        return ArgsParseResultInvalidArg;

    args->gateway = gateway;
    args->gatewayInputed = true;
    if (*end != '/')
        return ArgsParseResultOk;

    uint32_t mask;
    if (!parseUInt32(end + 1, &mask))
        return ArgsParseResultInvalidArg;
    args->mask = mask;
    args->maskInputed = true;
    return ArgsParseResultOk;
}

const char *convertArgsParseResult(ArgsParseResult res)
{
    switch (res)
    {
    case ArgsParseResultDevError:
        return "Ошибка приложения";
    case ArgsParseResultFlagAtEnd:
        return "Флаг без определения аргумента";
    case ArgsParseResultDoubleInit:
        return "Двойное определение параметра";
    case ArgsParseResultInvalidArg:
        return "Аргумент введён неверно";
    case ArgsParseResultOk:
        return NULL;
    default:
        return "Неизвестный результат";
    }
}

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
        "-g", "--gateway", cmdInitGateway);

    ArgsParseResult res;
    if ((res = cmdInitArgsWithFree(argc, argv, cmds, &args)) != ArgsParseResultOk)
    {
        printf("Ошибка: %s\n", convertArgsParseResult(res));
        return EXIT_FAILURE;
    }

    Statistick st;
    if (!fillStatistick(args, &st))
    {
        puts("Не все аргументы заполнены.");
        return EXIT_FAILURE;
    }
    statPrint(&st);

    return EXIT_SUCCESS;
}