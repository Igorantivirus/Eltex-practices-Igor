#include <utils/SortedList.h>

#include <stdlib.h>
#include <string.h>

static void clearList(SortedListNode *node, SlFreeElement elemDestructor)
{
    SortedListNode *next = NULL;
    while (node)
    {
        next = node->next;
        if (elemDestructor)
            elemDestructor(node->value);
        free(node);
        node = next;
    }
}

static SortedListNode *initNode(void *const value, const size_t oneElemSize, SortedListNode *prev, SortedListNode *next)
{
    SortedListNode *node = malloc(sizeof(SortedListNode));
    node->next = next;
    node->prev = prev;
    node->value = malloc(oneElemSize);
    memcpy(node->value, value, oneElemSize);
    return node;
}

SortedList slInit(const size_t oneElementSize, SlCompareFunction compare, SlFreeElement elemDestructor)
{
    SortedList res;
    res.oneElementSize = oneElementSize;
    res.compare = compare;
    res.elemDestructor = elemDestructor;
    res.size = 0;
    res.root = NULL;
    return res;
}

void slFree(SortedList *const self)
{
    if (!self)
        return;

    clearList(self->root, self->elemDestructor);
    memset(self, 0, sizeof(SortedList));
}

void slClear(SortedList *const self)
{
    if (!self)
        return;

    clearList(self->root, self->elemDestructor);
    self->root = NULL;
    self->size = 0;
}

void slRemove(SortedList *const self, const size_t index)
{
    if (!self || index >= self->size)
        return;

    SortedListNode *prev = NULL;
    SortedListNode *node = self->root;
    size_t          i = 0;

    while (i != index && node)
    {
        prev = node;
        node = node->next;
        ++i;
    }

    if (i != index || !node)
        return;

    if (prev)
        prev->next = node->next;
    else
        self->root = node->next;

    if (node->next)
        node->next->prev = prev;

    if (self->elemDestructor)
        self->elemDestructor(node->value);

    free(node);
    --self->size;
}
void  slRemoveNoFree(SortedList *const self, const size_t index)
{
    if (!self || index >= self->size)
        return;

    SortedListNode *prev = NULL;
    SortedListNode *node = self->root;
    size_t          i = 0;

    while (i != index && node)
    {
        prev = node;
        node = node->next;
        ++i;
    }

    if (i != index || !node)
        return;

    if (prev)
        prev->next = node->next;
    else
        self->root = node->next;

    if (node->next)
        node->next->prev = prev;

    free(node);
    --self->size;
}

void slPopBack(SortedList *const self)
{
    if (!self || self->size == 0)
        return;

    slRemove(self, self->size - 1);
}

void slPopFront(SortedList *const self)
{
    if (!self)
        return;

    slRemove(self, 0);
}

void slInsert(SortedList *const self, void *const element)
{
    if (!self || !self->compare)
        return;

    SortedListNode *prev = NULL;
    SortedListNode *node = self->root;

    while (node && self->compare(node->value, element) < 0)
    {
        prev = node;
        node = node->next;
    }

    SortedListNode *newNode = initNode(element, self->oneElementSize, prev, node);

    if (prev == NULL)
        self->root = newNode;
    else
        prev->next = newNode;

    if (node != NULL)
        node->prev = newNode;

    ++self->size;
}

void *slGetAt(const SortedList *const self, const size_t index)
{
    if (!self || index >= self->size)
        return NULL;

    SortedListNode *node = self->root;
    size_t          i = 0;

    while (i != index && node)
    {
        node = node->next;
        ++i;
    }

    return (i == index && node) ? node->value : NULL;
}