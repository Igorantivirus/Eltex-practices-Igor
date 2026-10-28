#include <utils/DynamicArray.h>

#include "unity.h"

static int g_freeCounter;
static void freeCounter(void *const ptr)
{
    (void)ptr;
    ++g_freeCounter;
}

void setUp(void)
{
    g_freeCounter = 0;
}
void tearDown(void)
{
}

void testDaInit(void)
{
    DynamicArray arr = daInit(sizeof(int), NULL);
    TEST_ASSERT_EQUAL_UINT32(arr.oneElementSize, sizeof(int));
    TEST_ASSERT_NULL(arr.freeFunction);
    TEST_ASSERT_EQUAL_UINT32(arr.size, 0);
    TEST_ASSERT_EQUAL_UINT32(arr.capacity, 0);
    TEST_ASSERT_NULL(arr.array_);
}

void testDaInitReserve(void)
{
    DynamicArray arr = daInitReserve(sizeof(int), 10, NULL);
    TEST_ASSERT_EQUAL_UINT32(arr.oneElementSize, sizeof(int));
    TEST_ASSERT_EQUAL_UINT32(arr.size, 0);
    TEST_ASSERT_EQUAL_UINT32(arr.capacity, 10);
    TEST_ASSERT_NOT_NULL(arr.array_);
    daFree(&arr);
    TEST_ASSERT_NULL(arr.array_);
}

void testDaPushBackAndGetAt(void)
{
    DynamicArray arr = daInitType(int, NULL);

    daPushBackValue(&arr, 10, int);
    daPushBackValue(&arr, 20, int);
    daPushBackValue(&arr, 30, int);

    TEST_ASSERT_EQUAL_UINT32(arr.size, 3);
    TEST_ASSERT_EQUAL_INT(*(int *)daGetAt(&arr, 0), 10);
    TEST_ASSERT_EQUAL_INT(*(int *)daGetAt(&arr, 1), 20);
    TEST_ASSERT_EQUAL_INT(*(int *)daGetAt(&arr, 2), 30);
    TEST_ASSERT_NULL(daGetAt(&arr, 3));
    daFree(&arr);
}

void testDaInsert(void)
{
    DynamicArray arr = daInitType(int, NULL);
    daPushBackValue(&arr, 1, int);
    daPushBackValue(&arr, 3, int);

    int val = 2;
    daInsert(&arr, 1, &val);

    TEST_ASSERT_EQUAL_UINT32(arr.size, 3);
    TEST_ASSERT_EQUAL_INT(*(int *)daGetAt(&arr, 0), 1);
    TEST_ASSERT_EQUAL_INT(*(int *)daGetAt(&arr, 1), 2);
    TEST_ASSERT_EQUAL_INT(*(int *)daGetAt(&arr, 2), 3);
    daFree(&arr);
}

void testDaRemove(void)
{
    DynamicArray arr = daInitType(int, NULL);
    daPushBackValue(&arr, 1, int);
    daPushBackValue(&arr, 2, int);
    daPushBackValue(&arr, 3, int);

    daRemove(&arr, 1);

    TEST_ASSERT_EQUAL_UINT32(arr.size, 2);
    TEST_ASSERT_EQUAL_INT(*(int *)daGetAt(&arr, 0), 1);
    TEST_ASSERT_EQUAL_INT(*(int *)daGetAt(&arr, 1), 3);
    daFree(&arr);
}

void testDaSetAt(void)
{
    DynamicArray arr = daInitType(int, NULL);
    daPushBackValue(&arr, 1, int);
    daPushBackValue(&arr, 2, int);
    daPushBackValue(&arr, 3, int);

    int val = 42;
    daSetAt(&arr, 1, &val);

    TEST_ASSERT_EQUAL_INT(*(int *)daGetAt(&arr, 1), 42);
    TEST_ASSERT_EQUAL_INT(*(int *)daGetAt(&arr, 0), 1);
    TEST_ASSERT_EQUAL_INT(*(int *)daGetAt(&arr, 2), 3);
    daFree(&arr);
}

void testDaReserve(void)
{
    DynamicArray arr = daInitReserve(sizeof(int), 2, NULL);
    daPushBackValue(&arr, 1, int);
    daPushBackValue(&arr, 2, int);
    daReserve(&arr, 10);

    TEST_ASSERT_TRUE(arr.capacity >= 10);
    TEST_ASSERT_EQUAL_UINT32(arr.size, 2);
    TEST_ASSERT_EQUAL_INT(*(int *)daGetAt(&arr, 0), 1);
    TEST_ASSERT_EQUAL_INT(*(int *)daGetAt(&arr, 1), 2);
    daFree(&arr);
}

