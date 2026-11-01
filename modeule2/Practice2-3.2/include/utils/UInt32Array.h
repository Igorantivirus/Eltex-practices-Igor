#pragma once

#include <stdint.h>

typedef struct
{
    uint32_t *array;
    uint32_t  size;
} UInt32Array;

UInt32Array ui32aFillRand(const uint32_t size);
void        ui32aFree(UInt32Array *self);
