#include <stdlib.h>
#include <string.h>

#include <utils/BalancedTree.h>

// private:

// Создаёт узел и копирует в него значение.
static BalancedTreeNode *bltNodeNew(const BalancedTree *const tree, const void *const value)
{
    BalancedTreeNode *node = malloc(sizeof(BalancedTreeNode));
    if (!node)
        return NULL;

    node->value = malloc(tree->valueSize);
    if (!node->value)
    {
        free(node);
        return NULL;
    }

    memcpy(node->value, value, tree->valueSize);
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

// Удаляет узел целиком: данные (через freeFunction, если freeData), память значения и сам узел.
// ИЗМЕНЕНО: добавлен параметр freeData (для bltRemoveNoFree)
static void bltNodeDelete(const BalancedTree *const tree, BalancedTreeNode *const node, const bool freeData)
{
    if (freeData && tree->freeFunction)
        tree->freeFunction(node->value);
    free(node->value);
    free(node);
}

// Рекурсивно удаляет поддерево.
static void bltDeleteSubtree(const BalancedTree *const tree, BalancedTreeNode *const node)
{
    if (!node)
        return;
    bltDeleteSubtree(tree, node->left);
    bltDeleteSubtree(tree, node->right);
    bltNodeDelete(tree, node, true); // ИЗМЕНЕНО: новый параметр
}

// Балансировка

static int bltHeight(const BalancedTreeNode *const node)
{
    return node ? node->height : 0;
}

static void bltUpdateHeight(BalancedTreeNode *const node)
{
    const int l = bltHeight(node->left);
    const int r = bltHeight(node->right);
    node->height = (uint16_t)((l > r ? l : r) + 1); // ИЗМЕНЕНО: height теперь uint16_t
}

// > 0 — перевешивает левое поддерево, < 0 — правое
static int bltBalanceFactor(const BalancedTreeNode *const node)
{
    return bltHeight(node->left) - bltHeight(node->right);
}

//      node            left
//      /  \            /  \
//    left  C   ->     A   node
//    /  \                 /  \
//   A    B               B    C
static BalancedTreeNode *bltRotateRight(BalancedTreeNode *const node)
{
    BalancedTreeNode *const left = node->left;
    node->left = left->right;
    left->right = node;
    bltUpdateHeight(node);
    bltUpdateHeight(left);
    return left;
}

// Зеркально bltRotateRight
static BalancedTreeNode *bltRotateLeft(BalancedTreeNode *const node)
{
    BalancedTreeNode *const right = node->right;
    node->right = right->left;
    right->left = node;
    bltUpdateHeight(node);
    bltUpdateHeight(right);
    return right;
}

// Восстанавливает баланс узла (разница высот детей не больше 1). Возвращает новый корень поддерева.
static BalancedTreeNode *bltBalance(BalancedTreeNode *const node)
{
    bltUpdateHeight(node);
    const int balance = bltBalanceFactor(node);

    if (balance > 1) // Перевес влево
    {
        if (bltBalanceFactor(node->left) < 0) // Случай «лево-право»: сначала выпрямляем
            node->left = bltRotateLeft(node->left);
        return bltRotateRight(node);
    }
    if (balance < -1) // Перевес вправо
    {
        if (bltBalanceFactor(node->right) > 0) // Случай «право-лево»
            node->right = bltRotateRight(node->right);
        return bltRotateLeft(node);
    }
    return node;
}

// ---- Рекурсивные операции (глубина рекурсии O(log n)) ----

// result — указатель на хранимое значение или NULL, если не хватило памяти.
static BalancedTreeNode *bltInsertNode(BalancedTree *const tree, BalancedTreeNode *const node, const void *const value, const void **const result)
{
    if (!node)
    {
        BalancedTreeNode *const created = bltNodeNew(tree, value);
        if (created)
        {
            tree->size++;
            *result = created->value;
        }
        return created; // NULL — ничего не прицепится, дерево не пострадает
    }

    const int res = tree->compare(value, node->value);
    if (res == 0)
    {
        // Значение уже есть: чистим старые данные и записываем новые
        // (если передали указатель на само хранимое значение — делать нечего)
        if (value != node->value)
        {
            if (tree->freeFunction)
                tree->freeFunction(node->value);
            memcpy(node->value, value, tree->valueSize);
        }
        *result = node->value;
        return node;
    }

    if (res < 0)
        node->left = bltInsertNode(tree, node->left, value, result);
    else
        node->right = bltInsertNode(tree, node->right, value, result);
    return bltBalance(node);
}

// Вырезает минимальный узел поддерева (не удаляя его), возвращает новый корень поддерева.
static BalancedTreeNode *bltDetachMin(BalancedTreeNode *const node, BalancedTreeNode **const minNode)
{
    if (!node->left)
    {
        *minNode = node;
        return node->right;
    }
    node->left = bltDetachMin(node->left, minNode);
    return bltBalance(node);
}

// ИЗМЕНЕНО: добавлен параметр freeData
static BalancedTreeNode *bltRemoveNode(BalancedTree *const tree, BalancedTreeNode *const node, const void *const value, const bool freeData, bool *const removed)
{
    if (!node)
        return NULL;

    const int res = tree->compare(value, node->value);
    if (res < 0)
        node->left = bltRemoveNode(tree, node->left, value, freeData, removed);
    else if (res > 0)
        node->right = bltRemoveNode(tree, node->right, value, freeData, removed);
    else
    {
        BalancedTreeNode *const left = node->left;
        BalancedTreeNode *right = node->right;
        bltNodeDelete(tree, node, freeData); // ИЗМЕНЕНО: freeData. value больше не трогаем: он мог указывать в этот узел
        tree->size--;
        *removed = true;

        if (!right)
            return left;

        // Два потомка (или только правый): на место узла ставим минимум правого поддерева
        BalancedTreeNode *minNode = NULL;
        right = bltDetachMin(right, &minNode);
        minNode->left = left;
        minNode->right = right;
        return bltBalance(minNode);
    }
    return bltBalance(node);
}

// ДОБАВЛЕНО: обход по возрастанию до элемента номер *index (счётчик уменьшается)
static const BalancedTreeNode *bltGetAtNode(const BalancedTreeNode *const node, size_t *const index)
{
    if (!node)
        return NULL;
    const BalancedTreeNode *const found = bltGetAtNode(node->left, index);
    if (found)
        return found;
    if (*index == 0)
        return node;
    --*index;
    return bltGetAtNode(node->right, index);
}

static void bltForEachNode(const BalancedTreeNode *const node, const BalancedTreeVisitFunction visit, void *const userData)
{
    if (!node)
        return;
    bltForEachNode(node->left, visit, userData);
    visit(node->value, userData);
    bltForEachNode(node->right, visit, userData);
}

// public:

BalancedTree bltInit(const uint32_t valueSize, const BalancedTreeCompare compare, const BalancedTreeFreeFunction freeFunction)
{
    return (BalancedTree){valueSize, NULL, compare, freeFunction, 0};
}

void bltClear(BalancedTree *const self)
{
    if (!self)
        return;
    bltDeleteSubtree(self, self->root);
    self->root = NULL;
    self->size = 0;
}

void bltFree(BalancedTree *const self)
{
    bltClear(self);
}

const void *bltInsert(BalancedTree *const self, const void *const value)
{
    if (!self || !value || !self->compare || self->valueSize == 0)
        return NULL;

    const void *result = NULL;
    self->root = bltInsertNode(self, self->root, value, &result);
    return result;
}

bool bltRemove(BalancedTree *const self, const void *const value)
{
    if (!self || !value || !self->compare)
        return false;

    bool removed = false;
    self->root = bltRemoveNode(self, self->root, value, true, &removed); // ИЗМЕНЕНО: freeData = true
    return removed;
}

// ДОБАВЛЕНО
bool bltRemoveNoFree(BalancedTree *const self, const void *const value)
{
    if (!self || !value || !self->compare)
        return false;

    bool removed = false;
    self->root = bltRemoveNode(self, self->root, value, false, &removed);
    return removed;
}

const void *bltFind(const BalancedTree *const self, const void *const value)
{
    if (!self || !value || !self->compare)
        return NULL;

    const BalancedTreeNode *node = self->root;
    while (node)
    {
        const int res = self->compare(value, node->value);
        if (res == 0)
            return node->value;
        node = (res < 0) ? node->left : node->right;
    }
    return NULL;
}

bool bltContains(const BalancedTree *const self, const void *const value)
{
    return bltFind(self, value) != NULL;
}

const void *bltMin(const BalancedTree *const self)
{
    if (!self || !self->root)
        return NULL;
    const BalancedTreeNode *node = self->root;
    while (node->left)
        node = node->left;
    return node->value;
}

const void *bltMax(const BalancedTree *const self)
{
    if (!self || !self->root)
        return NULL;
    const BalancedTreeNode *node = self->root;
    while (node->right)
        node = node->right;
    return node->value;
}

size_t bltSize(const BalancedTree *const self)
{
    return self ? self->size : 0;
}

// ДОБАВЛЕНО
const void *bltGetAt(const BalancedTree *const self, const size_t index)
{
    if (!self || index >= self->size)
        return NULL;
    size_t                        i = index;
    const BalancedTreeNode *const node = bltGetAtNode(self->root, &i);
    return node ? node->value : NULL;
}

void bltForEach(const BalancedTree *const self, const BalancedTreeVisitFunction visit, void *const userData)
{
    if (!self || !visit)
        return;
    bltForEachNode(self->root, visit, userData);
}
