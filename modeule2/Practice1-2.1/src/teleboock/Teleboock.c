#include <teleboock/Teleboock.h>
#include <teleboock/Contact.h>

void tbInit(Teleboock *const self)
{
    if (!self)
        return;
    *self = daInitType(Contact, constFreeContactVoid);
}

void tbFree(Teleboock *const self)
{
    if (!self)
        return;
    daFree(self);
}
