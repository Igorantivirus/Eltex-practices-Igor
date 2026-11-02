#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <queue/CycleQueue.h>

typedef struct
{
    int32_t     minPriority;
    int32_t     maxPriority;
    uint32_t    capacity;
    uint32_t    size;
    CycleQueue *queues;
} PriorityCycleQueue;

PriorityCycleQueue pcqInit(int32_t minPriority, int32_t maxPriority);
void               pcqFree(PriorityCycleQueue *const self);
bool               pcqPush(PriorityCycleQueue *const self, const QType elem, int32_t priority);
bool               pcqPop(PriorityCycleQueue *const self, QType *const v, int32_t *const priority);
bool               pcqPopPriority(PriorityCycleQueue *const self, QType *const v, int32_t priority);
bool               pcqPopAtLeast(PriorityCycleQueue *const self, QType *const v, int32_t priority, int32_t *const resPriority);
