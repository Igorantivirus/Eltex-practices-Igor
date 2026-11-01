#pragma once

#include <stdint.h>
#include <ui/InputArgs.h>

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

Commands cmdFillCommands(uint32_t count, ...);
void     cmdFree(Commands *self);

ArgsParseResult cmdInitArgsWithFree(int argc, char **argv, Commands cmds, void *context);

ArgsParseResult cmdInitCount(const char *arg, void *context);
ArgsParseResult cmdInitMask(const char *arg, void *context);
ArgsParseResult cmdInitGateway(const char *arg, void *context);
ArgsParseResult cmdInitSeed(const char *arg, void *context);

const char *convertArgsParseResult(ArgsParseResult res);
