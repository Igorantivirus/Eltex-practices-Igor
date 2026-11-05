#pragma once

#include <stdio.h>

typedef struct
{
    char  *str;
    size_t size;
    size_t capacity;
} String;

String sInitEmpty();
String sInitCharArray(const char *const string);
String sInitString(const String *const other);

void sFree(String *self);
void sFreeVoid(void *self);

String sInput(FILE *, const char stop);
String sInputC();
String sInputCOutput(const char* const output, FILE* out);
String sInputCCO(const char* const output);

int sCompare(const String* const left, const String* const right);
int sCompareChars(const String* const left, const char*const right);