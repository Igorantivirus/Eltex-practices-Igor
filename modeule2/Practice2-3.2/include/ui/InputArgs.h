#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct
{
    uint32_t gateway;
    uint32_t mask;
    uint32_t count;
    uint32_t seed;

    bool gatewayInputed;
    bool maskInputed;
    bool countInputed;
    bool seedInputed;
} InputArgs;

InputArgs iaInit(void);
bool      iaIsValid(InputArgs *const self);
