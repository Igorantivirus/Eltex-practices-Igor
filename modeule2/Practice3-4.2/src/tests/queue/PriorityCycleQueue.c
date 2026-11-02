#include <queue/PriorityCycleQueue.h>

#include "unity.h"

void setUp(void)
{
}
void tearDown(void)
{
}

void testPcqInit(void)
{
    PriorityCycleQueue q = pcqInit(0, 3);
    TEST_ASSERT_EQUAL_INT32(q.minPriority, 0);
    TEST_ASSERT_EQUAL_INT32(q.maxPriority, 3);
    TEST_ASSERT_EQUAL_UINT32(q.capacity, 4);
    TEST_ASSERT_EQUAL_UINT32(q.size, 0);
    TEST_ASSERT_NOT_NULL(q.queues);
    pcqFree(&q);
}

void testPcqInitReversed(void)
{
    // Аргументы в обратном порядке — min/max нормализуются
    PriorityCycleQueue q = pcqInit(10, 0);
    TEST_ASSERT_EQUAL_INT32(q.minPriority, 0);
    TEST_ASSERT_EQUAL_INT32(q.maxPriority, 10);
    TEST_ASSERT_EQUAL_UINT32(q.capacity, 11);
    pcqFree(&q);
}

void testPcqPushPopHighestPriorityFirst(void)
{
    PriorityCycleQueue q = pcqInit(0, 5);
    TEST_ASSERT_TRUE(pcqPush(&q, 1, 1));
    TEST_ASSERT_TRUE(pcqPush(&q, 3, 3));
    TEST_ASSERT_TRUE(pcqPush(&q, 2, 2));
    TEST_ASSERT_TRUE(pcqPush(&q, 5, 5));
    TEST_ASSERT_TRUE(pcqPush(&q, 0, 0));
    TEST_ASSERT_EQUAL_UINT32(q.size, 5);

    int elem = 0;
    int prio = -1;
    // Снимается строго по убыванию приоритета
    TEST_ASSERT_TRUE(pcqPop(&q, &elem, &prio));
    TEST_ASSERT_EQUAL_INT(prio, 5);
    TEST_ASSERT_EQUAL_INT(elem, 5);
    TEST_ASSERT_TRUE(pcqPop(&q, &elem, &prio));
    TEST_ASSERT_EQUAL_INT(prio, 3);
    TEST_ASSERT_TRUE(pcqPop(&q, &elem, &prio));
    TEST_ASSERT_EQUAL_INT(prio, 2);
    TEST_ASSERT_TRUE(pcqPop(&q, &elem, &prio));
    TEST_ASSERT_EQUAL_INT(prio, 1);
    TEST_ASSERT_TRUE(pcqPop(&q, &elem, &prio));
    TEST_ASSERT_EQUAL_INT(prio, 0);
    TEST_ASSERT_FALSE(pcqPop(&q, &elem, &prio)); // опустела
    TEST_ASSERT_EQUAL_UINT32(q.size, 0);

    pcqFree(&q);
}

void testPcqPushOutOfRange(void)
{
    PriorityCycleQueue q = pcqInit(2, 4);
    TEST_ASSERT_TRUE(pcqPush(&q, 1, 2));
    TEST_ASSERT_TRUE(pcqPush(&q, 1, 4));
    TEST_ASSERT_FALSE(pcqPush(&q, 1, 1)); // ниже минимума
    TEST_ASSERT_FALSE(pcqPush(&q, 1, 5)); // выше максимума
    TEST_ASSERT_FALSE(pcqPush(NULL, 1, 3));
    TEST_ASSERT_EQUAL_UINT32(q.size, 2);

    pcqFree(&q);
}

void testPcqPopEmpty(void)
{
    PriorityCycleQueue q = pcqInit(0, 3);
    int elem = 0;
    int prio = 0;
    TEST_ASSERT_FALSE(pcqPop(&q, &elem, &prio));
    TEST_ASSERT_FALSE(pcqPop(NULL, &elem, &prio));

    pcqFree(&q);
}

void testPcqPopPriority(void)
{
    PriorityCycleQueue q = pcqInit(0, 5);
    TEST_ASSERT_TRUE(pcqPush(&q, 10, 1));
    TEST_ASSERT_TRUE(pcqPush(&q, 20, 2));
    TEST_ASSERT_TRUE(pcqPush(&q, 30, 3));

    int elem = 0;
    // Точное попадание по заданному приоритету
    TEST_ASSERT_TRUE(pcqPopPriority(&q, &elem, 2));
    TEST_ASSERT_EQUAL_INT(elem, 20);
    TEST_ASSERT_TRUE(pcqPopPriority(&q, &elem, 1));
    TEST_ASSERT_EQUAL_INT(elem, 10);
    TEST_ASSERT_TRUE(pcqPopPriority(&q, &elem, 3));
    TEST_ASSERT_EQUAL_INT(elem, 30);
    TEST_ASSERT_EQUAL_UINT32(q.size, 0);

    pcqFree(&q);
}

