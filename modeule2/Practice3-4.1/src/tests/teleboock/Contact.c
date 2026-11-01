#include <teleboock/Contact.h>

#include <utils/String.h>

#include "unity.h"

void setUp(void)
{
}
void tearDown(void)
{
}

static Contact makeContact(void)
{
    Contact c;
    contInit(&c, sInitCharArray("Ivan"), sInitCharArray("Petrov"));
    return c;
}

void testContInit(void)
{
    Contact c;
    contInit(&c, sInitCharArray("Ivan"), sInitCharArray("Petrov"));

    TEST_ASSERT_EQUAL_STRING("Ivan", c.firstName.str);
    TEST_ASSERT_EQUAL_STRING("Petrov", c.secondName.str);

    TEST_ASSERT_TRUE(c.thirdName.size == 0);
    TEST_ASSERT_TRUE(c.workPlace.size == 0);
    TEST_ASSERT_TRUE(c.workPost.size == 0);

    TEST_ASSERT_TRUE(c.phonesNumbers.size == 0);
    TEST_ASSERT_TRUE(c.emails.size == 0);
    TEST_ASSERT_TRUE(c.webLinks.size == 0);
    TEST_ASSERT_TRUE(c.profiles.size == 0);

    constFreeContact(&c);
}

void testContInitWithInfo(void)
{
    Contact c;
    contInitWithInfo(&c,
                     sInitCharArray("Ivan"),
                     sInitCharArray("Petrov"),
                     sInitCharArray("Ivanovich"),
                     sInitCharArray("Company"),
                     sInitCharArray("Engineer"));

    TEST_ASSERT_EQUAL_STRING("Ivan", c.firstName.str);
    TEST_ASSERT_EQUAL_STRING("Petrov", c.secondName.str);
    TEST_ASSERT_EQUAL_STRING("Ivanovich", c.thirdName.str);
    TEST_ASSERT_EQUAL_STRING("Company", c.workPlace.str);
    TEST_ASSERT_EQUAL_STRING("Engineer", c.workPost.str);

    constFreeContact(&c);
}

void testContactPhonesNumbers(void)
{
    Contact c = makeContact();

    String phone = sInitCharArray("+71234567890");
    daPushBack(&c.phonesNumbers, &phone);

    TEST_ASSERT_EQUAL_UINT32(c.phonesNumbers.size, 1);
    String *stored = daGetAt(&c.phonesNumbers, 0);
    TEST_ASSERT_EQUAL_STRING("+71234567890", stored->str);

    constFreeContact(&c);
}

void testContactEmails(void)
{
    Contact c = makeContact();

    String email = sInitCharArray("ivan@example.com");
    daPushBack(&c.emails, &email);

    TEST_ASSERT_EQUAL_UINT32(c.emails.size, 1);
    String *stored = daGetAt(&c.emails, 0);
    TEST_ASSERT_EQUAL_STRING("ivan@example.com", stored->str);

    constFreeContact(&c);
}

void testContactWebLinksAndProfiles(void)
{
    Contact c = makeContact();

    String link = sInitCharArray("https://github.com/ivan");
    daPushBack(&c.webLinks, &link);

    String profile = sInitCharArray("@ivan_telegram");
    daPushBack(&c.profiles, &profile);

    TEST_ASSERT_EQUAL_UINT32(c.webLinks.size, 1);
    TEST_ASSERT_EQUAL_UINT32(c.profiles.size, 1);

    TEST_ASSERT_EQUAL_STRING("https://github.com/ivan", ((String *)daGetAt(&c.webLinks, 0))->str);
    TEST_ASSERT_EQUAL_STRING("@ivan_telegram", ((String *)daGetAt(&c.profiles, 0))->str);

    constFreeContact(&c);
}

void testContFree(void)
{
    Contact c = makeContact();

    constFreeContact(&c);

    TEST_ASSERT_TRUE(c.firstName.size == 0);
    TEST_ASSERT_TRUE(c.firstName.str == NULL);
    TEST_ASSERT_TRUE(c.secondName.size == 0);
    TEST_ASSERT_TRUE(c.secondName.str == NULL);
    TEST_ASSERT_TRUE(c.phonesNumbers.size == 0);
    TEST_ASSERT_TRUE(c.phonesNumbers.array_ == NULL);
    TEST_ASSERT_TRUE(c.emails.array_ == NULL);
    TEST_ASSERT_TRUE(c.webLinks.array_ == NULL);
    TEST_ASSERT_TRUE(c.profiles.array_ == NULL);
}

void testContFreeWithFillData(void)
{
    Contact c = makeContact();

    String phone = sInitCharArray("+70001112233");
    daPushBack(&c.phonesNumbers, &phone);
    String email = sInitCharArray("test@test.ru");
    daPushBack(&c.emails, &email);

    constFreeContact(&c);

    TEST_ASSERT_TRUE(c.firstName.str == NULL);
    TEST_ASSERT_TRUE(c.secondName.str == NULL);
    TEST_ASSERT_TRUE(c.thirdName.str == NULL);
    TEST_ASSERT_TRUE(c.workPlace.str == NULL);
    TEST_ASSERT_TRUE(c.workPost.str == NULL);
    TEST_ASSERT_TRUE(c.phonesNumbers.array_ == NULL);
    TEST_ASSERT_TRUE(c.emails.array_ == NULL);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(testContInit);
    RUN_TEST(testContInitWithInfo);
    RUN_TEST(testContactPhonesNumbers);
    RUN_TEST(testContactEmails);
    RUN_TEST(testContactWebLinksAndProfiles);
    RUN_TEST(testContFree);
    RUN_TEST(testContFreeWithFillData);

    return UNITY_END();
}