#include <utils/Arguments.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char* inputChars(bool* stop)
{
    int size = 0;
    int capacity = 5;
    char* str = malloc(capacity * sizeof(char));
    char symbol;
    for (symbol = getchar(); symbol != ' ' && symbol != '\n' && symbol != EOF; symbol = getchar())
    {
        str[size++] = symbol;
        if (size == capacity)
        {
            size_t newCapacity = capacity * 3 / 2;
            char  *newStr = realloc(str, newCapacity);
            if (newStr == NULL) // Ошибка
            {
                for (int c = getchar(); c != '\n' && c != EOF; c = getchar());
                return str;
            }
            capacity = newCapacity;
            str = newStr;
        }
    }
    memset(str + size, '\0', capacity - size);
    *stop = symbol == '\n';
    return str;
}

Arguments initArgs(int argc, char **argv)
{
    Arguments args;
    args.argc = argc;
    args.argv = argv;
    args.argCap_ = -1;
    return args;
}
void freeArguments(Arguments *const args)
{
    if (!args || args->argCap_ <= 0)
        return;
    for(int i = 0; i < args->argc; ++i)
        free(args->argv[i]);
    free(args->argv);
    args->argc = args->argCap_ = 0;
    args->argv = NULL;
}
Arguments inReadArguments(const char *const tittle)
{
    Arguments args;
    args.argCap_ = 5;
    args.argc = 0;
    args.argv = malloc(sizeof(char *) * args.argCap_);
    bool stop;
    for (char* str = inputChars(&stop); !stop; str = inputChars(&stop))
    {
        args.argv[args.argc++] = str;
        if (args.argc > args.argCap_)
        {
            int    newCapacity = args.argCap_ * 3 / 2;
            char **newArgv = realloc(args.argv, newCapacity);
            if (!newArgv)
            {
                for (int c = getchar(); c != '\n' && c != EOF; c = getchar())
                    ;
                return args;
            }
            args.argCap_ = newCapacity;
            args.argv = newArgv;
        }
    }
    return args;
}