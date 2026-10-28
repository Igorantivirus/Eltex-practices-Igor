#include <rules/ModeParse.h>

#include <stdint.h>

ModeParseResult interpStringToMode(StringView sv, mode_t *out)
{
    if (sv.str == NULL || out == NULL)
        return ModeParseResultError;

    if (sv.size == 9)
    {
        mode_t     mode = 0;
        const char symbols[3] = {'r', 'w', 'x'};

        for (size_t i = 0; i < 3; i++) // Категории
        {
            for (uint8_t j = 0; j < 3; j++) // Права
            {
                char cur = sv.str[i * 3 + j];
                if (cur == symbols[j])
                    mode |= (mode_t)(1u << (8u - (i * 3u + j)));
                else if (cur != '-')
                    return ModeParseResultError;
            }
        }
        *out = mode;
        return ModeParseResultOk;
    }
    else if (sv.size == 3) // Восьмиричная запись
    {
        mode_t mode = 0;
        for (size_t i = 0; i < 3; i++)
        {
            char c = sv.str[i];
            if (c < '0' || c > '7')
                return ModeParseResultError;
            mode |= (mode_t)(c - '0') << ((2u - (unsigned)i) * 3u);
        }
        *out = mode;
        return ModeParseResultOk;
    }

    return ModeParseResultError;
}
