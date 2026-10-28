#pragma once

typedef struct
{
    char** argv;
    int argc;
    int argCap_;
}Arguments;

Arguments initArgs(int argc, char** argv);
Arguments inReadArguments(const char* const tittle);

void freeArguments(Arguments* const args);