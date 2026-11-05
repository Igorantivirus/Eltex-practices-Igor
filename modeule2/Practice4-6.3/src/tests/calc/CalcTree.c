#include <math.h>
#include <stddef.h>
#include <stdlib.h>

#include <calc/CalcNodes.h>
#include <calc/CalcTree.h>
#include <calc/Callbacks.h>
#include <string.h>
#include <ui/BaseCalcBTInit.h>
#include <utils/BinTree.h>

#include "unity.h"

static int typeCompare(const void *a, const void *b)
{
    const int ca = *(const NodeType *)a;
    const int cb = *(const NodeType *)b;
    return (ca > cb) - (ca < cb);
}

static int charCompare(const void *a, const void *b)
{
    const char ca = *(const char *)a;
    const char cb = *(const char *)b;
    return (ca > cb) - (ca < cb);
}

static CallTree makeTree(void)
{
    CallTree tree;
    tree.root = NULL;
    tree.info.binOpTree = btInitType(char, OperatorInfo, charCompare, NULL);
    tree.info.callBacks = btInitType(NodeType, NodeCallBacks, typeCompare, NULL);
    return tree;
}

void setUp(void)
{
}
void tearDown(void)
{
}

// =========================== nodeInit =========================== //

void testNodeInitValue(void)
{
    Node *node = nodeInit(NodeTypeValue);
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_UINT32(node->type, NodeTypeValue);
    TEST_ASSERT_EQUAL_DOUBLE(node->data.value.value, 0.0);
    free(node);
}

void testNodeInitBinOp(void)
{
    Node *node = nodeInit(NodeTypeBinOp);
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_UINT32(node->type, NodeTypeBinOp);
    TEST_ASSERT_NULL(node->data.binop.left);
    TEST_ASSERT_NULL(node->data.binop.right);
    free(node);
}

// ===================== calc по одному узлу ===================== //

void testCalcValueNode(void)
{
    TEST_ASSERT_EQUAL_DOUBLE(calcValueNode(&(NodeData){.value = {.value = 12.5}}, NULL), 12.5);
    TEST_ASSERT_TRUE(isnan(calcValueNode(NULL, NULL)));
}

void testCalcBinOpNodeUnknownOp(void)
{
    // Оператор отсутствует в binOpTree => NaN
    CallTree tree = makeTree();
    Node    *left = nodeInit(NodeTypeValue);
    left->data.value.value = 1.0;
    Node *right = nodeInit(NodeTypeValue);
    right->data.value.value = 2.0;
    Node *op = nodeInit(NodeTypeBinOp);
    op->data.binop.op = '+';
    op->data.binop.left = left;
    op->data.binop.right = right;

    TEST_ASSERT_TRUE(isnan(calcBinOpNode(&op->data, &tree.info)));

    nodeFree(op, &tree.info);
}

// ========================= nodeCalc =========================== //

void testNodeCalcNull(void)
{
    CallTree tree = makeTree();
    TEST_ASSERT_TRUE(isnan(nodeCalc(NULL, &tree.info)));
}

// =========================== CallTree ========================== //

void testCtInitCalcAndFree(void)
{
    // 1 + 2
    Node *one = nodeInit(NodeTypeValue);
    one->data.value.value = 1.0;
    Node *two = nodeInit(NodeTypeValue);
    two->data.value.value = 2.0;
    Node *plus = nodeInit(NodeTypeBinOp);
    plus->data.binop.op = '+';
    plus->data.binop.left = one;
    plus->data.binop.right = two;

    CallTree tree;
    ctInit(&tree, plus, initDefaultBinOpTree());
    TEST_ASSERT_NOT_NULL(tree.root);
    TEST_ASSERT_EQUAL_DOUBLE(calc(&tree), 3.0);
    ctFree(&tree);
    TEST_ASSERT_NULL(tree.root);
}

void testCtChangeNodeRoot(void)
{
    CallTree tree;
    ctInit(&tree, NULL, initDefaultBinOpTree());

    Node *five = nodeInit(NodeTypeValue);
    five->data.value.value = 5.0;
    ctChangeNodeRoot(&tree, five);
    TEST_ASSERT_EQUAL_DOUBLE(calc(&tree), 5.0);

    Node *seven = nodeInit(NodeTypeValue);
    seven->data.value.value = 7.0;
    ctChangeNodeRoot(&tree, seven);
    TEST_ASSERT_EQUAL_DOUBLE(calc(&tree), 7.0);

    // Замена на NULL — безопасно, состояние не меняется
    ctChangeNodeRoot(&tree, NULL);
    TEST_ASSERT_EQUAL_DOUBLE(calc(&tree), 7.0);

    ctFree(&tree);
    TEST_ASSERT_NULL(tree.root);
}

void testCalcDefaultTree(void)
{
    // Все операции из initDefaultBinOpTree()

    const char   ops[] = "+-*/^";
    const double expected[] = {14.0, 6.0, 40.0, 2.5, 10000.0};

    for (size_t i = 0; i < strlen(ops); ++i)
    {
        Node *left = nodeInit(NodeTypeValue);
        left->data.value.value = 10.0;
        Node *right = nodeInit(NodeTypeValue);
        right->data.value.value = 4.0;

        Node *op = nodeInit(NodeTypeBinOp);
        op->data.binop.op = ops[i];
        op->data.binop.left = left;
        op->data.binop.right = right;

        CallTree tree;
        ctInit(&tree, op, initDefaultBinOpTree());
        double res = calc(&tree);
        TEST_ASSERT_DOUBLE_WITHIN(1e-9, res, expected[i]);
        ctFree(&tree);
    }
}

void testDivByZero(void)
{
    // Деление на ноль => NaN
    Node *left = nodeInit(NodeTypeValue);
    left->data.value.value = 1.0;
    Node *right = nodeInit(NodeTypeValue);
    right->data.value.value = 0.0;
    Node *op = nodeInit(NodeTypeBinOp);
    op->data.binop.op = '/';
    op->data.binop.left = left;
    op->data.binop.right = right;

    CallTree tree;
    ctInit(&tree, op, initDefaultBinOpTree());
    TEST_ASSERT_TRUE(isnan(calc(&tree)));
    ctFree(&tree);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(testNodeInitValue);
    RUN_TEST(testNodeInitBinOp);
    RUN_TEST(testCalcValueNode);
    RUN_TEST(testCalcBinOpNodeUnknownOp);
    RUN_TEST(testNodeCalcNull);
    RUN_TEST(testCtInitCalcAndFree);
    RUN_TEST(testCtChangeNodeRoot);
    RUN_TEST(testCalcDefaultTree);
    RUN_TEST(testDivByZero);

    return UNITY_END();
}