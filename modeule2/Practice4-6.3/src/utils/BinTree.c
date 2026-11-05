#include <stdlib.h>
#include <string.h>

#include <utils/BinTree.h>

// private:

// Создаёт узел и копирует в него ключ и значение.
static BinTreeNode *btNodeNew(const BinTree *const tree, const void *const key, const void *const value)
{
    BinTreeNode *node = malloc(sizeof(BinTreeNode));
    if (!node)
        return NULL;

    node->left = NULL;
    node->right = NULL;
    node->key = malloc(tree->keySize);
    node->value = malloc(tree->valueSize);

    if (!node->key || !node->value)
    {
        free(node->key);
        free(node->value);
        free(node);
        return NULL;
    }

    memcpy(node->key, key, tree->keySize);
    memcpy(node->value, value, tree->valueSize);
    return node;
}

// Вызывает freeFunction (если она есть) для данных узла.
static void btNodeFreeData(const BinTree *const tree, BinTreeNode *const node)
{
    if (tree->freeFunction)
        tree->freeFunction(node->key, node->value);
}

// Удаляет узел целиком: данные, память ключа, значения и сам узел.
static void btNodeDelete(const BinTree *const tree, BinTreeNode *const node)
{
    btNodeFreeData(tree, node);
    free(node->key);
    free(node->value);
    free(node);
}

// Рекурсивно удаляет поддерево.
static void btDeleteSubtree(const BinTree *const tree, BinTreeNode *const node)
{
    if (!node)
        return;
    btDeleteSubtree(tree, node->left);
    btDeleteSubtree(tree, node->right);
    btNodeDelete(tree, node);
}

// public:

BinTree btInit(const uint32_t keySize, const uint32_t valueSize, const BinTreeKeyCompare keyCompare, const BinTreeFreeFunction freeFunction)
{
    return (BinTree){keySize, valueSize, NULL, keyCompare, freeFunction, 0};
}

void btClear(BinTree *const self)
{
    btDeleteSubtree(self, self->root);
    self->root = NULL;
    self->size = 0;
}

void btFree(BinTree *const self)
{
    btClear(self);
}

BinTreeNode *btInsert(BinTree *const self, const void *const key, const void *const value)
{
    BinTreeNode *parent = NULL;
    BinTreeNode *node = self->root;
    int          res = 0;

    // Ищем ключ, запоминая родителя
    while (node)
    {
        res = self->keyCompare(key, node->key);
        if (res == 0)
        {
            // Ключ уже есть: чистим старые данные и записываем новые
            btNodeFreeData(self, node);
            memcpy(node->key, key, self->keySize);
            memcpy(node->value, value, self->valueSize);
            return node;
        }
        parent = node;
        node = (res < 0) ? node->left : node->right;
    }

    // Ключа нет: создаём новый узел и цепляем к родителю
    node = btNodeNew(self, key, value);
    if (!node)
        return NULL;

    if (!parent)
        self->root = node;
    else if (res < 0)
        parent->left = node;
    else
        parent->right = node;

    self->size++;
    return node;
}

void btRemove(BinTree *const self, const void *const key)
{
    BinTreeNode *parent = NULL;
    BinTreeNode *node = self->root;

    // Ищем узел и его родителя
    while (node)
    {
        const int res = self->keyCompare(key, node->key);
        if (res == 0)
            break;
        parent = node;
        node = (res < 0) ? node->left : node->right;
    }
    if (!node)
        return;

    if (node->left && node->right)
    {
        // Два потомка: находим минимум правого поддерева (преемника)
        BinTreeNode *succParent = node;
        BinTreeNode *succ = node->right;
        while (succ->left)
        {
            succParent = succ;
            succ = succ->left;
        }

        // Данные удаляемого узла уничтожаем, а память ключа и значения
        // преемника просто передаём удаляемому узлу
        btNodeFreeData(self, node);
        free(node->key);
        free(node->value);
        node->key = succ->key;
        node->value = succ->value;

        // Вырезаем преемника (у него нет левого потомка)
        if (succParent->left == succ)
            succParent->left = succ->right;
        else
            succParent->right = succ->right;
        free(succ);
    }
    else
    {
        // 0 или 1 потомок: заменяем узел его потомком (или NULL)
        BinTreeNode *const child = node->left ? node->left : node->right;
        if (!parent)
            self->root = child;
        else if (parent->left == node)
            parent->left = child;
        else
            parent->right = child;
        btNodeDelete(self, node);
    }

    self->size--;
}

const void *btGetAt(BinTree *const self, const void *const key)
{
    BinTreeNode *node = self->root;
    while (node)
    {
        const int res = self->keyCompare(key, node->key);
        if (res == 0)
            return node->value;
        node = (res < 0) ? node->left : node->right;
    }
    return NULL;
}