#pragma once

#include <parse/Parser.h>
#include <parse/StringView.h>
#include <utils/String.h>

typedef struct
{
    const char *title;
    const char *enter;
    const char *exit;
    const char *error;
    const char *result;
    const char *exitCommand;
} CalcAppConfig;

void calcApp(const CalcAppConfig *const config);
