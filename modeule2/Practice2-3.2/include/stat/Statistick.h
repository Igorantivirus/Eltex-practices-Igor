#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <ui/InputArgs.h>

typedef struct
{
    uint32_t fillCount;                   // Число сгенерированных IP
    uint32_t countToMyNetwork;            // Число IP в сети
    uint32_t countToOtherNetwork;         // Число IP не в сети
    double   countToMyNetworkPercents;    // Процент IP в сети
    double   countToOtherNetworkPercents; // Процент IP не в сети
} Statistick;

bool fillStatistick(InputArgs args, Statistick *st);
void statPrint(Statistick *st);
