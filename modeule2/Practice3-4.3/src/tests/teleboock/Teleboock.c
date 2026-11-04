#include <teleboock/Teleboock.h>
#include <teleboock/Contact.h>

#include <utils/String.h>
#include <utils/BalancedTree.h> // ИЗМЕНЕНО: добавлено

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

    TEST_ASSERT_EQUAL_UINT32(tb.valueSize, sizeof(Contact)); // ИЗМЕНЕНО: было oneElementSize
    TEST_ASSERT_NOT_NULL(tb.freeFunction);                     // ИЗМЕНЕНО: было elemDestructor
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
    bltInsert(&tb, &c);

    TEST_ASSERT_EQUAL_UINT32(tb.size, 1);
    const Contact *stored = bltGetAt(&tb, 0); // ИЗМЕНЕНО: было slGetAt
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

    bltInsert(&tb, &c1);
    bltInsert(&tb, &c2);
    bltInsert(&tb, &c3);

    TEST_ASSERT_EQUAL_UINT32(tb.size, 3);
    // ИЗМЕНЕНО: сортировка теперь по фамилии: Petrov, Smirnova, Volkov
    TEST_ASSERT_EQUAL_STRING("Ivan", ((const Contact *)bltGetAt(&tb, 0))->firstName.str);
    TEST_ASSERT_EQUAL_STRING("Anna", ((const Contact *)bltGetAt(&tb, 1))->firstName.str);
    TEST_ASSERT_EQUAL_STRING("Petr", ((const Contact *)bltGetAt(&tb, 2))->firstName.str);

    tbFree(&tb);
}

void testTbRemoveContact(void)
{
    Teleboock tb;
    tbInit(&tb);

    Contact c1 = makeContact("Ivan", "Petrov");
    Contact c2 = makeContact("Anna", "Smirnova");
    Contact c3 = makeContact("Petr", "Volkov");

    bltInsert(&tb, &c1);
    bltInsert(&tb, &c2);
    bltInsert(&tb, &c3);

    bltRemove(&tb, bltGetAt(&tb, 1)); // ИЗМЕНЕНО: было slRemove(&tb, 1); удаляется Smirnova Anna

    TEST_ASSERT_EQUAL_UINT32(tb.size, 2);
    TEST_ASSERT_EQUAL_STRING("Ivan", ((const Contact *)bltGetAt(&tb, 0))->firstName.str);
    TEST_ASSERT_EQUAL_STRING("Petr", ((const Contact *)bltGetAt(&tb, 1))->firstName.str);

    tbFree(&tb);
}

void testTbFree(void)
{
    Teleboock tb;
    tbInit(&tb);

    Contact c1 = makeContact("Ivan", "Petrov");
    Contact c2 = makeContact("Anna", "Smirnova");
    bltInsert(&tb, &c1);
    bltInsert(&tb, &c2);

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