#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// AVL дерево

typedef int  (*BalancedTreeCompare)(const void *a, const void *b); // <0, 0, >0 как у strcmp
typedef void (*BalancedTreeFreeFunction)(void *value);
typedef void (*BalancedTreeVisitFunction)(const void *value, void *userData);

typedef struct BalancedTreeNode
{
    struct BalancedTreeNode *left;
    struct BalancedTreeNode *right;
    void                    *value;
    uint16_t                 height;
} BalancedTreeNode;

typedef struct
{
    uint32_t                 valueSize;    // Размер памяти на одно значение
    BalancedTreeNode        *root;         // Корневой узел
    BalancedTreeCompare      compare;      // Сравнение элементов
    BalancedTreeFreeFunction freeFunction; // Может быть NULL
    size_t                   size;         // Число элементов в дереве
} BalancedTree;

// Конструкторы

BalancedTree bltInit(const uint32_t valueSize, const BalancedTreeCompare compare, const BalancedTreeFreeFunction freeFunction);
void         bltFree(BalancedTree *const self);
void         bltClear(BalancedTree *const self);

#define bltInitType(TYPE, compare, free) bltInit(sizeof(TYPE), compare, free)

const void *bltInsert(BalancedTree *const self, const void *const value);
bool        bltRemove(BalancedTree *const self, const void *const value);
bool        bltRemoveNoFree(BalancedTree *const self, const void *const value);

#define bltInsertValue(self, value, TYPE) \
    do                                    \
    {                                     \
        TYPE bltTmp_ = (value);           \
        bltInsert((self), &bltTmp_);      \
    } while (0)

const void *bltFind(const BalancedTree *const self, const void *const value);
bool        bltContains(const BalancedTree *const self, const void *const value);
const void *bltMin(const BalancedTree *const self);
const void *bltMax(const BalancedTree *const self);
size_t      bltSize(const BalancedTree *const self);
const void *bltGetAt(const BalancedTree *const self, const size_t index);

void bltForEach(const BalancedTree *const self, const BalancedTreeVisitFunction visit, void *const userData);