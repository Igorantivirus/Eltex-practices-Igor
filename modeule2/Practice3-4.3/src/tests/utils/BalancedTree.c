#include <utils/BalancedTree.h>

#include "unity.h"

static int  g_freeCounter;
static int  g_visited;
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

// Обход: собираем значения в массив (userData — указатель на буфер)
static void visitCollect(const void *const value, void *const userData)
{
    int *const buffer = userData;
    buffer[g_visited] = *(int *)value;
    ++g_visited;
}

// AVL-инвариант: коэффициент баланса каждого узла в диапазоне [-1; 1],
// сохранённая высота равна фактической. Возвращает фактическую высоту
// поддерева (пустое поддерево — 0).
static int checkAvl(const BalancedTreeNode *const node)
{
    if (!node)
        return 0;

    const int leftHeight  = checkAvl(node->left);
    const int rightHeight = checkAvl(node->right);

    const int balance = leftHeight - rightHeight;
    TEST_ASSERT_TRUE(balance >= -1 && balance <= 1);

    const int maxHeight = (leftHeight > rightHeight) ? leftHeight : rightHeight;
    TEST_ASSERT_EQUAL_UINT16(node->height, (uint16_t)(maxHeight + 1));

    return maxHeight + 1;
}

void setUp(void)
{
    g_freeCounter = 0;
    g_visited     = 0;
}
void tearDown(void)
{
}

void testBltInit(void)
{
    BalancedTree tree = bltInit(sizeof(int), cmpInt, NULL);

    TEST_ASSERT_EQUAL_UINT32(tree.valueSize, sizeof(int));
    TEST_ASSERT_NOT_NULL(tree.compare);
    TEST_ASSERT_NULL(tree.freeFunction);
    TEST_ASSERT_EQUAL_size_t(tree.size, 0);
    TEST_ASSERT_NULL(tree.root);
}

void testBltInsertAndGetAt(void)
{
    BalancedTree tree = bltInit(sizeof(int), cmpInt, NULL);

    int a = 30;
    int b = 10;
    int c = 20;
    bltInsert(&tree, &a);
    bltInsert(&tree, &b);
    bltInsert(&tree, &c);

    TEST_ASSERT_EQUAL_size_t(tree.size, 3);
    TEST_ASSERT_EQUAL_INT(*(int *)bltGetAt(&tree, 0), 10);
    TEST_ASSERT_EQUAL_INT(*(int *)bltGetAt(&tree, 1), 20);
    TEST_ASSERT_EQUAL_INT(*(int *)bltGetAt(&tree, 2), 30);
    TEST_ASSERT_NULL(bltGetAt(&tree, 3));
    bltFree(&tree);
}

void testBltInsertKeepsSorted(void)
{
    // Вставка в произвольном порядке всегда сохраняет отсортированность
    BalancedTree tree = bltInit(sizeof(int), cmpInt, NULL);

    const int values[] = {5, 3, 8, 1, 9, 2, 7, 4, 6, 0};
    for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); ++i)
        bltInsert(&tree, (void *)&values[i]);

    const int expected[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    TEST_ASSERT_EQUAL_size_t(tree.size, 10);
    for (size_t i = 0; i < sizeof(expected) / sizeof(expected[0]); ++i)
        TEST_ASSERT_EQUAL_INT(*(int *)bltGetAt(&tree, i), expected[i]);
    bltFree(&tree);
}

void testBltInsertDuplicateReplacesValue(void)
{
    // Равные значения не создают новый узел, а обновляют существующий
    BalancedTree tree = bltInit(sizeof(int), cmpInt, freeCounter);

    int a     = 5;
    int a2    = 5;
    const void *first  = bltInsert(&tree, &a);
    const void *second = bltInsert(&tree, &a2);

    TEST_ASSERT_EQUAL_size_t(tree.size, 1);
    TEST_ASSERT_EQUAL_PTR(first, second);
    TEST_ASSERT_EQUAL_INT(g_freeCounter, 1); // старые данные освобождены
    bltFree(&tree);
}

