#include <utils/Rand.h>
#include <utils/UInt32Array.h>

#include "unity.h"

void setUp(void)
{
}
void tearDown(void)
{
}

void testRandU32Deterministic(void)
{
    // Один и тот же seed => одинаковая последовательность
    srand(1234);
    uint32_t a1 = randU32();
    uint32_t a2 = randU32();
    uint32_t a3 = randU32();

    srand(1234);
    uint32_t b1 = randU32();
    uint32_t b2 = randU32();
    uint32_t b3 = randU32();

    TEST_ASSERT_EQUAL_UINT32(a1, b1);
    TEST_ASSERT_EQUAL_UINT32(a2, b2);
    TEST_ASSERT_EQUAL_UINT32(a3, b3);
}

void testUi32aFillRandAndFree(void)
{
    // Нулевой размер — корректно, элементов нет
    UInt32Array empty = ui32aFillRand(0);
    TEST_ASSERT_EQUAL_UINT32(empty.size, 0);
    ui32aFree(&empty);
    TEST_ASSERT_NULL(empty.array);

    // Ненулевой размер: size отражает количество элементов
    const uint32_t size = 1000;
    UInt32Array arr = ui32aFillRand(size);
    TEST_ASSERT_EQUAL_UINT32(arr.size, size);
    TEST_ASSERT_NOT_NULL(arr.array);

    ui32aFree(&arr);
    TEST_ASSERT_NULL(arr.array);
    TEST_ASSERT_EQUAL_UINT32(arr.size, 0);
}

void testUi32aFreeNullIsSafe(void)
{
    // Освобождение NULL — безопасное no-op
    ui32aFree(NULL);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(testRandU32Deterministic);
    RUN_TEST(testUi32aFillRandAndFree);
    RUN_TEST(testUi32aFreeNullIsSafe);

    return UNITY_END();
}