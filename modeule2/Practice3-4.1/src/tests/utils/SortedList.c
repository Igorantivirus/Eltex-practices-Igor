#include <utils/SortedList.h>

#include "unity.h"

static int g_freeCounter;
static void freeCounter(void *const ptr)
{
    (void)ptr;
    ++g_freeCounter;
}

static int cmpInt(const void *const left, const void *const right)
{
    const int *const l = left;
    const int *const r = right;
    return (*l > *r) - (*l < *r);
}

void setUp(void)
{
    g_freeCounter = 0;
}
void tearDown(void)
{
}

void testSlInit(void)
{
    SortedList list = slInit(sizeof(int), cmpInt, NULL);

    TEST_ASSERT_EQUAL_UINT32(list.oneElementSize, sizeof(int));
    TEST_ASSERT_NOT_NULL(list.compare);
    TEST_ASSERT_NULL(list.elemDestructor);
    TEST_ASSERT_EQUAL_UINT32(list.size, 0);
    TEST_ASSERT_NULL(list.root);
}

void testSlInsertAndGetAt(void)
{
    SortedList list = slInit(sizeof(int), cmpInt, NULL);

    int a = 30;
    int b = 10;
    int c = 20;
    slInsert(&list, &a);
    slInsert(&list, &b);
    slInsert(&list, &c);

    TEST_ASSERT_EQUAL_UINT32(list.size, 3);
    TEST_ASSERT_EQUAL_INT(*(int *)slGetAt(&list, 0), 10);
    TEST_ASSERT_EQUAL_INT(*(int *)slGetAt(&list, 1), 20);
    TEST_ASSERT_EQUAL_INT(*(int *)slGetAt(&list, 2), 30);
    TEST_ASSERT_NULL(slGetAt(&list, 3));
    slFree(&list);
}

void testSlInsertKeepsSorted(void)
{
    // Вставка в произвольном порядке всегда сохраняет отсортированность
    SortedList list = slInit(sizeof(int), cmpInt, NULL);

    const int values[] = {5, 3, 8, 1, 9, 2, 7, 4, 6, 0};
    for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); ++i)
        slInsert(&list, &values[i]);

    const int expected[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    TEST_ASSERT_EQUAL_UINT32(list.size, 10);
    for (size_t i = 0; i < sizeof(expected) / sizeof(expected[0]); ++i)
        TEST_ASSERT_EQUAL_INT(*(int *)slGetAt(&list, i), expected[i]);
    slFree(&list);
}

void testSlInsertDuplicates(void)
{
    // Равные значения не отбрасываются, а соседствуют в списке
    SortedList list = slInit(sizeof(int), cmpInt, NULL);

    int one = 1;
    int two = 2;
    int one2 = 1;
    slInsert(&list, &one);
    slInsert(&list, &two);
    slInsert(&list, &one2);

    TEST_ASSERT_EQUAL_UINT32(list.size, 3);
    TEST_ASSERT_EQUAL_INT(*(int *)slGetAt(&list, 0), 1);
    TEST_ASSERT_EQUAL_INT(*(int *)slGetAt(&list, 1), 1);
    TEST_ASSERT_EQUAL_INT(*(int *)slGetAt(&list, 2), 2);
    slFree(&list);
}

void testSlRemove(void)
{
    SortedList list = slInit(sizeof(int), cmpInt, NULL);

    int a = 1;
    int b = 2;
    int c = 3;
    slInsert(&list, &a);
    slInsert(&list, &b);
    slInsert(&list, &c);

    slRemove(&list, 1);

    TEST_ASSERT_EQUAL_UINT32(list.size, 2);
    TEST_ASSERT_EQUAL_INT(*(int *)slGetAt(&list, 0), 1);
    TEST_ASSERT_EQUAL_INT(*(int *)slGetAt(&list, 1), 3);
    slFree(&list);
}

void testSlRemoveInvalidIndex(void)
{
    // Удаление из пустого контейнера — no-op
    SortedList empty = slInit(sizeof(int), cmpInt, NULL);
    slRemove(&empty, 0);
    slRemove(&empty, 100);
    TEST_ASSERT_EQUAL_UINT32(empty.size, 0);
    slFree(&empty);

    // Удаление по индексу вне диапазона (index >= size) — no-op
    SortedList list = slInit(sizeof(int), cmpInt, NULL);
    int a = 1;
    int b = 2;
    slInsert(&list, &a);
    slInsert(&list, &b);
    slRemove(&list, 2);   // == size, невалидно
    slRemove(&list, 100); // далеко за пределами
    TEST_ASSERT_EQUAL_UINT32(list.size, 2);
    TEST_ASSERT_EQUAL_INT(*(int *)slGetAt(&list, 0), 1);
    TEST_ASSERT_EQUAL_INT(*(int *)slGetAt(&list, 1), 2);
    slFree(&list);
}

