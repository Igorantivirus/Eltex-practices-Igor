#include <utils/DynamicArray.h>

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

// private:

#define DYNAMICARRAY_NEXT_SIZE(s) ((s) * 3 / 2 + 1)

#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

// public:

DynamicArray daInit(const uint32_t oneElementSize, DaFreeFunction freeFunction)
{
    DynamicArray res;
    res.oneElementSize = oneElementSize;
    res.freeFunction = freeFunction;
    res.size = res.capacity = 0;
    res.array_ = NULL;
    return res;
}
DynamicArray daInitReserve(const uint32_t oneElementSize, const size_t capacity, DaFreeFunction freeFunction)
{
    DynamicArray res;
    res.oneElementSize = oneElementSize;
    res.freeFunction = freeFunction;
    res.capacity = capacity;
    res.size = 0;
    res.array_ = malloc(res.capacity * res.oneElementSize);
    return res;
}

void daFree(DynamicArray *const self)
{
    if (!self)
        return;
    if (self->freeFunction)
        for (size_t i = 0; i < self->size; ++i)
            self->freeFunction(self->array_ + i * self->oneElementSize);
    free(self->array_);
    self->capacity = self->size = self->oneElementSize = 0;
    self->array_ = NULL;
}

void daClear(DynamicArray *const self)
{
    if (!self)
        return;
    if (self->freeFunction)
        for (uint32_t i = 0; i < self->size; ++i)
            self->freeFunction(self->array_ + i * self->oneElementSize);
    self->size = 0;
}
void daReserve(DynamicArray *const self, const size_t newCapacity)
{
    if (!self || self->capacity >= newCapacity)
        return;
    void *newArray = realloc(self->array_, newCapacity * self->oneElementSize);
    if (!newArray)
        return;
    self->array_ = newArray;
    self->capacity = newCapacity;
}

void daInsert(DynamicArray *const self, const size_t index, const DaOneElementPtr ptr)
{
    if (!self || index > self->size || !ptr)
        return;

    if (self->size + 1 > self->capacity)
        daReserve(self, max(DYNAMICARRAY_NEXT_SIZE(self->capacity), self->size + 1));
    if (index != self->size)
        memmove(self->array_ + (index + 1) * self->oneElementSize, self->array_ + index * self->oneElementSize, (self->size - index) * self->oneElementSize);

    memmove(self->array_ + index * self->oneElementSize, ptr, self->oneElementSize);
    ++self->size;
}

void daPushBack(DynamicArray *const self, const DaOneElementPtr ptr)
{
    if (self)
        daInsert(self, self->size, ptr);
    return;
}

void daRemove(DynamicArray *const self, const size_t index)
{
    if (!self || index >= self->size)
        return;
    if (self->freeFunction)
        self->freeFunction(self->array_ + index * self->oneElementSize);
    memmove(self->array_ + index * self->oneElementSize, self->array_ + (index + 1) * self->oneElementSize, (self->size - index - 1) * self->oneElementSize);
    --self->size;
}

DaOneElementPtr daGetAt(const DynamicArray *const self, const size_t index)
{
    if (!self || index >= self->size)
        return NULL;
    return self->array_ + index * self->oneElementSize;
}

void daSetAt(DynamicArray *const self, const size_t index, const DaOneElementPtr ptr)
{
    if (!self || index >= self->size)
        return;
    if (self->freeFunction)
        self->freeFunction(self->array_ + index * self->oneElementSize);
    memmove(self->array_ + index * self->oneElementSize, ptr, self->oneElementSize);
}