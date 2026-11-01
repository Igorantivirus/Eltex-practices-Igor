#include <ui/InputArgs.h>

// public:

InputArgs iaInit(void)
{
    InputArgs res;
    res.countInputed = false;
    res.maskInputed = false;
    res.gatewayInputed = false;
    res.seedInputed = false;
    return res;
}

bool iaIsValid(InputArgs *const self)
{
    return self->countInputed && self->gatewayInputed && self->maskInputed;
}