void testSlPopFrontAndBack(void)
{
    // Пустой контейнер — no-op
    SortedList empty = slInit(sizeof(int), cmpInt, NULL);
    slPopFront(&empty);
    slPopBack(&empty);
    TEST_ASSERT_EQUAL_UINT32(empty.size, 0);
    slFree(&empty);

    // Непустой контейнер — убираем с обоих концов
    SortedList list = slInit(sizeof(int), cmpInt, NULL);
    int a = 1;
    int b = 2;
    int c = 3;
    slInsert(&list, &a);
    slInsert(&list, &b);
    slInsert(&list, &c);

    slPopFront(&list); // убираем минимум (1)
    slPopBack(&list);  // убираем максимум (3)

    TEST_ASSERT_EQUAL_UINT32(list.size, 1);
    TEST_ASSERT_EQUAL_INT(*(int *)slGetAt(&list, 0), 2);
    slFree(&list);
}

void testSlClear(void)
{
    // Очистка освобождает все элементы (вызывает elemDestructor для каждого)
    SortedList list = slInit(sizeof(int), cmpInt, freeCounter);
    int a = 1;
    int b = 2;
    int c = 3;
    slInsert(&list, &a);
    slInsert(&list, &b);
    slInsert(&list, &c);

    slClear(&list);

    TEST_ASSERT_EQUAL_UINT32(list.size, 0);
    TEST_ASSERT_NULL(list.root);
    TEST_ASSERT_EQUAL_INT(g_freeCounter, 3);
    slFree(&list);
}

void testSlClearRepeated(void)
{
    // Повторная очистка — вторая и дальше не меняют состояние
    SortedList list = slInit(sizeof(int), cmpInt, freeCounter);
    int a = 1;
    int b = 2;
    slInsert(&list, &a);
    slInsert(&list, &b);

    slClear(&list);
    slClear(&list); // пустой контейнер — no-op

    TEST_ASSERT_EQUAL_UINT32(list.size, 0);
    TEST_ASSERT_EQUAL_INT(g_freeCounter, 2); // вызовы только за первый slClear
    slFree(&list);
}

void testSlFreeCallsDestructor(void)
{
    // Освобождение вызывается для каждого элемента
    SortedList list = slInit(sizeof(int), cmpInt, freeCounter);
    int a = 1;
    int b = 2;
    slInsert(&list, &a);
    slInsert(&list, &b);

    slFree(&list);

    TEST_ASSERT_EQUAL_INT(g_freeCounter, 2);
    TEST_ASSERT_NULL(list.root);
    TEST_ASSERT_EQUAL_UINT32(list.size, 0);
}

void testSlGetAtOutOfRange(void)
{
    // Взятие вне диапазона из пустого контейнера
    SortedList empty = slInit(sizeof(int), cmpInt, NULL);
    TEST_ASSERT_NULL(slGetAt(&empty, 0));
    slFree(&empty);

    // Взятие вне диапазона у непустого контейнера
    SortedList list = slInit(sizeof(int), cmpInt, NULL);
    int a = 1;
    int b = 2;
    slInsert(&list, &a);
    slInsert(&list, &b);
    TEST_ASSERT_NULL(slGetAt(&list, 2));
    TEST_ASSERT_NULL(slGetAt(&list, 100));
    slFree(&list);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(testSlInit);
    RUN_TEST(testSlInsertAndGetAt);
    RUN_TEST(testSlInsertKeepsSorted);
    RUN_TEST(testSlInsertDuplicates);
    RUN_TEST(testSlRemove);
    RUN_TEST(testSlRemoveInvalidIndex);
    RUN_TEST(testSlPopFrontAndBack);
    RUN_TEST(testSlClear);
    RUN_TEST(testSlClearRepeated);
    RUN_TEST(testSlFreeCallsDestructor);
    RUN_TEST(testSlGetAtOutOfRange);

    return UNITY_END();
}