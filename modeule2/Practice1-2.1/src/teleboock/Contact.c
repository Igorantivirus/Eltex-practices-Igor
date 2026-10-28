#include <teleboock/Contact.h>

#include <utils/String.h>

void contInit(Contact *const self, const String firstName, const String secondName)
{
    if (!self)
        return;
    self->firstName = firstName;
    self->secondName = secondName;
    self->thirdName = sInitEmpty();
    self->workPlace = sInitEmpty();
    self->workPost = sInitEmpty();
    self->phonesNumbers = daInitType(String, sFreeVoid);
    self->emails = daInitType(String, sFreeVoid);
    self->webLinks = daInitType(String, sFreeVoid);
    self->profiles = daInitType(String, sFreeVoid);
}
void contInitWithInfo(
    Contact *const self,
    const String   firstName,
    const String   secondName,
    const String   thirdName,
    const String   workPlace,
    const String   workPost)
{
    if (!self)
        return;
    self->firstName = firstName;
    self->secondName = secondName;
    self->thirdName = thirdName;
    self->workPlace = workPlace;
    self->workPost = workPost;
    self->phonesNumbers = daInitType(String, sFreeVoid);
    self->emails = daInitType(String, sFreeVoid);
    self->webLinks = daInitType(String, sFreeVoid);
    self->profiles = daInitType(String, sFreeVoid);
}

void constFreeContact(Contact *const self)
{
    if (!self)
        return;
    sFree(&self->firstName);
    sFree(&self->secondName);
    sFree(&self->thirdName);
    sFree(&self->workPlace);
    sFree(&self->workPost);

    daFree(&self->phonesNumbers);
    daFree(&self->emails);
    daFree(&self->webLinks);
    daFree(&self->profiles);
}
void constFreeContactVoid(void *const self)
{
    constFreeContact(self);
}
