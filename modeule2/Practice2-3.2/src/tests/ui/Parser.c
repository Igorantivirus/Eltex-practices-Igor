#include <ui/Parser.h>

#include "unity.h"

void setUp(void)
{
}
void tearDown(void)
{
}

void testParseUInt32Basic(void)
{
    uint32_t val = 0;
    TEST_ASSERT_TRUE(parseUInt32("0", &val));
    TEST_ASSERT_EQUAL_UINT32(val, 0);

    TEST_ASSERT_TRUE(parseUInt32("12345", &val));
    TEST_ASSERT_EQUAL_UINT32(val, 12345);
}

void testParseUInt32Max(void)
{
    uint32_t val = 0;
    TEST_ASSERT_TRUE(parseUInt32("4294967295", &val));
    TEST_ASSERT_EQUAL_UINT32(val, UINT32_MAX);
}

void testParseUInt32Overflow(void)
{
    uint32_t val = 0;
    TEST_ASSERT_FALSE(parseUInt32("4294967296", &val)); // UINT32_MAX + 1
}

void testParseUInt32Invalid(void)
{
    uint32_t val = 0;
    bool res = parseUInt32("", &val);
    TEST_ASSERT_FALSE(res);        // пустая строка
    TEST_ASSERT_FALSE(parseUInt32("abc", &val));    // не числа
    TEST_ASSERT_FALSE(parseUInt32("12a", &val));    // хвост после чисел
    TEST_ASSERT_FALSE(parseUInt32("-5", &val));     // знак минус
    TEST_ASSERT_FALSE(parseUInt32("1 2", &val));    // пробел посередине
}

void testParseIpBasic(void)
{
    uint32_t ip = 0;
    const char *end = NULL;

    TEST_ASSERT_TRUE(parseIp("192.168.1.10", &ip, &end));
    TEST_ASSERT_EQUAL_UINT32(ip, 0xC0A8010A); // big-endian: 192.168.1.10
    TEST_ASSERT_EQUAL_STRING("", end);
}

void testParseIpLoopback(void)
{
    uint32_t ip = 0;
    const char *end = NULL;
    TEST_ASSERT_TRUE(parseIp("127.0.0.1", &ip, &end));
    TEST_ASSERT_EQUAL_UINT32(ip, 0x7F000001);
}

void testParseIpBounds(void)
{
    uint32_t ip = 0;
    const char *end = NULL;

    TEST_ASSERT_TRUE(parseIp("0.0.0.0", &ip, &end));
    TEST_ASSERT_EQUAL_UINT32(ip, 0x00000000);

    TEST_ASSERT_TRUE(parseIp("255.255.255.255", &ip, &end));
    TEST_ASSERT_EQUAL_UINT32(ip, 0xFFFFFFFF);
}

void testParseIpWithTrailing(void)
{
    // Разбор останавливается после 4-го октета; остаток возвращается через end
    uint32_t ip = 0;
    const char *end = NULL;
    TEST_ASSERT_TRUE(parseIp("1.2.3.4/24", &ip, &end));
    TEST_ASSERT_EQUAL_UINT32(ip, 0x01020304);
    TEST_ASSERT_EQUAL_STRING("/24", end);
}

void testParseIpInvalid(void)
{
    // parseIp читает ровно 4 октета и не проверяет хвост (его проверяет вызывающий).
    // Здесь ip == NULL безопасно: во всех этих случаях функция вернёт false раньше memcpy.
    TEST_ASSERT_FALSE(parseIp("256.1.1.1", NULL, NULL));   // > 255
    TEST_ASSERT_FALSE(parseIp("1.1.1", NULL, NULL));       // не 4 октета
    TEST_ASSERT_FALSE(parseIp("1.1.1.1.", NULL, NULL));    // хвостовая точка
    TEST_ASSERT_FALSE(parseIp("1.2.3.456", NULL, NULL));   // > 255
    TEST_ASSERT_FALSE(parseIp("1.2.3.04", NULL, NULL));    // ведущий ноль
    TEST_ASSERT_FALSE(parseIp("1.2.3. 4", NULL, NULL));    // пробел
    TEST_ASSERT_FALSE(parseIp("a.b.c.d", NULL, NULL));     // не цифры
    TEST_ASSERT_FALSE(parseIp("1..2.3", NULL, NULL));      // пустой октет
}

void testParseIpIgnoresTrailing(void)
{
    // Контракт: лишние октеты НЕ отбрасываются, а остаются в end (проверка — у вызывающего)
    uint32_t ip = 0;
    const char *end = NULL;
    TEST_ASSERT_TRUE(parseIp("1.2.3.4.5", &ip, &end));
    TEST_ASSERT_EQUAL_UINT32(ip, 0x01020304);
    TEST_ASSERT_EQUAL_STRING(".5", end);
}

void testParseIpNullArgs(void)
{
    uint32_t ip = 0;
    TEST_ASSERT_FALSE(parseIp(NULL, &ip, NULL));  // нет строки
    TEST_ASSERT_FALSE(parseIp("bad", NULL, NULL)); // нет места для результата (невалидный ввод)
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(testParseUInt32Basic);
    RUN_TEST(testParseUInt32Max);
    RUN_TEST(testParseUInt32Overflow);
    RUN_TEST(testParseUInt32Invalid);
    RUN_TEST(testParseIpBasic);
    RUN_TEST(testParseIpLoopback);
    RUN_TEST(testParseIpBounds);
    RUN_TEST(testParseIpWithTrailing);
    RUN_TEST(testParseIpInvalid);
    RUN_TEST(testParseIpIgnoresTrailing);
    RUN_TEST(testParseIpNullArgs);

    return UNITY_END();
}