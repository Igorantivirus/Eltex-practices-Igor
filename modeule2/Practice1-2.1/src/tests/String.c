#include <string.h>
#include <utils/String.h>

#include "unity.h"

void setUp(void)
{
}
void tearDown(void)
{
}

void testEmptyString(void)
{
    String str = sInitEmpty();

    TEST_ASSERT_TRUE(str.size == 0);
    TEST_ASSERT_TRUE(str.capacity == 0);
    TEST_ASSERT_TRUE(str.str == NULL);
    sFree(&str);
}

void testCharArrayFrom(const char *const cstring)
{
    String s = sInitCharArray(cstring);
    size_t len = strlen(cstring);
    if (len == 0)
        TEST_ASSERT_TRUE(s.str == NULL);
    else
        TEST_ASSERT_EQUAL_STRING(s.str, cstring);
    TEST_ASSERT_TRUE(s.capacity >= s.size);
    TEST_ASSERT_TRUE(s.capacity >= len);
    TEST_ASSERT_TRUE(s.size == len);
    sFree(&s);
}
void testCharArrayString(void)
{
    testCharArrayFrom("Hello, world!");
    testCharArrayFrom("");
    testCharArrayFrom("ToooLoooooongTEXT_TEST_____________________________");
    testCharArrayFrom("Enters\n\n\t  \t   \n\n\n");
    testCharArrayFrom("NullPtr\0AndNext");
}

void testCopyStringFrom(const char *const cstring)
{
    String s1 = sInitCharArray(cstring);
    String s2 = sInitString(&s1);
    TEST_ASSERT_TRUE(s1.size == s2.size);
    TEST_ASSERT_TRUE(s1.capacity >= s2.capacity);
    TEST_ASSERT_EQUAL_STRING(s1.str, s2.str);
    sFree(&s1);
    sFree(&s2);
}
void testCopyString(void)
{
    testCopyStringFrom("Hello, world!");
    testCopyStringFrom("");
    testCopyStringFrom("ToooLoooooongTEXT_TEST_____________________________");
    testCopyStringFrom("Enters\n\n\t  \t   \n\n\n");
    testCopyStringFrom("NullPtr\0AndNext");
}

void testFreeStringFrom(const char *const cstring)
{
    String s = sInitCharArray(cstring);
    sFree(&s);
    TEST_ASSERT_TRUE(s.size == 0);
    TEST_ASSERT_TRUE(s.capacity == 0);
    TEST_ASSERT_TRUE(s.str == NULL);
}
void testFreeString(void)
{
    testFreeStringFrom("Hello, world!");
    testFreeStringFrom("");
    testFreeStringFrom("ToooLoooooongTEXT_TEST_____________________________");
    testFreeStringFrom("Enters\n\n\t  \t   \n\n\n");
    testFreeStringFrom("NullPtr\0AndNext");
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(testEmptyString);
    RUN_TEST(testCharArrayString);
    RUN_TEST(testCopyString);
    RUN_TEST(testFreeString);

    return UNITY_END();
}