void testDaClear(void)
{
    DynamicArray arr = daInitType(int, freeCounter);
    daPushBackValue(&arr, 1, int);
    daPushBackValue(&arr, 2, int);
    daPushBackValue(&arr, 3, int);

    daClear(&arr);

    TEST_ASSERT_EQUAL_UINT32(arr.size, 0);
    TEST_ASSERT_EQUAL_INT(g_freeCounter, 3);
    daFree(&arr);
}

void testDaFreeCallsFreeFunction(void)
{
    DynamicArray arr = daInitType(int, freeCounter);
    daPushBackValue(&arr, 1, int);
    daPushBackValue(&arr, 2, int);

    daFree(&arr);

    TEST_ASSERT_EQUAL_INT(g_freeCounter, 2);
    TEST_ASSERT_NULL(arr.array_);
    TEST_ASSERT_EQUAL_UINT32(arr.size, 0);
}

void testDaRemoveInvalidIndex(void)
{
    // Удаление из пустого контейнера — должно быть no-op
    DynamicArray arr = daInitType(int, NULL);
    daRemove(&arr, 0);
    daRemove(&arr, 100);
    TEST_ASSERT_EQUAL_UINT32(arr.size, 0);
    daFree(&arr);

    // Удаление по индексу вне диапазона (index >= size) — no-op
    DynamicArray arr2 = daInitType(int, NULL);
    daPushBackValue(&arr2, 1, int);
    daPushBackValue(&arr2, 2, int);
    daRemove(&arr2, 2);    // == size, невалидно
    daRemove(&arr2, 100);  // далеко за пределами
    TEST_ASSERT_EQUAL_UINT32(arr2.size, 2);
    TEST_ASSERT_EQUAL_INT(*(int *)daGetAt(&arr2, 0), 1);
    TEST_ASSERT_EQUAL_INT(*(int *)daGetAt(&arr2, 1), 2);
    daFree(&arr2);
}

void testDaGetAtOutOfRange(void)
{
    // Взятие вне диапазона из пустого контейнера
    DynamicArray empty = daInitType(int, NULL);
    TEST_ASSERT_NULL(daGetAt(&empty, 0));
    daFree(&empty);

    // Взятие вне диапазона у непустого контейнера
    DynamicArray arr = daInitType(int, NULL);
    daPushBackValue(&arr, 1, int);
    daPushBackValue(&arr, 2, int);
    TEST_ASSERT_NULL(daGetAt(&arr, 2));
    TEST_ASSERT_NULL(daGetAt(&arr, 100));
    daFree(&arr);
}

void testDaClearRepeated(void)
{
    // Повторная очистка — вторая и дальше не меняют состояние
    DynamicArray arr = daInitType(int, freeCounter);
    daPushBackValue(&arr, 1, int);
    daPushBackValue(&arr, 2, int);

    daClear(&arr);
    daClear(&arr); // пустой контейнер — no-op

    TEST_ASSERT_EQUAL_UINT32(arr.size, 0);
    TEST_ASSERT_EQUAL_INT(g_freeCounter, 2); // вызовы только за первый даClear
    daFree(&arr);
}

void testDaInsertInvalidIndex(void)
{
    // Вставка по индексу вне диапазона (index > size) — no-op
    // (индекс == size допустим и вставляет в конец, поэтому проверяем строго > size)
    DynamicArray arr = daInitType(int, NULL);
    daPushBackValue(&arr, 1, int);
    daPushBackValue(&arr, 2, int);
    TEST_ASSERT_EQUAL_UINT32(arr.size, 2);

    int val = 99;
    daInsert(&arr, 3, &val);   // 3 > 2 — no-op
    daInsert(&arr, 100, &val); // далеко за пределами — no-op
    TEST_ASSERT_EQUAL_UINT32(arr.size, 2);
    TEST_ASSERT_EQUAL_INT(*(int *)daGetAt(&arr, 0), 1);
    TEST_ASSERT_EQUAL_INT(*(int *)daGetAt(&arr, 1), 2);
    daFree(&arr);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(testDaInit);
    RUN_TEST(testDaInitReserve);
    RUN_TEST(testDaPushBackAndGetAt);
    RUN_TEST(testDaInsert);
    RUN_TEST(testDaRemove);
    RUN_TEST(testDaSetAt);
    RUN_TEST(testDaReserve);
    RUN_TEST(testDaClear);
    RUN_TEST(testDaFreeCallsFreeFunction);
    RUN_TEST(testDaRemoveInvalidIndex);
    RUN_TEST(testDaGetAtOutOfRange);
    RUN_TEST(testDaClearRepeated);
    RUN_TEST(testDaInsertInvalidIndex);

    return UNITY_END();
}