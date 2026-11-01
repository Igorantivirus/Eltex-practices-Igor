#include <stdint.h>
#include <stdlib.h>

#include <utils/Rand.h>

// public:

uint32_t randU32(void)
{
    uint32_t r = 0;
    r = (r << 15) | ((uint32_t)rand() & 0x7FFFu);
    r = (r << 15) | ((uint32_t)rand() & 0x7FFFu);
    r = (r << 15) | ((uint32_t)rand() & 0x7FFFu);
    return r;
}
