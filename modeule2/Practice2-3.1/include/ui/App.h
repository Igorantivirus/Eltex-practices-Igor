#pragma once

#include <sys/types.h>

#include <ui/menu/ConsoleOperation.h>

#define INVALID_MODE (mode_t)(~(mode_t)(0))

void setContext(void *context, mode_t mode);
mode_t getContext(void *context);
void printContext(void* context);

ConsoleOperation convertRulers(void *context);
ConsoleOperation checkFileRulers(void *context);
ConsoleOperation changeRulers(void *context);
ConsoleOperation clearContextRules(void *context);
ConsoleOperation printContextRules(void *context);
ConsoleOperation closeAppRulers(void *context);
ConsoleOperation clearConsoleRulers(void *context);