#pragma once

#include <stdbool.h>
#include <stdint.h>

#define QType int
#define START_CYCLE_QUEUE_SIZE 4
#define NEXT_CYCLE_QUEUE_SIZE(size) ((size) * 3 / 2)

typedef struct
{
    QType   *queue;
    uint32_t capacity;
    uint32_t size;
    uint32_t headElem; // Следующий куда пишем
    uint32_t tailElem; // Первый, который удаляем
} CycleQueue;

bool cqInit(CycleQueue *const self, uint32_t size);
void cqFree(CycleQueue *const self);
bool cqPush(CycleQueue *const self, const QType v);
bool cqPop(CycleQueue *const self, QType *const v);
