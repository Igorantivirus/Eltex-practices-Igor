#pragma once

#include <stdbool.h>
#include <utils/DynamicArray.h>

typedef DynamicArray DynamicLibrarys;

bool dlInit(DynamicLibrarys *const dl, const char *const path);
bool dlFree(DynamicLibrarys *const dl);