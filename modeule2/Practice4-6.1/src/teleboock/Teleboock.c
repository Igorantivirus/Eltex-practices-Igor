#include <teleboock/Contact.h>
#include <teleboock/Teleboock.h>


void tbInit(Teleboock *const self)
{
    if (!self)
        return;
    *self = slInit(sizeof(Contact), cmpContact, constFreeContactVoid);
}

void tbFree(Teleboock *const self)
{
    if (!self)
        return;
    slFree(self);
}
