#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include <ui/Commands.h>
#include <ui/Parser.h>

// public:

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
                if ((res = elem->filler(argv[i + 1], context)) != ArgsParseResultOk)
                    return res;
                ++i;
                break;
            }
        }
    }
    cmdFree(&cmds);
    return ArgsParseResultOk;
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
    if (!parseUInt32(arg, &mask) || mask > 32)
        return ArgsParseResultInvalidArg;
    args->mask = (~0u) << (32 - mask);
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
    if (!parseUInt32(end + 1, &mask) || mask > 32)
        return ArgsParseResultInvalidArg;
    args->mask = (~0u) << (32 - mask);
    args->maskInputed = true;
    return ArgsParseResultOk;
}

ArgsParseResult cmdInitSeed(const char *arg, void *context)
{
    if (!context)
        return ArgsParseResultDevError;
    InputArgs *args = context;
    if (args->seedInputed)
        return ArgsParseResultDoubleInit;

    uint32_t seed;
    if (!parseUInt32(arg, &seed))
        return ArgsParseResultInvalidArg;
    args->seed = seed;
    args->seedInputed = true;
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
