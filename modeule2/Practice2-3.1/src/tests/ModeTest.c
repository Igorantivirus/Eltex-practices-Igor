#include <rules/Changer.h>
#include <rules/ModeParse.h>
#include <rules/ModeString.h>
#include <utils/StringView.h>

#include "unity.h"

// Полный цикл: строка прав -> mode_t -> представления -> изменение прав

#define INVALID_MODE_SENTINEL ((mode_t)0xFFFF)

// Строка (RWX или окт.) -> mode_t
static mode_t parseMode(const char *const s)
{
    mode_t mode = 0;
    if (interpStringToMode(svFromChars(s), &mode) != ModeParseResultOk)
        return INVALID_MODE_SENTINEL;
    return mode;
}

void setUp(void)
{
}
void tearDown(void)
{
}

void testFullCycleRoundTrip(void)
{
    // RWX-запись -> RWX-строка (идемпотентность)
    TEST_ASSERT_EQUAL_STRING("rw-r--r--", modeToStringRWX(parseMode("rw-r--r--")).mode);
    TEST_ASSERT_EQUAL_STRING("rwxr-xr-x", modeToStringRWX(parseMode("rwxr-xr-x")).mode);
    TEST_ASSERT_EQUAL_STRING("rwxrwxrwx", modeToStringRWX(parseMode("rwxrwxrwx")).mode);

    // RWX-запись -> окт.-строка
    TEST_ASSERT_EQUAL_INT(420, parseMode("644"));
    TEST_ASSERT_EQUAL_INT(493, parseMode("755"));

    // Битовое представление
    TEST_ASSERT_EQUAL_STRING("110100100", modeToStringBin(parseMode("rw-r--r--")).mode);
    TEST_ASSERT_EQUAL_STRING("111101101", modeToStringBin(parseMode("755")).mode);
}

void testFullCycleChangeAfterParse(void)
{
    // Изменение прав после разбора исходной строки
    mode_t mode = parseMode("rw-r--r--");
    TEST_ASSERT_TRUE(changeMode(&mode, svFromChars("g+w")));
    TEST_ASSERT_EQUAL_STRING("rw-rw-r--", modeToStringRWX(mode).mode);
    TEST_ASSERT_EQUAL_UINT32(mode, 0664);

    // Символьная команда поверх разобранных прав с очисткой
    mode = parseMode("755");
    TEST_ASSERT_TRUE(changeMode(&mode, svFromChars("u=rw,o-rwx,g=rx")));
    TEST_ASSERT_EQUAL_STRING("rw-r-x---", modeToStringRWX(mode).mode);
    TEST_ASSERT_EQUAL_UINT32(mode, 0650);
}

void testFullCycleChangeThenRepaint(void)
{
    // Окт. смена -> RWX-строка
    mode_t mode = 0644;
    TEST_ASSERT_TRUE(changeMode(&mode, svFromChars("755")));
    TEST_ASSERT_EQUAL_STRING("rwxr-xr-x", modeToStringRWX(mode).mode);
    TEST_ASSERT_EQUAL_STRING("111101101", modeToStringBin(mode).mode);

    // Цепочка: парсинг -> смена -> снова смена
    mode = parseMode("644");
    TEST_ASSERT_TRUE(changeMode(&mode, svFromChars("u+rwx,g+rx")));
    TEST_ASSERT_TRUE(changeMode(&mode, svFromChars("o-wx")));
    TEST_ASSERT_EQUAL_UINT32(mode, 0754);
    TEST_ASSERT_EQUAL_STRING("rwxr-xr--", modeToStringRWX(mode).mode);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(testFullCycleRoundTrip);
    RUN_TEST(testFullCycleChangeAfterParse);
    RUN_TEST(testFullCycleChangeThenRepaint);

    return UNITY_END();
}
