#pragma once

#include <stdbool.h>
#include <sys/types.h>

#include <utils/StringView.h>

bool changeMode(mode_t *mode, StringView arg);