void testBltFindContainsMinMax(void)
{
    BalancedTree tree = bltInit(sizeof(int), cmpInt, NULL);

    TEST_ASSERT_FALSE(bltContains(&tree, NULL));
    TEST_ASSERT_NULL(bltMin(&tree));
    TEST_ASSERT_NULL(bltMax(&tree));

    int a = 30;
    int b = 10;
    int c = 20;
    bltInsert(&tree, &a);
    bltInsert(&tree, &b);
    bltInsert(&tree, &c);

    TEST_ASSERT_TRUE(bltContains(&tree, &a));
    TEST_ASSERT_TRUE(bltContains(&tree, &b));
    TEST_ASSERT_TRUE(bltContains(&tree, &c));

    int missing = 99;
    TEST_ASSERT_FALSE(bltContains(&tree, &missing));
    TEST_ASSERT_NULL(bltFind(&tree, &missing));

    TEST_ASSERT_EQUAL_INT(*(int *)bltMin(&tree), 10);
    TEST_ASSERT_EQUAL_INT(*(int *)bltMax(&tree), 30);

    int found = 20;
    TEST_ASSERT_NOT_NULL(bltFind(&tree, &found));
    TEST_ASSERT_EQUAL_INT(*(int *)bltFind(&tree, &found), 20);
    bltFree(&tree);
}

void testBltRemove(void)
{
    BalancedTree tree = bltInit(sizeof(int), cmpInt, freeCounter);

    int a = 30;
    int b = 10;
    int c = 20;
    bltInsert(&tree, &a);
    bltInsert(&tree, &b);
    bltInsert(&tree, &c);

    TEST_ASSERT_TRUE(bltRemove(&tree, &b));
    TEST_ASSERT_EQUAL_size_t(tree.size, 2);
    TEST_ASSERT_FALSE(bltContains(&tree, &b));
    TEST_ASSERT_TRUE(bltContains(&tree, &a));
    TEST_ASSERT_TRUE(bltContains(&tree, &c));

    // Удаление отсутствующего значения — false
    int missing = 40;
    TEST_ASSERT_FALSE(bltRemove(&tree, &missing));
    TEST_ASSERT_EQUAL_size_t(tree.size, 2);

    TEST_ASSERT_EQUAL_INT(g_freeCounter, 1); // освобождён только один элемент
    bltFree(&tree);
}

void testBltRemoveNoFree(void)
{
    // bltRemoveNoFree убирает элемент, но НЕ вызывает деструктор
    BalancedTree tree = bltInit(sizeof(int), cmpInt, freeCounter);

    int a = 30;
    int b = 10;
    bltInsert(&tree, &a);
    bltInsert(&tree, &b);

    TEST_ASSERT_TRUE(bltRemoveNoFree(&tree, &b));
    TEST_ASSERT_EQUAL_size_t(tree.size, 1);
    TEST_ASSERT_FALSE(bltContains(&tree, &b));

    // Деструктор не должен был вызываться
    TEST_ASSERT_EQUAL_INT(g_freeCounter, 0);
    bltFree(&tree);
    // После bltFree деструктор вызывается для оставшегося элемента
    TEST_ASSERT_EQUAL_INT(g_freeCounter, 1);
}

void testBltClear(void)
{
    // Очистка освобождает все элементы и сбрасывает состояние
    BalancedTree tree = bltInit(sizeof(int), cmpInt, freeCounter);

    int a = 30;
    int b = 10;
    int c = 20;
    bltInsert(&tree, &a);
    bltInsert(&tree, &b);
    bltInsert(&tree, &c);

    bltClear(&tree);

    TEST_ASSERT_EQUAL_size_t(tree.size, 0);
    TEST_ASSERT_NULL(tree.root);
    TEST_ASSERT_EQUAL_INT(g_freeCounter, 3);

    // Повторная очистка пустого дерева — no-op
    bltClear(&tree);
    TEST_ASSERT_EQUAL_INT(g_freeCounter, 3);
    bltFree(&tree);
}

void testBltFreeCallsDestructor(void)
{
    // bltFree освобождает все элементы (вызывает деструктор для каждого)
    BalancedTree tree = bltInit(sizeof(int), cmpInt, freeCounter);

    int a = 30;
    int b = 10;
    bltInsert(&tree, &a);
    bltInsert(&tree, &b);

    bltFree(&tree);

    TEST_ASSERT_EQUAL_INT(g_freeCounter, 2);
    TEST_ASSERT_NULL(tree.root);
    TEST_ASSERT_EQUAL_size_t(tree.size, 0);
}

void testBltGetAtOutOfRange(void)
{
    // Взятие вне диапазона — NULL
    BalancedTree empty = bltInit(sizeof(int), cmpInt, NULL);
    TEST_ASSERT_NULL(bltGetAt(&empty, 0));
    TEST_ASSERT_NULL(bltGetAt(&empty, 100));
    bltFree(&empty);

    BalancedTree tree = bltInit(sizeof(int), cmpInt, NULL);
    int a = 1;
    int b = 2;
    bltInsert(&tree, &a);
    bltInsert(&tree, &b);
    TEST_ASSERT_NULL(bltGetAt(&tree, 2));    // == size, невалидно
    TEST_ASSERT_NULL(bltGetAt(&tree, 100)); // далеко за пределами
    bltFree(&tree);
}

