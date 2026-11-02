#include <queue/CycleQueue.h>

#include "unity.h"

void setUp(void)
{
}
void tearDown(void)
{
}

static bool pushAll(CycleQueue *q, int count, int from)
{
    for (int i = 0; i < count; ++i)
        if (!cqPush(q, from + i))
            return false;
    return true;
}

void testCqInit(void)
{
    CycleQueue q;
    TEST_ASSERT_TRUE(cqInit(&q, 4));

    TEST_ASSERT_EQUAL_UINT32(q.capacity, 4);
    TEST_ASSERT_EQUAL_UINT32(q.size, 0);
    TEST_ASSERT_EQUAL_UINT32(q.headElem, 0);
    TEST_ASSERT_EQUAL_UINT32(q.tailElem, 0);
    TEST_ASSERT_NOT_NULL(q.queue);

    cqFree(&q);
    TEST_ASSERT_NULL(q.queue);
    TEST_ASSERT_EQUAL_UINT32(q.capacity, 0);
}

void testCqInitNull(void)
{
    TEST_ASSERT_FALSE(cqInit(NULL, 4));
}

void testCqPushPopFifo(void)
{
    CycleQueue q;
    cqInit(&q, 4);

    TEST_ASSERT_TRUE(cqPush(&q, 10));
    TEST_ASSERT_TRUE(cqPush(&q, 20));
    TEST_ASSERT_TRUE(cqPush(&q, 30));
    TEST_ASSERT_EQUAL_UINT32(q.size, 3);

    int v = -1;
    TEST_ASSERT_TRUE(cqPop(&q, &v));
    TEST_ASSERT_EQUAL_INT(v, 10);
    TEST_ASSERT_TRUE(cqPop(&q, &v));
    TEST_ASSERT_EQUAL_INT(v, 20);
    TEST_ASSERT_TRUE(cqPop(&q, &v));
    TEST_ASSERT_EQUAL_INT(v, 30);
    TEST_ASSERT_EQUAL_UINT32(q.size, 0);

    // Пустая очередь: head/tail сбрасываются в ноль
    TEST_ASSERT_EQUAL_UINT32(q.headElem, 0);
    TEST_ASSERT_EQUAL_UINT32(q.tailElem, 0);

    cqFree(&q);
}

void testCqPopEmpty(void)
{
    CycleQueue q;
    cqInit(&q, 4);

    int v = 999;
    TEST_ASSERT_FALSE(cqPop(&q, &v));
    TEST_ASSERT_EQUAL_INT(v, 999); // при неудаче значение не меняется
    TEST_ASSERT_FALSE(cqPop(NULL, &v));
    TEST_ASSERT_FALSE(cqPop(&q, NULL));

    cqFree(&q);
}

void testCqNullSafety(void)
{
    TEST_ASSERT_FALSE(cqPush(NULL, 5));
    int v = 0;
    TEST_ASSERT_FALSE(cqPop(NULL, &v));
    cqFree(NULL); // не должно падать
}

void testCqGrowthFifo(void)
{
    CycleQueue q;
    cqInit(&q, 4);

    const int count = 20; // много больше стартовой ёмкости 4
    TEST_ASSERT_TRUE(pushAll(&q, count, 0));
    TEST_ASSERT_EQUAL_UINT32(q.size, (uint32_t)count);
    TEST_ASSERT_TRUE(q.capacity >= (uint32_t)count);

    // FIFO-порядок сохраняется даже после нескольких расширений
    int v = 0;
    for (int i = 0; i < count; ++i)
    {
        TEST_ASSERT_TRUE(cqPop(&q, &v));
        TEST_ASSERT_EQUAL_INT(v, i);
    }
    TEST_ASSERT_EQUAL_UINT32(q.size, 0);

    cqFree(&q);
}

void testCqWraparound(void)
{
    // Чередование push/pop сдвигает «голову» и «хвост» по кругу,
    // затем дозаполнение заставляет очередь расшириться (tail != 0).
    CycleQueue q;
    cqInit(&q, 4);

    for (int i = 0; i < 6; ++i)
    {
        int v = 0;
        TEST_ASSERT_TRUE(cqPush(&q, 100 + i));
        TEST_ASSERT_TRUE(cqPop(&q, &v));
    }
    TEST_ASSERT_EQUAL_UINT32(q.size, 0);

    // После «раскрутки» хвоста продолжаем добавлять/снимать подряд
    TEST_ASSERT_TRUE(pushAll(&q, 8, 0));
    int v = 0;
    for (int i = 0; i < 8; ++i)
    {
        TEST_ASSERT_TRUE(cqPop(&q, &v));
        TEST_ASSERT_EQUAL_INT(v, i);
    }
    TEST_ASSERT_EQUAL_UINT32(q.size, 0);

    cqFree(&q);
}

void testCqFree(void)
{
    CycleQueue q;
    cqInit(&q, 4);
    TEST_ASSERT_TRUE(cqPush(&q, 7));

    cqFree(&q);
    TEST_ASSERT_NULL(q.queue);
    TEST_ASSERT_EQUAL_UINT32(q.capacity, 0);
    TEST_ASSERT_EQUAL_UINT32(q.size, 0);
    TEST_ASSERT_EQUAL_UINT32(q.headElem, 0);
    TEST_ASSERT_EQUAL_UINT32(q.tailElem, 0);
}

void testCqFreeNull(void)
{
    cqFree(NULL); // не должно падать
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(testCqInit);
    RUN_TEST(testCqInitNull);
    RUN_TEST(testCqPushPopFifo);
    RUN_TEST(testCqPopEmpty);
    RUN_TEST(testCqNullSafety);
    RUN_TEST(testCqGrowthFifo);
    RUN_TEST(testCqWraparound);
    RUN_TEST(testCqFree);
    RUN_TEST(testCqFreeNull);

    return UNITY_END();
}
