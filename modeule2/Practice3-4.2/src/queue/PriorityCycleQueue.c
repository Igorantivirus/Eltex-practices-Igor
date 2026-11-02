#include <queue/PriorityCycleQueue.h>

#include <stdlib.h>
#include <string.h>

#define return_check(expr, doing) \
    do                            \
    {                             \
        bool res = expr;          \
        if (res)                  \
            doing;                \
        return res;               \
    } while (false)

PriorityCycleQueue pcqInit(int32_t minPriority, int32_t maxPriority)
{
    PriorityCycleQueue res;
    res.minPriority = minPriority < maxPriority ? minPriority : maxPriority;
    res.maxPriority = minPriority < maxPriority ? maxPriority : minPriority;
    res.capacity = (uint32_t)(maxPriority - minPriority) + 1;
    res.size = 0;
    res.queues = malloc(res.capacity * sizeof(CycleQueue));
    if (!res.queues)
    {
        res.capacity = res.size = 0;
        return res;
    }
    for (uint32_t i = 0; i < res.capacity; ++i)
        if (!cqInit(res.queues + i, START_CYCLE_QUEUE_SIZE))
        {
            for (uint32_t j = 0; j < i; ++j)
                cqFree(res.queues + j);
            free(res.queues);
            res.queues = NULL;
            res.capacity = 0;
            break;
        }
    return res;
}

void pcqFree(PriorityCycleQueue *const self)
{
    if (!self)
        return;
    for (uint32_t i = 0; i < self->capacity; ++i)
        cqFree(self->queues + i);
    free(self->queues);
    memset(self, 0, sizeof(PriorityCycleQueue));
}

bool pcqPush(PriorityCycleQueue *const self, const QType elem, int32_t priority)
{
    if (!self || priority < self->minPriority || priority > self->maxPriority)
        return false;
    uint32_t index = priority - self->minPriority;
    return_check(cqPush(self->queues + index, elem), ++self->size);
}

bool pcqPop(PriorityCycleQueue *const self, QType *const v, int32_t *const priority)
{
    if (!self)
        return false;

    for (uint32_t i = self->capacity; i > 0; --i)
    {
        uint32_t index = i - 1;
        if (self->queues[index].size > 0)
        {
            if (priority)
                *priority = (int32_t)index + self->minPriority;
            return_check(cqPop(self->queues + index, v), --self->size);
        }
    }
    return false;
}
bool pcqPopPriority(PriorityCycleQueue *const self, QType *const v, int32_t priority)
{
    if (!self || priority < self->minPriority || priority > self->maxPriority)
        return false;
    uint32_t index = priority - self->minPriority;
    return_check(cqPop(self->queues + index, v), --self->size);
}
bool pcqPopAtLeast(PriorityCycleQueue *const self, QType *const v, int32_t priority, int32_t *const resPriority)
{
    if (!self || priority < self->minPriority || priority > self->maxPriority)
        return false;
    for (uint32_t i = self->capacity; i > priority - self->minPriority; --i)
    {
        uint32_t index = i - 1;
        if (self->queues[index].size > 0)
        {
            if (resPriority)
                *resPriority = (int32_t)index + self->minPriority;
            return_check(cqPop(self->queues + index, v), --self->size);
        }
    }
    return false;
}
