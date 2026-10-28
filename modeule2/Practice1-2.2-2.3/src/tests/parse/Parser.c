#include <utils/BinTree.h>
#include <utils/String.h>
#include <ui/BaseCalcBTInit.h>
#include <calc/Callbacks.h>
#include <calc/CalcNodes.h>
#include <parse/Parser.h>
#include <parse/StringView.h>

#include "calc/CalcTree.h"
#include "unity.h"

// Дерево операций для тестов парсера
static int charCompare(const void *a, const void *b)
{
    const char ca = *(const char *)a;
    const char cb = *(const char *)b;
    return (ca > cb) - (ca < cb);
}

static CallTreeAdditionalInfo makeInfo(void)
{
    CallTreeAdditionalInfo info;
    info.binOpTree = btInitType(char, OperatorInfo, charCompare, NULL);
    info.callBacks = (BinTree){0};
    return info;
}

void setUp(void)
{
}
void tearDown(void)
{
}

// ======================= StringView ======================= //

void testSvFromCharsArray(void)
{
    char buf[] = "1 + 2";
    StringView sv = svFromCharsArray(buf);
    TEST_ASSERT_NOT_NULL(sv.str);
    TEST_ASSERT_EQUAL_UINT32(sv.size, 5);

    char num[] = "3.5";
    TEST_ASSERT_EQUAL_DOUBLE(svToDouble(svFromCharsArray(num)), 3.5);
}

void testSvFromString(void)
{
    String s = sInitCharArray("7 + 1");
    StringView sv = svFromString(s);
    TEST_ASSERT_EQUAL_UINT32(sv.size, s.size);
    TEST_ASSERT_EQUAL_DOUBLE(svToDouble(sv), 7.0);
    sFree(&s);
}

// ========================== Парсинг ======================= //

void testParseSingleNumber(void)
{
    CallTreeAdditionalInfo info = makeInfo();
    char expr[] = "42";
    Node *root = parse(svFromCharsArray(expr), &info);
    TEST_ASSERT_NOT_NULL(root);
    TEST_ASSERT_EQUAL_UINT32(root->type, NodeTypeValue);
    TEST_ASSERT_EQUAL_DOUBLE(root->data.value.value, 42);
    nodeFree(root, &info);
}

void testParseBinOpStructure(void)
{
    CallTree tree;
    ctInit(&tree, NULL, initDefaultBinOpTree());
    char expr[] = "1 + 2";
    Node *root = parse(svFromCharsArray(expr), &tree.info);
    TEST_ASSERT_NOT_NULL(root);
    TEST_ASSERT_EQUAL_UINT32(root->type, NodeTypeBinOp);
    TEST_ASSERT_EQUAL_CHAR(root->data.binop.op, '+');
    TEST_ASSERT_EQUAL_UINT32(root->data.binop.left->type, NodeTypeValue);
    TEST_ASSERT_EQUAL_UINT32(root->data.binop.right->type, NodeTypeValue);
    TEST_ASSERT_EQUAL_DOUBLE(root->data.binop.left->data.value.value, 1);
    TEST_ASSERT_EQUAL_DOUBLE(root->data.binop.right->data.value.value, 2);
    nodeFree(root, &tree.info);
    ctFree(&tree);
}

void testParsePrecedence(void)
{
    // 1 + 2 * 3  =>  (1 + (2 * 3))
    CallTree tree;
    ctInit(&tree, NULL, initDefaultBinOpTree());
    char expr[] = "1 + 2 * 3";
    Node *root = parse(svFromCharsArray(expr), &tree.info);
    TEST_ASSERT_NOT_NULL(root);
    TEST_ASSERT_EQUAL_CHAR(root->data.binop.op, '+');
    TEST_ASSERT_EQUAL_UINT32(root->data.binop.left->type, NodeTypeValue);
    TEST_ASSERT_EQUAL_CHAR(root->data.binop.right->data.binop.op, '*');
    nodeFree(root, &tree.info);
    ctFree(&tree);
}

