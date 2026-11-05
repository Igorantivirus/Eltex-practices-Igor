#include <ui/MainMenu.h>

#include <teleboock/Contact.h>
#include <ui/EditContactMenu.h>
#include <ui/menu/InputOutput.h>
#include <utils/BalancedTree.h>

#define saveTeleboock(from, to)      \
    if (!from)                       \
        return ConsoleOperationNone; \
    Teleboock *to = from;

ConsoleOperation addContact(void *context)
{
    saveTeleboock(context, tb);
    Contact cnt = contEmptyContact();
    do
    {
        constFreeContact(&cnt);
        inReadContact(&cnt);
    } while (inReadBool("Сохранить контакт? (y/n): ") == false);
    bltInsert(tb, &cnt);
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
    // ИЗМЕНЕНО: было Contact *cnt = slGetAt(...) и правка прямо в контейнере.
    // В дереве менять имя/фамилию «на месте» нельзя (сломается порядок), поэтому:
    // копируем контакт к себе -> вынимаем из дерева без освобождения -> редактируем -> вставляем обратно.
    const Contact *const stored = bltGetAt(tb, pos - 1);
    if (!stored)
    {
        printf("Ошибка, просим прощения.");
        return ConsoleOperationNone;
    }
    Contact  copy = *stored;
    Contact *cnt = &copy;
    bltRemoveNoFree(tb, cnt); // stored после этого недействителен, дальше работаем только с copy
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

    bltInsert(tb, cnt);

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
    bltRemove(tb, bltGetAt(tb, pos - 1));
    return ConsoleOperationNone;
}

// Вспомогательная работа приложения
ConsoleOperation printContacts(void *context)
{
    saveTeleboock(context, tb);
    for (size_t i = 0; i < tb->size; i++)
    {
        const Contact *cnt = bltGetAt(tb, i);
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
