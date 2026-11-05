#pragma once

#include <stddef.h>
#include <stdint.h>

typedef int  (*BinTreeKeyCompare)(const void *a, const void *b);
typedef void (*BinTreeFreeFunction)(void *key, void *value);

typedef struct BinTreeNode
{
    struct BinTreeNode *left;
    struct BinTreeNode *right;
    void               *key;
    void               *value;
} BinTreeNode;

typedef struct
{
    uint32_t            keySize;      // Размер памяти на один ключ
    uint32_t            valueSize;    // Размер памяти на одно значение
    BinTreeNode        *root;         // Стартовый (корневой) узел
    BinTreeKeyCompare   keyCompare;   // Сравнение элементов
    BinTreeFreeFunction freeFunction; // Может быть NULL
    size_t              size;         // Число элементов в дереве
} BinTree;

// Конструкторы

BinTree btInit(const uint32_t keySize, const uint32_t valueSize, const BinTreeKeyCompare keyCompare, const BinTreeFreeFunction freeFunction);
void    btFree(BinTree *const self);
void    btClear(BinTree *const self);

#define btInitType(KEY, VALUE, compare, free) btInit(sizeof(KEY), sizeof(VALUE), compare, free)

BinTreeNode *btInsert(BinTree *const self, const void *const key, const void *const value);
void         btRemove(BinTree *const self, const void *const key);
const void  *btGetAt(BinTree *const self, const void *const key);

#define btInsertValue(self, key, value, KEY, VALUE) \
    {                                               \
        KEY   a = key;                              \
        VALUE b = value;                            \
        btInsert(self, &a, &b);                     \
    }