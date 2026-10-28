#include <rules/ModeString.h>

#include <stddef.h>

ModeString modeToStringRWX(mode_t mode)
{
    ModeString res;
    for (size_t i = 0; i < 3; i++)
    {
        res.mode[i * 3 + 0] = (mode & ((mode_t)1u << (8u - (unsigned)i * 3u))) ? 'r' : '-';
        res.mode[i * 3 + 1] = (mode & ((mode_t)1u << (7u - (unsigned)i * 3u))) ? 'w' : '-';
        res.mode[i * 3 + 2] = (mode & ((mode_t)1u << (6u - (unsigned)i * 3u))) ? 'x' : '-';
    }
    res.mode[9] = '\0';
    return res;
}
ModeString modeToStringBin(mode_t mode)
{
    ModeString res;
    for (size_t i = 0; i < 3; i++)
    {
        res.mode[i * 3 + 0] = (mode & ((mode_t)1u << (8u - (unsigned)i * 3u))) ? '1' : '0';
        res.mode[i * 3 + 1] = (mode & ((mode_t)1u << (7u - (unsigned)i * 3u))) ? '1' : '0';
        res.mode[i * 3 + 2] = (mode & ((mode_t)1u << (6u - (unsigned)i * 3u))) ? '1' : '0';
    }
    res.mode[9] = '\0';
    return res;
}