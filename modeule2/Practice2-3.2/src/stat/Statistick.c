#include <stdint.h>
#include <stdio.h>

#include <stat/Statistick.h>
#include <ui/InputArgs.h>
#include <utils/UInt32Array.h>

// public:

bool fillStatistick(InputArgs args, Statistick *st)
{
    if (!iaIsValid(&args) || !st)
        return false;
    st->fillCount = args.count;
    st->countToMyNetwork = 0;
    st->countToOtherNetwork = 0;
    st->countToMyNetworkPercents = 0.;
    st->countToOtherNetworkPercents = 0.;
    UInt32Array array = ui32aFillRand(args.count);
    uint32_t    network = args.gateway & args.mask;
    for (uint32_t i = 0; i < args.count; ++i)
    {
        uint32_t prNwtwork = args.mask & array.array[i];
        if (prNwtwork == network)
            ++st->countToMyNetwork;
    }
    ui32aFree(&array);
    st->countToOtherNetwork = st->fillCount - st->countToMyNetwork;

    st->countToMyNetworkPercents = 100. * st->countToMyNetwork / st->fillCount;
    st->countToOtherNetworkPercents = 100. * st->countToOtherNetwork / st->fillCount;

    return true;
}

void statPrint(Statistick *st)
{
    if (!st)
        return;
    printf("Число сгенерированных ip:        %u\n", st->fillCount);
    printf("Число ip в указанной подсети:    %u (%f %%)\n", st->countToMyNetwork, st->countToMyNetworkPercents);
    printf("Число ip не в указанной подсети: %u (%f %%)\n", st->countToOtherNetwork, st->countToOtherNetworkPercents);
}
