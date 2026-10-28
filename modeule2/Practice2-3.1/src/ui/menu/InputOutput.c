#include <ui/menu/InputOutput.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <utils/DynamicArray.h>
#include <utils/String.h>

uint32_t inReadUInt32(const char *const text, uint32_t defErr)
{
    printf("%s", text);
    uint32_t res = defErr;
    scanf("%u", &res);

    int c;
    for (int c = getchar(); c != '\n' && c != EOF; c = getchar())
        ;
    return res;
}

DynamicArray inReadStringArray(const char *const text, const char *const next)
{
    printf("%s\n", text);
    DynamicArray arr = daInitType(String, sFreeVoid);
    String       str;
    for (str = sInputCCO(next); str.size > 0; str = sInputCCO(next))
        daPushBack(&arr, &str);
    return arr;
}

bool inReadBool(const char *const text)
{
    printf("%s", text);
    int c;
    do
    {
        c = getchar();
    } while (c == ' ' || c == '\t' || c == '\n' || c == '\r');
    if (c == EOF)
        return false;
    const bool result = (c == 'y' || c == 'Y');
    while (c != EOF && c != '\n')
        c = getchar();
    return result;
}