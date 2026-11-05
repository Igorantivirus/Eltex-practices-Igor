#include <cmath>
#include <libs/LibsHeader.h>

double mathCalcFunction(const double left, const double right)
{
    return pow(left, right);
}

const char     mathCalcOperationSymbol = '^';
const uint32_t mathCalcPriority = 3;