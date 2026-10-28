#pragma once

#include <teleboock/Contact.h>
#include <ui/menu/MainMenu.h>

#include "EditSArrayMenu.h"

// void *context == Contact*

ConsoleOperation editFirstName(void *context);
ConsoleOperation editSecondName(void *context);
ConsoleOperation editThirdName(void *context);
ConsoleOperation editWorkPlace(void *context);
ConsoleOperation editWorkPost(void *context);

ConsoleOperation editPhoneNumbers(void *context);
ConsoleOperation editEmails(void *context);
ConsoleOperation editWebLink(void *context);
ConsoleOperation editProfiles(void *context);
ConsoleOperation closeEditMenu(void *context);
