#pragma once

#include <stddef.h>
#include <stdint.h>

typedef void *DaOneElementPtr;
typedef void  (*DaFreeFunction)(void *);

typedef struct
{
    uint32_t       oneElementSize; // Размер памяти на один элемент
    DaFreeFunction freeFunction;   // Функция освобождения памяти для одного элемента
    size_t         size;           // Размер массива
    size_t         capacity;       // Вместимость
    uint8_t       *array_;         // Массив
} DynamicArray;

// Конструкторы

DynamicArray daInit(const uint32_t oneElementSize, DaFreeFunction freeFunction);
DynamicArray daInitReserve(const uint32_t oneElementSize, const size_t capacity, DaFreeFunction freeFunction);

#define daInitType(TYPE, freeFunction) daInit(sizeof(TYPE), freeFunction);
#define daInitTypeReserve(TYPE, capacity, freeFunction) daInitReserve(sizeof(TYPE), capacity, freeFunction);
// Деструкторы

void daFree(DynamicArray *const self);
void daClear(DynamicArray *const self);
void daReserve(DynamicArray *const self, const size_t newCapacity);

// Работа с элементами

void            daInsert(DynamicArray *const self, const size_t index, const DaOneElementPtr ptr);
void            daPushBack(DynamicArray *const self, const DaOneElementPtr ptr);
void            daRemove(DynamicArray *const self, const size_t index);
void            daSetAt(DynamicArray *const self, const size_t index, const DaOneElementPtr ptr);
DaOneElementPtr daGetAt(const DynamicArray *const self, const size_t index);

#define daInsertValue(self, index, element, TYPE) \
    {                                             \
        TYPE a = element;                         \
        daInsert(self, index, &a);                \
    }
#define daPushBackValue(self, element, TYPE) \
    {                                        \
        TYPE a = element;                    \
        daPushBack(self, &a);                \
    }
