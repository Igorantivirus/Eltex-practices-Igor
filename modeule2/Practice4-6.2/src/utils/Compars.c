#include "utils/String.h"
#include <string.h>
#include <utils/Compars.h>

int cmpString(const void *const left, const void *const right)
{
    const String *ls = left;
    const String *rs = right;
    int           res = strcmp(ls->str, rs->str);
    return res < 0 ? -1 : res > 0 ? 1 : 0;
}