#include <rules/Changer.h>
#include <utils/StringView.h>

#include "unity.h"

// Утилита: применяет символную команду chmod к исходным правам.
// result == true -> изменение выполнено и *mode получило новое значение.
static bool apply(mode_t start, const char *const arg, mode_t *result)
{
    *result = start;
    return changeMode(result, svFromChars(arg));
}

void setUp(void)
{
}
void tearDown(void)
{
}

void testChangeModeOctal(void)
{
    mode_t mode = 0;

    TEST_ASSERT_TRUE(apply(0000, "755", &mode));
    TEST_ASSERT_EQUAL_UINT32(mode, 0755);

    TEST_ASSERT_TRUE(apply(0755, "000", &mode));
    TEST_ASSERT_EQUAL_UINT32(mode, 0);

    TEST_ASSERT_TRUE(apply(0644, "641", &mode));
    TEST_ASSERT_EQUAL_UINT32(mode, 0641);
}

void testChangeModeSymbolAdd(void)
{
    mode_t mode = 0;

    // +rwx — без категории, по POSIX значит 'a' (все)
    TEST_ASSERT_TRUE(apply(0000, "+rwx", &mode));
    TEST_ASSERT_EQUAL_UINT32(mode, 0777);

    // Конкатенация категорий: ug => u|g
    TEST_ASSERT_TRUE(apply(0644, "ug+rwx", &mode));
    TEST_ASSERT_EQUAL_UINT32(mode, 0774);

    // Пустой блок категорий — 'a'
    TEST_ASSERT_TRUE(apply(0644, "+r", &mode));
    TEST_ASSERT_EQUAL_UINT32(mode, 0755);

    // Конкатенация прав: +rw
    TEST_ASSERT_TRUE(apply(0000, "+rw", &mode));
    TEST_ASSERT_EQUAL_UINT32(mode, 0666);
}

void testChangeModeSymbolRemove(void)
{
    mode_t mode = 0;

    // -rwx по 'a'
    TEST_ASSERT_TRUE(apply(0777, "-rwx", &mode));
    TEST_ASSERT_EQUAL_UINT32(mode, 0);

    TEST_ASSERT_TRUE(apply(0755, "-x", &mode));
    TEST_ASSERT_EQUAL_UINT32(mode, 0644);

    TEST_ASSERT_TRUE(apply(0755, "u-x", &mode));
    TEST_ASSERT_EQUAL_UINT32(mode, 0655);

    TEST_ASSERT_TRUE(apply(0777, "go-w", &mode));
    TEST_ASSERT_EQUAL_UINT32(mode, 0755);
}

void testChangeModeSymbolSet(void)
{
    mode_t mode = 0;

    // = заменяет биты только в указанной категории
    TEST_ASSERT_TRUE(apply(0777, "u=r", &mode));
    TEST_ASSERT_EQUAL_UINT32(mode, 0477);

    TEST_ASSERT_TRUE(apply(0755, "go=rx", &mode));
    TEST_ASSERT_EQUAL_UINT32(mode, 0755);

    // Без категорий — 'a'
    TEST_ASSERT_TRUE(apply(0777, "=r", &mode));
    TEST_ASSERT_EQUAL_UINT32(mode, 0444);

    TEST_ASSERT_TRUE(apply(0000, "=rwx", &mode));
    TEST_ASSERT_EQUAL_UINT32(mode, 0777);
}

void testChangeModeCommaList(void)
{
    mode_t mode = 0;

    // Команды выполняются слева направо
    TEST_ASSERT_TRUE(apply(0644, "u+w,g-w", &mode));
    TEST_ASSERT_EQUAL_UINT32(mode, 0744);

    TEST_ASSERT_TRUE(apply(0000, "u+rwx,g+rx,o+x", &mode));
    TEST_ASSERT_EQUAL_UINT32(mode, 0751);

    // Команда "u," с пустым блоком прав после запятой — ошибка
    TEST_ASSERT_FALSE(apply(0755, "u,", &mode));
}

void testChangeModeErrors(void)
{
    mode_t mode = 0;

    // NULL-указатели
    TEST_ASSERT_FALSE(changeMode(NULL, svFromChars("755")));
    TEST_ASSERT_FALSE(changeMode(&mode, svFromChars(NULL)));

    // Пустая команда
    TEST_ASSERT_FALSE(apply(0755, "", &mode));

    // Неопознанные категории / операторы
    TEST_ASSERT_FALSE(apply(0755, "q+r", &mode));
    TEST_ASSERT_FALSE(apply(0755, "u~r", &mode));
    TEST_ASSERT_FALSE(apply(0755, "u r", &mode));
    TEST_ASSERT_FALSE(apply(0755, "u", &mode));

    // Пустой блок прав для "=" не допустим
    TEST_ASSERT_FALSE(apply(0755, "u=", &mode));

    // Не-восьмеричные "числовые" аргументы
    TEST_ASSERT_FALSE(apply(0755, "999", &mode));
    TEST_ASSERT_FALSE(apply(0755, "88", &mode));
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(testChangeModeOctal);
    RUN_TEST(testChangeModeSymbolAdd);
    RUN_TEST(testChangeModeSymbolRemove);
    RUN_TEST(testChangeModeSymbolSet);
    RUN_TEST(testChangeModeCommaList);
    RUN_TEST(testChangeModeErrors);

    return UNITY_END();
}