void testParseLeftAssociativity(void)
{
    // 1 - 2 - 3  =>  ((1 - 2) - 3)
    CallTree tree;
    ctInit(&tree, NULL, initDefaultBinOpTree());
    char expr[] = "1 - 2 - 3";
    Node *root = parse(svFromCharsArray(expr), &tree.info);
    TEST_ASSERT_NOT_NULL(root);
    TEST_ASSERT_EQUAL_CHAR(root->data.binop.op, '-');
    TEST_ASSERT_EQUAL_CHAR(root->data.binop.left->data.binop.op, '-');
    TEST_ASSERT_EQUAL_UINT32(root->data.binop.right->type, NodeTypeValue);
    nodeFree(root, &tree.info);
    ctFree(&tree);
}

void testParseParentheses(void)
{
    // (1 + 2) * 3  =>  ((1 + 2) * 3)
    CallTree tree;
    ctInit(&tree, NULL, initDefaultBinOpTree());
    char expr[] = "(1 + 2) * 3";
    Node *root = parse(svFromCharsArray(expr), &tree.info);
    TEST_ASSERT_NOT_NULL(root);
    TEST_ASSERT_EQUAL_CHAR(root->data.binop.op, '*');
    TEST_ASSERT_EQUAL_CHAR(root->data.binop.left->data.binop.op, '+');
    nodeFree(root, &tree.info);
    ctFree(&tree);
}

void testParseWhitespace(void)
{
    // Пробелы вокруг и внутри выражения не имеют значения
    CallTreeAdditionalInfo info = makeInfo();
    char expr[] = " 12 . 5 ";
    Node *root = parse(svFromCharsArray(expr), &info);
    TEST_ASSERT_NULL(root);
    nodeFree(root, &info);
}

void testParseErrors(void)
{
    CallTreeAdditionalInfo info = makeInfo();

    // Пустые выражения
    TEST_ASSERT_NULL(parse((StringView){.str = NULL, .size = 0}, &info));
    TEST_ASSERT_NULL(parse((StringView){.str = "", .size = 0}, &info));
    TEST_ASSERT_NULL(parse(svFromCharsArray("   "), &info));

    // Неполные выражения
    char e1[] = "1 +";
    TEST_ASSERT_NULL(parse(svFromCharsArray(e1), &info));
    char e2[] = "+ 1";
    TEST_ASSERT_NULL(parse(svFromCharsArray(e2), &info));
    char e3[] = "1 + 2 3";
    TEST_ASSERT_NULL(parse(svFromCharsArray(e3), &info));

    // Нераспознанный оператор
    char e4[] = "1 % 2";
    TEST_ASSERT_NULL(parse(svFromCharsArray(e4), &info));

    // Число, за которым идёт "мусор"
    char e5[] = "123abc";
    TEST_ASSERT_NULL(parse(svFromCharsArray(e5), &info));

    // Неразкрытая скобка
    char e6[] = "(1 + 2";
    TEST_ASSERT_NULL(parse(svFromCharsArray(e6), &info));

    // Лишняя закрывающая скобка
    char e7[] = "1 + 2)";
    TEST_ASSERT_NULL(parse(svFromCharsArray(e7), &info));

    // Нулевой info
    char e8[] = "1";
    TEST_ASSERT_NULL(parse(svFromCharsArray(e8), NULL));
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(testSvFromCharsArray);
    RUN_TEST(testSvFromString);
    RUN_TEST(testParseSingleNumber);
    RUN_TEST(testParseBinOpStructure);
    RUN_TEST(testParsePrecedence);
    RUN_TEST(testParseLeftAssociativity);
    RUN_TEST(testParseParentheses);
    RUN_TEST(testParseWhitespace);
    RUN_TEST(testParseErrors);

    return UNITY_END();
}