#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// AVL дерево

typedef int  (*BalancedTreeCompare)(const void *a, const void *b); // <0, 0, >0 как у strcmp
typedef void (*BalancedTreeFreeFunction)(void *value);            // Освобождает ресурсы ВНУТРИ значения
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

// Модификация

// Копирует значение в дерево. Если равное уже есть — старое освобождается и заменяется.
// Возвращает указатель на хранимое значение или NULL при ошибке (нет памяти / неверные аргументы).
const void *bltInsert(BalancedTree *const self, const void *const value);
// Возвращает true, если элемент был найден и удалён.
bool        bltRemove(BalancedTree *const self, const void *const value);
// ДОБАВЛЕНО: удаляет элемент БЕЗ вызова freeFunction (данные внутри значения остаются жить у вызывающего).
// Сначала скопируй значение к себе, потом удаляй — память узла освобождается.
bool        bltRemoveNoFree(BalancedTree *const self, const void *const value);

#define bltInsertValue(self, value, TYPE) \
    do                                    \
    {                                     \
        TYPE bltTmp_ = (value);           \
        bltInsert((self), &bltTmp_);      \
    } while (0)

// Доступ (указатели действительны до следующего изменения дерева; менять «ключевые» поля через них нельзя)

const void *bltFind(const BalancedTree *const self, const void *const value);
bool        bltContains(const BalancedTree *const self, const void *const value);
const void *bltMin(const BalancedTree *const self);
const void *bltMax(const BalancedTree *const self);
size_t      bltSize(const BalancedTree *const self);
// ДОБАВЛЕНО: элемент по индексу в отсортированном порядке (0..size-1) или NULL. Работает за O(index).
const void *bltGetAt(const BalancedTree *const self, const size_t index);

// Обход всех элементов по возрастанию
void bltForEach(const BalancedTree *const self, const BalancedTreeVisitFunction visit, void *const userData);
