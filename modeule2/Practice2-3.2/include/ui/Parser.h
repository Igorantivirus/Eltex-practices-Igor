#pragma once

#include <stdbool.h>
#include <stdint.h>

bool parseUInt32(const char *arg, uint32_t *val);
bool parseIp(const char *str, uint32_t *ip, const char **end);
