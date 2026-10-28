#include <rules/ModeString.h>

#include "unity.h"

void setUp(void)
{
}
void tearDown(void)
{
}

void testModeToStringRWX(void)
{
    TEST_ASSERT_EQUAL_STRING("rwxrwxrwx", modeToStringRWX(0777).mode);
    TEST_ASSERT_EQUAL_STRING("rwxr-xr-x", modeToStringRWX(0755).mode);
    TEST_ASSERT_EQUAL_STRING("rw-r--r--", modeToStringRWX(0644).mode);
    TEST_ASSERT_EQUAL_STRING("rw-r----x", modeToStringRWX(0651).mode);
    TEST_ASSERT_EQUAL_STRING("r--r--r--", modeToStringRWX(0444).mode);
    TEST_ASSERT_EQUAL_STRING("---------", modeToStringRWX(0).mode);
}

void testModeToStringBin(void)
{
    TEST_ASSERT_EQUAL_STRING("111111111", modeToStringBin(0777).mode);
    TEST_ASSERT_EQUAL_STRING("111101101", modeToStringBin(0755).mode);
    TEST_ASSERT_EQUAL_STRING("110100100", modeToStringBin(0644).mode);
    TEST_ASSERT_EQUAL_STRING("110100001", modeToStringBin(0641).mode);
    TEST_ASSERT_EQUAL_STRING("100000101", modeToStringBin(0411).mode);
    TEST_ASSERT_EQUAL_STRING("000000000", modeToStringBin(0).mode);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(testModeToStringRWX);
    RUN_TEST(testModeToStringBin);

    return UNITY_END();
}
