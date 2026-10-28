#include <rules/ModeParse.h>
#include <utils/StringView.h>

#include "unity.h"

void setUp(void)
{
}
void tearDown(void)
{
}

void testParseRWXFormat(void)
{
    mode_t mode = 0;

    TEST_ASSERT_EQUAL_INT(ModeParseResultOk, interpStringToMode(svFromChars("rwxrwxrwx"), &mode));
    TEST_ASSERT_EQUAL_UINT32(mode, 0777);

    TEST_ASSERT_EQUAL_INT(ModeParseResultOk, interpStringToMode(svFromChars("rwxr-xr-x"), &mode));
    TEST_ASSERT_EQUAL_UINT32(mode, 0755);

    TEST_ASSERT_EQUAL_INT(ModeParseResultOk, interpStringToMode(svFromChars("rw-r--r--"), &mode));
    TEST_ASSERT_EQUAL_UINT32(mode, 0644);

    TEST_ASSERT_EQUAL_INT(ModeParseResultOk, interpStringToMode(svFromChars("rw-r----x"), &mode));
    TEST_ASSERT_EQUAL_UINT32(mode, 0641);

    TEST_ASSERT_EQUAL_INT(ModeParseResultOk, interpStringToMode(svFromChars("---------"), &mode));
    TEST_ASSERT_EQUAL_UINT32(mode, 0);

    TEST_ASSERT_EQUAL_INT(ModeParseResultOk, interpStringToMode(svFromChars("r--r--r--"), &mode));
    TEST_ASSERT_EQUAL_UINT32(mode, 0444);

    TEST_ASSERT_EQUAL_INT(ModeParseResultError, interpStringToMode(svFromChars("r-----x-x"), &mode));
}

void testParseOctalFormat(void)
{
    mode_t mode = 0;

    TEST_ASSERT_EQUAL_INT(ModeParseResultOk, interpStringToMode(svFromChars("755"), &mode));
    TEST_ASSERT_EQUAL_UINT32(mode, 0755);

    TEST_ASSERT_EQUAL_INT(ModeParseResultOk, interpStringToMode(svFromChars("644"), &mode));
    TEST_ASSERT_EQUAL_UINT32(mode, 0644);

    TEST_ASSERT_EQUAL_INT(ModeParseResultOk, interpStringToMode(svFromChars("641"), &mode));
    TEST_ASSERT_EQUAL_UINT32(mode, 0641);

    TEST_ASSERT_EQUAL_INT(ModeParseResultOk, interpStringToMode(svFromChars("000"), &mode));
    TEST_ASSERT_EQUAL_UINT32(mode, 0);

    TEST_ASSERT_EQUAL_INT(ModeParseResultOk, interpStringToMode(svFromChars("137"), &mode));
    TEST_ASSERT_EQUAL_UINT32(mode, 0137);
}

void testParseErrors(void)
{
    mode_t mode = 0;

    // NULL-указатели
    TEST_ASSERT_EQUAL_INT(ModeParseResultError, interpStringToMode(svFromChars(NULL), &mode));
    TEST_ASSERT_EQUAL_INT(ModeParseResultError, interpStringToMode(svFromChars("755"), NULL));

    // Пустая строка
    TEST_ASSERT_EQUAL_INT(ModeParseResultError, interpStringToMode(svFromChars(""), &mode));

    // Некорректная длина
    TEST_ASSERT_EQUAL_INT(ModeParseResultError, interpStringToMode(svFromChars("75"), &mode));
    TEST_ASSERT_EQUAL_INT(ModeParseResultError, interpStringToMode(svFromChars("7555"), &mode));
    TEST_ASSERT_EQUAL_INT(ModeParseResultError, interpStringToMode(svFromChars("rw-r--r-x-"), &mode));

    // Окталия: буквы вместо цифр (в т.ч. "rwx" — это не RWX-запись)
    TEST_ASSERT_EQUAL_INT(ModeParseResultError, interpStringToMode(svFromChars("rwx"), &mode));
    TEST_ASSERT_EQUAL_INT(ModeParseResultError, interpStringToMode(svFromChars("800"), &mode));
    TEST_ASSERT_EQUAL_INT(ModeParseResultError, interpStringToMode(svFromChars("778"), &mode));

    // RWX-запись: недопустимые символы
    TEST_ASSERT_EQUAL_INT(ModeParseResultError, interpStringToMode(svFromChars("rwq---x-x"), &mode));
    TEST_ASSERT_EQUAL_INT(ModeParseResultError, interpStringToMode(svFromChars("rrw----x-"), &mode));
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(testParseRWXFormat);
    RUN_TEST(testParseOctalFormat);
    RUN_TEST(testParseErrors);

    return UNITY_END();
}