void testBltForEach(void)
{
    // bltForEach обходит дерево в порядке возрастания
    BalancedTree tree = bltInit(sizeof(int), cmpInt, NULL);

    int values[] = {30, 10, 20};
    for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); ++i)
        bltInsert(&tree, (void *)&values[i]);

    int buffer[16] = {0};
    bltForEach(&tree, visitCollect, buffer);

    TEST_ASSERT_EQUAL_INT(g_visited, 3);
    TEST_ASSERT_EQUAL_INT(buffer[0], 10);
    TEST_ASSERT_EQUAL_INT(buffer[1], 20);
    TEST_ASSERT_EQUAL_INT(buffer[2], 30);
    bltFree(&tree);
}


void testBltBalancedAfterSortedInsert(void)
{
    // Вставка отсортированных значений — классический случай вырождения.
    // AVL-дерево должно остаться сбалансированным (высота O(log n)).
    BalancedTree tree = bltInit(sizeof(int), cmpInt, NULL);

    for (int i = 1; i <= 100; ++i)
        bltInsert(&tree, &i);

    TEST_ASSERT_EQUAL_size_t(tree.size, 100);

    const int height = checkAvl(tree.root);
    // Для AVL-дерева из 100 элементов высота не превышает 9
    TEST_ASSERT_TRUE(height <= 9);
    bltFree(&tree);
}

void testBltBalancedAfterRemovals(void)
{
    // После удалений дерево остаётся сбалансированным и отсортированным
    BalancedTree tree = bltInit(sizeof(int), cmpInt, NULL);

    for (int i = 1; i <= 50; ++i)
        bltInsert(&tree, &i);

    for (int i = 1; i <= 50; i += 2)
    {
        bltRemove(&tree, &i); // удаляем все нечётные
    }

    TEST_ASSERT_EQUAL_size_t(tree.size, 25);
    checkAvl(tree.root);

    // Остались чётные числа по возрастанию
    for (int i = 0; i < 25; ++i)
        TEST_ASSERT_EQUAL_INT(*(int *)bltGetAt(&tree, (size_t)i), 2 * (i + 1));
    bltFree(&tree);
}

void testBltInsertValueMacro(void)
{
    // Макрос bltInsertValue вставляет значение по типу
    BalancedTree tree = bltInit(sizeof(int), cmpInt, NULL);

    bltInsertValue(&tree, 42, int);

    TEST_ASSERT_EQUAL_size_t(tree.size, 1);
    int key = 42;
    TEST_ASSERT_TRUE(bltContains(&tree, &key));
    bltFree(&tree);
}

void testBltNullSafety(void)
{
    // Все операции должны корректно переживать NULL-ссылки
    TEST_ASSERT_NULL(bltInsert(NULL, NULL));
    TEST_ASSERT_TRUE(bltSize(NULL) == 0);
    TEST_ASSERT_FALSE(bltRemove(NULL, NULL));
    TEST_ASSERT_FALSE(bltRemoveNoFree(NULL, NULL));
    TEST_ASSERT_NULL(bltFind(NULL, NULL));
    TEST_ASSERT_FALSE(bltContains(NULL, NULL));
    TEST_ASSERT_NULL(bltMin(NULL));
    TEST_ASSERT_NULL(bltMax(NULL));
    TEST_ASSERT_NULL(bltGetAt(NULL, 0));
    bltForEach(NULL, visitCollect, NULL);
    bltClear(NULL);
    bltFree(NULL);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(testBltInit);
    RUN_TEST(testBltInsertAndGetAt);
    RUN_TEST(testBltInsertKeepsSorted);
    RUN_TEST(testBltInsertDuplicateReplacesValue);
    RUN_TEST(testBltFindContainsMinMax);
    RUN_TEST(testBltRemove);
    RUN_TEST(testBltRemoveNoFree);
    RUN_TEST(testBltClear);
    RUN_TEST(testBltFreeCallsDestructor);
    RUN_TEST(testBltGetAtOutOfRange);
    RUN_TEST(testBltForEach);
    RUN_TEST(testBltBalancedAfterSortedInsert);
    RUN_TEST(testBltBalancedAfterRemovals);
    RUN_TEST(testBltInsertValueMacro);
    RUN_TEST(testBltNullSafety);

    return UNITY_END();
}

