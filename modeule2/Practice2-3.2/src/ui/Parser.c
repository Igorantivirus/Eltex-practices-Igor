#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <ui/Parser.h>

// public:

bool parseUInt32(const char *arg, uint32_t *val)
{
    char              *end;
    unsigned long long input = strtoull(arg, &end, 10);
    if (*end != '\0' || input > (unsigned long long)UINT32_MAX)
        return false;
    *val = input;
    return true;
}

bool parseIp(const char *str, uint32_t *ip, const char **end)
{
    if (!str || !ip)
        return false;

    uint8_t     ipRes[4];
    const char *p = str;

    for (int i = 0; i < 4; ++i)
    {
        if (*p < '0' || *p > '9')
            return false;

        if (p[0] == '0' && p[1] >= '0' && p[1] <= '9')
            return false;

        unsigned v = 0;
        int      n = 0;
        while (*p >= '0' && *p <= '9')
        {
            if (++n > 3) // > 3 цифр — заведомо > 255
                return false;
            v = v * 10 + (unsigned)(*p - '0');
            ++p;
        }
        if (v > 255)
            return false;
        ipRes[i] = (uint8_t)v;

        if (i < 3)
        {
            if (*p != '.') // после октетов 1..3 нужна точка
                return false;
            ++p;
        }
    }

    if (end)
        *end = p;
    memcpy(ip, ipRes, sizeof(ipRes));
    return true;
}
