#pragma once

#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>

#include "ConsoleOperation.h"

typedef ConsoleOperation (*Operation)(void *);
typedef void             (*ContextOperationBeroreNext)(void *);

typedef struct
{
    Operation   operation;
    const char *pointName;
} ElementMenu;

typedef struct
{
    ElementMenu               *elements;
    size_t                     size;
    const char                *title;
    void                      *context;
    ContextOperationBeroreNext oper;
} MenuElements;

MenuElements fillMenuElements(void *context, ContextOperationBeroreNext oper, const char *const title, const size_t size, ...);
void         freeMenuElements(MenuElements *const self);