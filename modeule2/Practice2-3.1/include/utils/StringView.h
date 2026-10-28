#pragma once

#include <stddef.h>

#include "String.h"

typedef struct
{
    const char *str;
    size_t      size;
} StringView;

double     svToDouble(StringView sv);
StringView svFromString(const String str);
StringView svFromChars(const char *const str);