#include <stat/Statistick.h>
#include <stdlib.h>
#include <ui/InputArgs.h>

#include "unity.h"

void setUp(void)
{
}
void tearDown(void)
{
}

void testFillStatistickInvalidArgs(void)
{
    // Невалидные аргументы (не всё заполнено) => false
    Statistick st;
    InputArgs args = iaInit();
    TEST_ASSERT_FALSE(fillStatistick(args, &st));
}

void testFillStatistickNullStat(void)
{
    // Нулевой указатель на статистику => false
    InputArgs args = iaInit();
    args.count = 10;
    args.gateway = 0xC0A80101u;
    args.mask = 24;
    args.countInputed = true;
    args.gatewayInputed = true;
    args.maskInputed = true;
    TEST_ASSERT_FALSE(fillStatistick(args, NULL));
}

void testFillStatistickAllInNetworkWhenMaskZero(void)
{
    // mask = 0: network = gateway & 0 = 0, и (0 & value) = 0 для любого value,
    // поэтому каждый сгенерированный IP попадает в сеть НЕЗАВИСИМО от случайных значений.
    // Это даёт точные, не зависящие от seed ожидания.
    InputArgs args = iaInit();
    args.count          = 5000;
    args.gateway        = 0x12345678u;
    args.mask           = 0;
    args.countInputed   = true;
    args.gatewayInputed = true;
    args.maskInputed    = true;

    Statistick st;
    TEST_ASSERT_TRUE(fillStatistick(args, &st));

    TEST_ASSERT_EQUAL_UINT32(st.fillCount, 5000);
    TEST_ASSERT_EQUAL_UINT32(st.countToMyNetwork, 5000);
    TEST_ASSERT_EQUAL_UINT32(st.countToOtherNetwork, 0);
    TEST_ASSERT_EQUAL_DOUBLE(st.countToMyNetworkPercents, 100.0);
    TEST_ASSERT_EQUAL_DOUBLE(st.countToOtherNetworkPercents, 0.0);
}

void testFillStatistickInvariants(void)
{
    // Общий случай (фиксированный seed): проверяем связности, которые должны
    // выполняться для любых входных данных, независимо от случайных значений.
    srand(2024);

    InputArgs args = iaInit();
    args.count          = 1000;
    args.gateway        = 0xC0A80101u;
    args.mask           = 24;
    args.countInputed   = true;
    args.gatewayInputed = true;
    args.maskInputed    = true;

    Statistick st;
    TEST_ASSERT_TRUE(fillStatistick(args, &st));

    TEST_ASSERT_EQUAL_UINT32(st.fillCount, 1000);
    // "в сети" + "вне сети" = общее число
    TEST_ASSERT_EQUAL_UINT32(st.countToMyNetwork + st.countToOtherNetwork, st.fillCount);
    // Проценты согласованы со счётом и складываются в 100
    TEST_ASSERT_DOUBLE_WITHIN(1e-9, 100. * st.countToMyNetwork / st.fillCount, st.countToMyNetworkPercents);
    TEST_ASSERT_DOUBLE_WITHIN(1e-9, 100. * st.countToOtherNetwork / st.fillCount, st.countToOtherNetworkPercents);
    TEST_ASSERT_DOUBLE_WITHIN(1e-9, 100.0, st.countToMyNetworkPercents + st.countToOtherNetworkPercents);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(testFillStatistickInvalidArgs);
    RUN_TEST(testFillStatistickNullStat);
    RUN_TEST(testFillStatistickAllInNetworkWhenMaskZero);
    RUN_TEST(testFillStatistickInvariants);

    return UNITY_END();
}