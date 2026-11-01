#include <string.h>
#include <teleboock/Contact.h>

#include <utils/Compars.h>
#include <utils/String.h>

int cmpContact(const void *const left, const void *const right)
{
    const Contact *const lc = left;
    const Contact *const rc = right;
    return cmpString(&lc->firstName, &rc->firstName);
}
Contact contEmptyContact()
{
    Contact res;
    memset(&res, 0, sizeof(Contact));
    return res;
}
void contInit(Contact *const self, const String firstName, const String secondName)
{
    if (!self)
        return;
    self->firstName = firstName;
    self->secondName = secondName;
    self->thirdName = sInitEmpty();
    self->workPlace = sInitEmpty();
    self->workPost = sInitEmpty();
    self->phonesNumbers = slInit(sizeof(String), cmpString, sFreeVoid);
    self->emails = slInit(sizeof(String), cmpString, sFreeVoid);
    self->webLinks = slInit(sizeof(String), cmpString, sFreeVoid);
    self->profiles = slInit(sizeof(String), cmpString, sFreeVoid);
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
    self->phonesNumbers = slInit(sizeof(String), cmpString, sFreeVoid);
    self->emails = slInit(sizeof(String), cmpString, sFreeVoid);
    self->webLinks = slInit(sizeof(String), cmpString, sFreeVoid);
    self->profiles = slInit(sizeof(String), cmpString, sFreeVoid);
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

    slFree(&self->phonesNumbers);
    slFree(&self->emails);
    slFree(&self->webLinks);
    slFree(&self->profiles);
}
void constFreeContactVoid(void *const self)
{
    constFreeContact(self);
}
