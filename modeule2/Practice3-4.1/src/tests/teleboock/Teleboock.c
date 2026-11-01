#include <teleboock/Teleboock.h>
#include <teleboock/Contact.h>

#include <utils/String.h>

#include "unity.h"

void setUp(void)
{
}
void tearDown(void)
{
}

static Contact makeContact(const char *const firstName, const char *const secondName)
{
    Contact c;
    contInit(&c, sInitCharArray(firstName), sInitCharArray(secondName));
    return c;
}

void testTbInit(void)
{
    Teleboock tb;
    tbInit(&tb);

    TEST_ASSERT_EQUAL_UINT32(tb.oneElementSize, sizeof(Contact));
    TEST_ASSERT_NOT_NULL(tb.freeFunction);
    TEST_ASSERT_EQUAL_UINT32(tb.size, 0);
    TEST_ASSERT_EQUAL_UINT32(tb.capacity, 0);
    TEST_ASSERT_NULL(tb.array_);

    tbFree(&tb);
    TEST_ASSERT_NULL(tb.array_);
}

void testTbAddAndGetContact(void)
{
    Teleboock tb;
    tbInit(&tb);

    Contact c = makeContact("Ivan", "Petrov");
    daPushBack(&tb, &c);

    TEST_ASSERT_EQUAL_UINT32(tb.size, 1);
    Contact *stored = daGetAt(&tb, 0);
    TEST_ASSERT_EQUAL_STRING("Ivan", stored->firstName.str);
    TEST_ASSERT_EQUAL_STRING("Petrov", stored->secondName.str);

    tbFree(&tb);
}

void testTbMultipleContacts(void)
{
    Teleboock tb;
    tbInit(&tb);

    Contact c1 = makeContact("Ivan", "Petrov");
    Contact c2 = makeContact("Anna", "Smirnova");
    Contact c3 = makeContact("Petr", "Volkov");

    daPushBack(&tb, &c1);
    daPushBack(&tb, &c2);
    daPushBack(&tb, &c3);

    TEST_ASSERT_EQUAL_UINT32(tb.size, 3);
    TEST_ASSERT_EQUAL_STRING("Ivan", ((Contact *)daGetAt(&tb, 0))->firstName.str);
    TEST_ASSERT_EQUAL_STRING("Anna", ((Contact *)daGetAt(&tb, 1))->firstName.str);
    TEST_ASSERT_EQUAL_STRING("Petr", ((Contact *)daGetAt(&tb, 2))->firstName.str);

    tbFree(&tb);
}

void testTbRemoveContact(void)
{
    Teleboock tb;
    tbInit(&tb);

    Contact c1 = makeContact("Ivan", "Petrov");
    Contact c2 = makeContact("Anna", "Smirnova");
    Contact c3 = makeContact("Petr", "Volkov");

    daPushBack(&tb, &c1);
    daPushBack(&tb, &c2);
    daPushBack(&tb, &c3);

    daRemove(&tb, 1);

    TEST_ASSERT_EQUAL_UINT32(tb.size, 2);
    TEST_ASSERT_EQUAL_STRING("Ivan", ((Contact *)daGetAt(&tb, 0))->firstName.str);
    TEST_ASSERT_EQUAL_STRING("Petr", ((Contact *)daGetAt(&tb, 1))->firstName.str);

    tbFree(&tb);
}

void testTbFree(void)
{
    Teleboock tb;
    tbInit(&tb);

    Contact c1 = makeContact("Ivan", "Petrov");
    Contact c2 = makeContact("Anna", "Smirnova");
    daPushBack(&tb, &c1);
    daPushBack(&tb, &c2);

    tbFree(&tb);

    TEST_ASSERT_EQUAL_UINT32(tb.size, 0);
    TEST_ASSERT_TRUE(tb.array_ == NULL);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(testTbInit);
    RUN_TEST(testTbAddAndGetContact);
    RUN_TEST(testTbMultipleContacts);
    RUN_TEST(testTbRemoveContact);
    RUN_TEST(testTbFree);

    return UNITY_END();
}