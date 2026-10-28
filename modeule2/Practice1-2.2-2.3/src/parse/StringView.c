#include <stdlib.h>
#include <string.h>

#include <parse/StringView.h>

double svToDouble(StringView sv)
{
    if (!sv.str)
        return 0.0;
    char   buf[64]; // хватит для любого double
    size_t n = sv.size < sizeof(buf) - 1 ? sv.size : sizeof(buf) - 1;
    memcpy(buf, sv.str, n);
    buf[n] = '\0';
    return strtod(buf, NULL);
}

StringView svFromString(const String str)
{
    StringView sv;
    sv.str = str.str;
    sv.size = str.size;
    return sv;
}
