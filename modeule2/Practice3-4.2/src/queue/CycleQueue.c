#include <queue/CycleQueue.h>

#include <stdlib.h>
#include <string.h>

static void reverse(QType *array, uint32_t start, uint32_t end)
{
    QType pr;
    while (start < end)
    {
        pr = array[start];
        array[start] = array[end];
        array[end] = pr;
        ++start;
        --end;
    }
}

bool cqInit(CycleQueue *const self, uint32_t size)
{
    if (!self)
        return false;
    self->size = 0;
    self->headElem = 0;
    self->tailElem = 0;
    self->queue = malloc(size * sizeof(QType));
    self->capacity = self->queue ? size : 0;
    return self->queue != NULL;
}
void cqFree(CycleQueue *const self)
{
    if (!self)
        return;
    free(self->queue);
    memset(self, 0, sizeof(CycleQueue));
}

bool cqPush(CycleQueue *const self, const QType v)
{
    if (!self)
        return false;
    if (self->size == self->capacity) // Место кончилось
    {
        uint32_t newCapacity = NEXT_CYCLE_QUEUE_SIZE(self->capacity);
        QType   *newQueue = realloc(self->queue, sizeof(QType) * newCapacity);
        if (!newQueue)
            return false;
        self->queue = newQueue;
        if (self->tailElem != 0) // Если цикл не выстроен линейно, выравнивание
        {
            reverse(self->queue, 0, self->tailElem - 1);
            reverse(self->queue, self->tailElem, self->capacity - 1);
            reverse(self->queue, 0, self->capacity - 1);
        }
        self->capacity = newCapacity;
        self->tailElem = 0;
        self->headElem = self->size;
    }
    // Тут точно есть место для записи
    self->queue[self->headElem] = v;
    self->headElem = (self->headElem == (self->capacity - 1)) ? 0 : (self->headElem + 1);
    ++self->size;
    return true;
}

bool cqPop(CycleQueue *const self, QType *const v)
{
    if (!self || self->size == 0)
        return false;
    if (v)
        *v = self->queue[self->tailElem];
    --self->size;
    self->tailElem = (self->tailElem == (self->capacity - 1)) ? 0 : (self->tailElem + 1);
    if (self->size == 0)
    {
        self->tailElem = 0;
        self->headElem = 0;
    }
    return true;
}
