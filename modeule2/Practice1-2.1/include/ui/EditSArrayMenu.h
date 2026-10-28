#pragma once

#include <ui/menu/MainMenu.h>
#include <utils/DynamicArray.h>
#include <utils/String.h>

// void *context == DynamicArray<String>*;

ConsoleOperation rewriteValueSA(void *context);
ConsoleOperation deleteValueSA(void *context);
ConsoleOperation pushBackValueSA(void *context);
ConsoleOperation addValueSA(void *context);
ConsoleOperation clearAllSA(void *context);
ConsoleOperation rewriteAllSA(void *context);
ConsoleOperation closeMenuSA(void *context);
