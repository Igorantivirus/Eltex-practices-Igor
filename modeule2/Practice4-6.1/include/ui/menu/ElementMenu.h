#pragma once

#include <stdarg.h>
#include <stdlib.h>
#include <stddef.h>

#include "ConsoleOperation.h"

typedef ConsoleOperation (*Operation)(void *);

typedef struct
{
    Operation operation;
    char     *pointName;
} ElementMenu;

typedef struct
{
    ElementMenu *elements;
    size_t       size;
    const char *title;
    void       *context;
} MenuElements;

MenuElements fillMenuElements(void *context, const char *const title, const size_t size, ...);
