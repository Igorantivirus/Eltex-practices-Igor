#include <teleboock/Contact.h>
#include <teleboock/Teleboock.h>


void tbInit(Teleboock *const self)
{
    if (!self)
        return;
    *self = bltInit(sizeof(Contact), cmpContact, constFreeContactVoid);
}

void tbFree(Teleboock *const self)
{
    if (!self)
        return;
    bltFree(self);
}
