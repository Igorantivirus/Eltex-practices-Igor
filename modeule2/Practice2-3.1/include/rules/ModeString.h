#pragma once

#include <sys/types.h>

typedef struct
{
    char mode[10];
} ModeString;

ModeString modeToStringRWX(mode_t mode);
ModeString modeToStringBin(mode_t mode);