#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <utils/String.h>

// private:

#define STRING_NEXT_SIZE(s) ((s) * 3 / 2 + 1)

static size_t charArrayLen(const char *const string)
{
    if (!string)
        return 0;
    size_t size = 0;
    while (string[size] != '\0')
        ++size;
    return size;
}
static void charArrayCopy(char *to, const char *const from, const size_t size)
{
    if (!to || !from)
        return;
    for (size_t i = 0; i < size; ++i)
        to[i] = from[i];
}
static void charArraySet(char *to, const char symbol, const size_t size)
{
    if (!to)
        return;
    for (size_t i = 0; i < size; ++i)
        to[i] = symbol;
}

// public:

String sInitEmpty()
{
    String res;
    res.size = res.capacity = 0;
    res.str = NULL;
    return res;
}

String sInitCharArray(const char *const string)
{
    String res = sInitEmpty();
    size_t size = charArrayLen(string);
    if (size == 0)
        return res;
    res.size = size;
    res.capacity = size + 1;
    res.str = malloc(res.capacity * sizeof(char));
    res.str[size] = '\0';
    charArrayCopy(res.str, string, size);
    return res;
}

String sInitString(const String *const other)
{
    String res = sInitEmpty();
    if (!other || other->size == 0)
        return res;
    res.size = other->size;
    res.capacity = other->capacity;
    res.str = malloc(res.capacity * sizeof(char));
    res.str[res.size] = '\0';
    charArrayCopy(res.str, other->str, res.size);
    return res;
}

void sFree(String *this)
{
    if (!this)
        return;
    free(this->str);
    this->str = NULL;
    this->capacity = this->size = 0;
}
void sFreeVoid(void *self)
{
    sFree(self);
}

String sInput(FILE *in, const char stop)
{
    String res = sInitEmpty();
    if (!in)
        return res;
    res.size = 0;
    res.capacity = 5;
    res.str = malloc(res.capacity * sizeof(char));
    for (char symbol = fgetc(in); symbol != stop && symbol != EOF; symbol = fgetc(in))
    {
        res.str[res.size++] = symbol;
        if (res.size == res.capacity)
        {
            size_t newCapacity = STRING_NEXT_SIZE(res.capacity);
            char  *newStr = realloc(res.str, newCapacity);
            if (newStr == NULL) // Ошибка
            {
                free(res.str);
                return sInitEmpty();
            }
            res.capacity = newCapacity;
            res.str = newStr;
        }
    }
    charArraySet(res.str + res.size, '\0', res.capacity - res.size);
    return res;
}

String sInputC()
{
    return sInput(stdin, '\n');
}
String sInputCOutput(const char *const output, FILE *out)
{
    fprintf(out, "%s", output);
    return sInputC();
}

String sInputCCO(const char *const output)
{
    fprintf(stdout, "%s", output);
    return sInputC();
}

int sCompare(const String* const left, const String* const right)
{
    if (left == right)  return 0;
    if (left == NULL)   return -1;
    if (right == NULL)  return 1;

    size_t leftLen  = left->str ? left->size : 0;
    size_t rightLen = right->str ? right->size : 0;
    size_t len = leftLen < rightLen ? leftLen : rightLen;

    if (len > 0)
    {
        int cmp = memcmp(left->str, right->str, len);
        if (cmp != 0)
            return cmp < 0 ? -1 : 1;
    }

    if (leftLen < rightLen) return -1;
    if (leftLen > rightLen) return 1;
    return 0;
}

int sCompareChars(const String* const left, const char* const right)
{
    if (left == NULL && right == NULL) return 0;
    if (left == NULL)  return -1;
    if (right == NULL) return 1;

    size_t leftLen  = left->str ? left->size : 0;
    size_t rightLen = strlen(right);
    size_t len = leftLen < rightLen ? leftLen : rightLen;

    if (len > 0)
    {
        int cmp = memcmp(left->str, right, len);
        if (cmp != 0)
            return cmp < 0 ? -1 : 1;
    }

    if (leftLen < rightLen) return -1;
    if (leftLen > rightLen) return 1;
    return 0;
}