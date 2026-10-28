#pragma once

#include <stddef.h>

#include <utils/String.h>

typedef struct
{
    char  *str;
    size_t size;
} StringView;

double svToDouble(StringView sv);
StringView svFromString(const String str);