#pragma once

#include <stddef.h>
typedef int  (*SlCompareFunction)(const void *const left, const void *const right);
typedef void (*SlFreeElement)(void *elem);

typedef struct SortedListNode SortedListNode;
struct SortedListNode
{
    void           *value;
    SortedListNode *prev;
    SortedListNode *next;
};

typedef struct
{
    size_t            oneElementSize;
    SlCompareFunction compare;
    SlFreeElement     elemDestructor;
    size_t            size;
    SortedListNode   *root;
} SortedList;

SortedList slInit(const size_t oneElementSize, SlCompareFunction compare, SlFreeElement elemDestructor);
void       slFree(SortedList *const self);
void       slClear(SortedList *const self);

void  slRemove(SortedList *const self, const size_t index);
void  slRemoveNoFree(SortedList *const self, const size_t index);
void  slPopBack(SortedList *const self);
void  slPopFront(SortedList *const self);
void  slInsert(SortedList *const self, void *const element);
void *slGetAt(const SortedList *const self, const size_t index);