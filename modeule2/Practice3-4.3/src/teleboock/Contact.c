#include <string.h>
#include <teleboock/Contact.h>

#include <utils/Compars.h>
#include <utils/String.h>

static int cmpField(const String *const left, const String *const right)
{
    const String l = {left->str ? left->str : "", 0, 0};
    const String r = {right->str ? right->str : "", 0, 0};
    return cmpString(&l, &r);
}

static int cmpStringList(const SortedList *const left, const SortedList *const right)
{
    const size_t n = left->size < right->size ? left->size : right->size;
    for (size_t i = 0; i < n; i++)
    {
        const int res = cmpField(slGetAt(left, i), slGetAt(right, i));
        if (res != 0)
            return res;
    }
    return (left->size > right->size) - (left->size < right->size);
}

int cmpContact(const void *const left, const void *const right)
{
    const Contact *const lc = left;
    const Contact *const rc = right;
    int                  res;

    if ((res = cmpField(&lc->secondName, &rc->secondName)) != 0)
        return res;
    if ((res = cmpField(&lc->firstName, &rc->firstName)) != 0)
        return res;
    if ((res = cmpField(&lc->thirdName, &rc->thirdName)) != 0)
        return res;
    if ((res = cmpField(&lc->workPlace, &rc->workPlace)) != 0)
        return res;
    if ((res = cmpField(&lc->workPost, &rc->workPost)) != 0)
        return res;
    if ((res = cmpStringList(&lc->phonesNumbers, &rc->phonesNumbers)) != 0)
        return res;
    if ((res = cmpStringList(&lc->emails, &rc->emails)) != 0)
        return res;
    if ((res = cmpStringList(&lc->webLinks, &rc->webLinks)) != 0)
        return res;
    return cmpStringList(&lc->profiles, &rc->profiles);
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
