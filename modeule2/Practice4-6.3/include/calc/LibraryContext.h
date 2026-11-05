#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef double (*LibraryFunction)(const double left, const double right);

typedef struct
{
    void           *handle;
    LibraryFunction function;
    char            operation;
    uint32_t        priority;
} LibraryContext;

bool clInit(LibraryContext *const self, const char *const path);

void clFree(LibraryContext *const self);

void clFreeVoid(void *const self);