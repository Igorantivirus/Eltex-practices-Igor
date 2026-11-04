#pragma once

#include <utils/BalancedTree.h>

typedef BalancedTree /*Contact*/ Teleboock; // ИЗМЕНЕНО: было SortedList

void tbInit(Teleboock *const self);
void tbFree(Teleboock *const self);
