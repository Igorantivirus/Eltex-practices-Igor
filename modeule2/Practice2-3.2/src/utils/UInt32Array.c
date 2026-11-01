#include <stdlib.h>

#include <utils/Rand.h>
#include <utils/UInt32Array.h>

// public:

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
