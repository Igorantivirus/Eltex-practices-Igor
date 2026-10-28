#include <rules/Changer.h>

#include <stdint.h>

#include <utils/StringView.h>

static bool isOct(StringView sv)
{
    if (sv.size != 3)
        return false;
    for (uint8_t i = 0; i < sv.size; ++i)
        if (sv.str[i] < '0' || sv.str[i] > '7')
            return false;
    return true;
}

static bool charInChars(const char c, const char *chars)
{
    for (; *chars != '\0'; ++chars)
        if (*chars == c)
            return true;
    return false;
}

static int getIndex(const char c, const char *chars)
{
    for (int index = 0; *chars != '\0'; ++index, ++chars)
        if (*chars == c)
            return index;
    return -1;
}

static int nextSubArg(mode_t *mode, StringView sv, int start)
{
    int pos = start;

    mode_t ugoa_mask = 0;
    while (pos < (int)sv.size && charInChars(sv.str[pos], "ugoa"))
    {
        char c = sv.str[pos];
        ugoa_mask |= (c == 'u' ? 0700 : (c == 'g' ? 0070 : (c == 'o' ? 0007 : 0777)));
        ++pos;
    }

    if (ugoa_mask == 0)
        ugoa_mask = 0777; /* пустой блок = 'a' (по POSIX) */

    if (pos >= (int)sv.size)
        return -1;
    char oper = sv.str[pos];
    if (!charInChars(oper, "+-="))
        return -1;
    ++pos;

    mode_t rwx_mask = 0;
    while (pos < (int)sv.size && charInChars(sv.str[pos], "rwx"))
    {
        char c = sv.str[pos];
        rwx_mask |= (c == 'r' ? 4 : (c == 'w' ? 2 : 1));
        ++pos;
    }

    mode_t effect = (rwx_mask | (rwx_mask << 3) | (rwx_mask << 6)) & ugoa_mask;

    if (oper == '+')
        *mode |= effect;
    else if (oper == '-')
        *mode &= (mode_t)~effect;
    else
        *mode = (mode_t)((*mode & ~ugoa_mask) | effect);

    return pos;
}
static void numberMask(mode_t *mode, const char *const str)
{
    mode_t mask = ((str[0] - '0') << 6) & ((str[1] - '0') << 3) & (str[2] - '0');

}

bool changeMode(mode_t *mode, StringView arg)
{
    if (mode == NULL || arg.str == NULL)
        return false;
    if (isOct(arg))
    {
        *mode = ((arg.str[0] - '0') << 6) | ((arg.str[1] - '0') << 3) | (arg.str[2] - '0');
        return true;
    }
    mode_t tmpMode = *mode;
    int pos = 0;
    while (true)
    {
        int end = nextSubArg(&tmpMode, arg, pos);
        pos = end + 1;
        if (end == -1)
            return false;
        if (end == (int)arg.size)
        {
            *mode = tmpMode;
            return true;
        }
        if (arg.str[end] != ',')
            return false;
        if (pos >= (int)arg.size)
            return false;
    }
    *mode = tmpMode;
    return true;
}