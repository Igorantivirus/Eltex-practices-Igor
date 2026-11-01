#include <ui/InputArgs.h>

#include "unity.h"

void setUp(void)
{
}
void tearDown(void)
{
}

void testIaInit(void)
{
    InputArgs args = iaInit();

    TEST_ASSERT_FALSE(args.gatewayInputed);
    TEST_ASSERT_FALSE(args.maskInputed);
    TEST_ASSERT_FALSE(args.countInputed);
    TEST_ASSERT_FALSE(args.seedInputed);
    TEST_ASSERT_FALSE(iaIsValid(&args));
}

void testIaIsValidAllFilled(void)
{
    InputArgs args = iaInit();
    args.countInputed   = true;
    args.gatewayInputed = true;
    args.maskInputed    = true;

    TEST_ASSERT_TRUE(iaIsValid(&args));
}

void testIaIsValidMissingOne(void)
{
    // Отсутствие любого из обязательных аргументов => невалидно
    InputArgs args = iaInit();
    args.countInputed = true;
    args.gatewayInputed = true;
    TEST_ASSERT_FALSE(iaIsValid(&args)); // нет mask

    args.maskInputed = true;
    args.gatewayInputed = false;
    TEST_ASSERT_FALSE(iaIsValid(&args)); // нет gateway
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(testIaInit);
    RUN_TEST(testIaIsValidAllFilled);
    RUN_TEST(testIaIsValidMissingOne);

    return UNITY_END();
}