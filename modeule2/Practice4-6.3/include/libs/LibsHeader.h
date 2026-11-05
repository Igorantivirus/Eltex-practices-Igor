#ifndef MATH_PLUGIN_H
#define MATH_PLUGIN_H

#include <stdint.h>

double mathCalcFunction(const double left, const double right);

extern const char     mathCalcOperationSymbol;
extern const uint32_t mathCalcPriority;

#endif
