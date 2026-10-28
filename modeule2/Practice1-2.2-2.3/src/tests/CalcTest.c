#include "unity.h"

#include "calc/CalcNodes.h"
#include "calc/CalcTree.h"
#include "parse/Parser.h"
#include "parse/StringView.h"
#include <math.h>
#include <ui/BaseCalcBTInit.h>

// Полный цикл: разбор выражения -> построение дерева -> вычисление
double test(char *const expression)
{
    CallTree tree;
    ctInit(&tree, NULL, initDefaultBinOpTree());
    Node *node = parse(svFromCharsArray(expression), &tree.info);
    ctChangeNodeRoot(&tree, node);
    double resultNum = calc(&tree);
    ctFree(&tree);
    return resultNum;
}

void setUp(void)
{
}
void tearDown(void)
{
}

void testFullCycleNumbers(void)
{
    // Целые и десятичные числа (в т.ч. без цифр перед точкой)
    TEST_ASSERT_EQUAL_DOUBLE(test("0"), 0);
    TEST_ASSERT_EQUAL_DOUBLE(test("42"), 42);
    TEST_ASSERT_EQUAL_DOUBLE(test("3.5"), 3.5);
    TEST_ASSERT_EQUAL_DOUBLE(test(".5"), 0.5);
    TEST_ASSERT_EQUAL_DOUBLE(test("100.250"), 100.25);
}

void testFullCycleBasicOps(void)
{
    TEST_ASSERT_EQUAL_DOUBLE(test("1 + 2"), 3);
    TEST_ASSERT_EQUAL_DOUBLE(test("10 - 4"), 6);
    TEST_ASSERT_EQUAL_DOUBLE(test("6 * 7"), 42);
    TEST_ASSERT_EQUAL_DOUBLE(test("8 / 2"), 4);
    TEST_ASSERT_EQUAL_DOUBLE(test("7 / 2"), 3.5);
}

void testFullCycleWhitespace(void)
{
    TEST_ASSERT_EQUAL_DOUBLE(test(" 1 + 2 "), 3);
    TEST_ASSERT_EQUAL_DOUBLE(test("\t2*3\t"), 6);
    TEST_ASSERT_EQUAL_DOUBLE(test("1\n+\n2\n"), 3);
}

void testFullCyclePrecedenceAndAssociativity(void)
{
    TEST_ASSERT_EQUAL_DOUBLE(test("2 + 3 * 4"), 14);
    TEST_ASSERT_EQUAL_DOUBLE(test("2 * 3 + 4"), 10);
    TEST_ASSERT_EQUAL_DOUBLE(test("10 - 2 - 3"), 5); // левая ассоциативность
    TEST_ASSERT_EQUAL_DOUBLE(test("10 / 2 / 5"), 1); // (10 / 2) / 5
    TEST_ASSERT_EQUAL_DOUBLE(test("1 + 2 * 3 - 4 / 2"), 5);
}

void testFullCyclePower(void)
{
    // pow() может давать ошибку в последнем бите — проверяем с допуском
    TEST_ASSERT_DOUBLE_WITHIN(1e-9, test("2 ^ 10"), 1024);
    TEST_ASSERT_DOUBLE_WITHIN(1e-9, test("9 ^ 0.5"), 3);
    TEST_ASSERT_DOUBLE_WITHIN(1e-9, test("2 ^ 3 ^ 2"), 64); // левая ассоциативность: (2 ^ 3) ^ 2
    TEST_ASSERT_DOUBLE_WITHIN(1e-9, test("2 * 3 ^ 2"), 18); // приоритет выше умножения
    TEST_ASSERT_DOUBLE_WITHIN(1e-9, test("3 ^ 2 * 2"), 18);
}

void testFullCycleParentheses(void)
{
    TEST_ASSERT_EQUAL_DOUBLE(test("(1 + 2) * 3"), 9);
    TEST_ASSERT_EQUAL_DOUBLE(test("2 * (3 + 4 * (5 - 2))"), 30);
    TEST_ASSERT_EQUAL_DOUBLE(test("((2 + 3) * (4 - 1))"), 15);
    TEST_ASSERT_EQUAL_DOUBLE(test("((((5))))"), 5);
}

void testFullCycleDivisionByZero(void)
{
    TEST_ASSERT_TRUE(isnan(test("1 / 0")));
}

void testFullCycleErrors(void)
{
    // Неверные выражения дают NaN
    TEST_ASSERT_TRUE(isnan(test("")));
    TEST_ASSERT_TRUE(isnan(test("1 +")));
    TEST_ASSERT_TRUE(isnan(test("+ 1")));
    TEST_ASSERT_TRUE(isnan(test("1 2")));
    TEST_ASSERT_TRUE(isnan(test("(1 + 2")));
    TEST_ASSERT_TRUE(isnan(test("1 + 2)")));
    TEST_ASSERT_TRUE(isnan(test("123abc")));
    TEST_ASSERT_TRUE(isnan(test("a + b")));
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(testFullCycleNumbers);
    RUN_TEST(testFullCycleBasicOps);
    RUN_TEST(testFullCycleWhitespace);
    RUN_TEST(testFullCyclePrecedenceAndAssociativity);
    RUN_TEST(testFullCyclePower);
    RUN_TEST(testFullCycleParentheses);
    RUN_TEST(testFullCycleDivisionByZero);
    RUN_TEST(testFullCycleErrors);

    return UNITY_END();
}