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
    TEST_ASSERT_NOT_NULL(tb.elemDestructor);
    TEST_ASSERT_EQUAL_UINT32(tb.size, 0);
    TEST_ASSERT_NULL(tb.root);

    tbFree(&tb);
    TEST_ASSERT_NULL(tb.root);
}

void testTbAddAndGetContact(void)
{
    Teleboock tb;
    tbInit(&tb);

    Contact c = makeContact("Ivan", "Petrov");
    slInsert(&tb, &c);

    TEST_ASSERT_EQUAL_UINT32(tb.size, 1);
    Contact *stored = slGetAt(&tb, 0);
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

    slInsert(&tb, &c1);
    slInsert(&tb, &c2);
    slInsert(&tb, &c3);

    TEST_ASSERT_EQUAL_UINT32(tb.size, 3);
    TEST_ASSERT_EQUAL_STRING("Anna", ((Contact *)slGetAt(&tb, 0))->firstName.str);
    TEST_ASSERT_EQUAL_STRING("Ivan", ((Contact *)slGetAt(&tb, 1))->firstName.str);
    TEST_ASSERT_EQUAL_STRING("Petr", ((Contact *)slGetAt(&tb, 2))->firstName.str);

    tbFree(&tb);
}

void testTbRemoveContact(void)
{
    Teleboock tb;
    tbInit(&tb);

    Contact c1 = makeContact("Ivan", "Petrov");
    Contact c2 = makeContact("Anna", "Smirnova");
    Contact c3 = makeContact("Petr", "Volkov");

    slInsert(&tb, &c1);
    slInsert(&tb, &c2);
    slInsert(&tb, &c3);

    slRemove(&tb, 1);

    TEST_ASSERT_EQUAL_UINT32(tb.size, 2);
    TEST_ASSERT_EQUAL_STRING("Anna", ((Contact *)slGetAt(&tb, 0))->firstName.str);
    TEST_ASSERT_EQUAL_STRING("Petr", ((Contact *)slGetAt(&tb, 1))->firstName.str);

    tbFree(&tb);
}

void testTbFree(void)
{
    Teleboock tb;
    tbInit(&tb);

    Contact c1 = makeContact("Ivan", "Petrov");
    Contact c2 = makeContact("Anna", "Smirnova");
    slInsert(&tb, &c1);
    slInsert(&tb, &c2);

    tbFree(&tb);

    TEST_ASSERT_EQUAL_UINT32(tb.size, 0);
    TEST_ASSERT_TRUE(tb.root == NULL);
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