void testPcqPopPriorityMissing(void)
{
    PriorityCycleQueue q = pcqInit(0, 5);
    TEST_ASSERT_TRUE(pcqPush(&q, 10, 1));

    int elem = 0;
    // Приоритет без элементов и приоритет вне диапазона
    TEST_ASSERT_FALSE(pcqPopPriority(&q, &elem, 4));
    TEST_ASSERT_FALSE(pcqPopPriority(&q, &elem, 6));
    TEST_ASSERT_FALSE(pcqPopPriority(NULL, &elem, 1));
    TEST_ASSERT_EQUAL_UINT32(q.size, 1); // ничего не извлечено

    pcqFree(&q);
}

void testPcqPopAtLeast(void)
{
    PriorityCycleQueue q = pcqInit(0, 5);
    TEST_ASSERT_TRUE(pcqPush(&q, 1, 1));
    TEST_ASSERT_TRUE(pcqPush(&q, 3, 3));
    TEST_ASSERT_TRUE(pcqPush(&q, 5, 5));

    int elem = 0;
    int prio = 0;
    // «Не ниже порога»: снимается высший доступный >= порога
    TEST_ASSERT_TRUE(pcqPopAtLeast(&q, &elem, 3, &prio));
    TEST_ASSERT_EQUAL_INT(prio, 5);
    TEST_ASSERT_EQUAL_INT(elem, 5);
    TEST_ASSERT_TRUE(pcqPopAtLeast(&q, &elem, 3, &prio));
    TEST_ASSERT_EQUAL_INT(prio, 3);
    TEST_ASSERT_TRUE(pcqPopAtLeast(&q, &elem, 0, &prio));
    TEST_ASSERT_EQUAL_INT(prio, 1);
    TEST_ASSERT_EQUAL_UINT32(q.size, 0);

    pcqFree(&q);
}

void testPcqPopAtLeastFifoWithinPriority(void)
{
    PriorityCycleQueue q = pcqInit(0, 5);
    TEST_ASSERT_TRUE(pcqPush(&q, 1, 2));
    TEST_ASSERT_TRUE(pcqPush(&q, 2, 2));
    TEST_ASSERT_TRUE(pcqPush(&q, 3, 2));

    int elem = 0;
    int prio = 0;
    // Внутри одного приоритета — FIFO
    TEST_ASSERT_TRUE(pcqPopAtLeast(&q, &elem, 2, &prio));
    TEST_ASSERT_EQUAL_INT(prio, 2);
    TEST_ASSERT_EQUAL_INT(elem, 1);
    TEST_ASSERT_TRUE(pcqPopAtLeast(&q, &elem, 2, &prio));
    TEST_ASSERT_EQUAL_INT(elem, 2);
    TEST_ASSERT_TRUE(pcqPopAtLeast(&q, &elem, 2, &prio));
    TEST_ASSERT_EQUAL_INT(elem, 3);
    TEST_ASSERT_FALSE(pcqPopAtLeast(&q, &elem, 2, &prio));

    pcqFree(&q);
}

void testPcqPopAtLeastNoMatch(void)
{
    PriorityCycleQueue q = pcqInit(0, 5);
    TEST_ASSERT_TRUE(pcqPush(&q, 10, 1));
    TEST_ASSERT_TRUE(pcqPush(&q, 20, 2));

    int elem = 0;
    int prio = 0;
    // Порог выше всех приоритетов — ничего не найдено
    TEST_ASSERT_FALSE(pcqPopAtLeast(&q, &elem, 3, &prio));
    TEST_ASSERT_FALSE(pcqPopAtLeast(NULL, &elem, 0, &prio));
    TEST_ASSERT_EQUAL_UINT32(q.size, 2);

    pcqFree(&q);
}

void testPcqFree(void)
{
    PriorityCycleQueue q = pcqInit(0, 5);
    TEST_ASSERT_TRUE(pcqPush(&q, 1, 3));
    TEST_ASSERT_TRUE(pcqPush(&q, 2, 1));

    pcqFree(&q);
    TEST_ASSERT_NULL(q.queues);
    TEST_ASSERT_EQUAL_UINT32(q.capacity, 0);
    TEST_ASSERT_EQUAL_UINT32(q.size, 0);

    pcqFree(NULL); // не должно падать
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(testPcqInit);
    RUN_TEST(testPcqInitReversed);
    RUN_TEST(testPcqPushPopHighestPriorityFirst);
    RUN_TEST(testPcqPushOutOfRange);
    RUN_TEST(testPcqPopEmpty);
    RUN_TEST(testPcqPopPriority);
    RUN_TEST(testPcqPopPriorityMissing);
    RUN_TEST(testPcqPopAtLeast);
    RUN_TEST(testPcqPopAtLeastFifoWithinPriority);
    RUN_TEST(testPcqPopAtLeastNoMatch);
    RUN_TEST(testPcqFree);

    return UNITY_END();
}

