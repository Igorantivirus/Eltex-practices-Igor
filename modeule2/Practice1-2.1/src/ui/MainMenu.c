#include <ui/MainMenu.h>

#include <ui/EditContactMenu.h>
#include <ui/menu/InputOutput.h>

#define saveTeleboock(from, to)      \
    if (!from)                       \
        return ConsoleOperationNone; \
    Teleboock *to = from;

ConsoleOperation addContact(void *context)
{
    saveTeleboock(context, tb);
    Contact cnt;
    do
    {
        inReadContact(&cnt);
    } while (inReadBool("Сохранить контакт? (y/n): ") == false);
    daPushBack(tb, &cnt);
    return ConsoleOperationNone;
}
ConsoleOperation editContact(void *context)
{
    saveTeleboock(context, tb);

    uint32_t pos = inReadUInt32("Введите id для изменения: ", 0);
    if (pos <= 0 || pos > tb->size)
    {
        printf("Неверный индекс\n");
        return ConsoleOperationNone;
    }
    Contact *cnt = daGetAt(tb, pos - 1);
    if (!cnt)
    {
        printf("Ошибка, просим прощения.");
        return ConsoleOperationNone;
    }
    startApp(fillMenuElements(
        cnt,
        "Редактирование контакта", 9,
        "Изменить имя", editFirstName,
        "Изменить фамилию", editSecondName,
        "Изменить отчество", editThirdName,
        "Изменить место работы", editWorkPlace,
        "Изменить должность", editWorkPost,
        "Изменить номера телефонов", editPhoneNumbers,
        "Изменить ссылки", editWebLink,
        "Изменить мессенджеры", editPhoneNumbers,
        "Закрыть", closeEditMenu));
    return ConsoleOperationNone;
}
ConsoleOperation delContact(void *context)
{
    saveTeleboock(context, tb);
    uint32_t pos = inReadUInt32("Введите индекс для удаления: ", 0);
    if (pos <= 0 || pos > tb->size)
    {
        printf("Индекс вне диапазона\n");
        return ConsoleOperationNone;
    }
    daRemove(tb, pos - 1);
    return ConsoleOperationNone;
}

// Вспомогательная работа приложения
ConsoleOperation printContacts(void *context)
{
    saveTeleboock(context, tb);
    for (int i = 0; i < tb->size; i++)
    {
        Contact *cnt = daGetAt(tb, i);
        outPrintContact(cnt, "Контакт:");
    }
    return ConsoleOperationNone;
}
ConsoleOperation clearConsole(void *context)
{
    saveTeleboock(context, tb);
    return ConsoleOperationClearConsole;
}
ConsoleOperation closeApp(void *context)
{
    saveTeleboock(context, tb);
    return ConsoleOperationCloseApp;
}
