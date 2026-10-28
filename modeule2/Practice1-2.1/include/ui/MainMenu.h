#pragma once

#include <teleboock/Teleboock.h>
#include <ui/menu/MainMenu.h>
#include <utils/DynamicArray.h>

ConsoleOperation addContact(void *context);
ConsoleOperation editContact(void *context);
ConsoleOperation delContact(void *context);

// Вспомогательная работа приложения
ConsoleOperation printContacts(void *context);
ConsoleOperation clearConsole(void *context);
ConsoleOperation closeApp(void *context);
