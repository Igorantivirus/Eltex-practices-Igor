#pragma once

#include <sys/types.h>

#include <utils/StringView.h>

typedef enum
{
    ModeParseResultOk,
    ModeParseResultError
} ModeParseResult;

ModeParseResult interpStringToMode(StringView sv, mode_t *out);
