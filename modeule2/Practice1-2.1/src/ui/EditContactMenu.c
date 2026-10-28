#include <ui/EditContactMenu.h>

#define saveAsContact(from, to)      \
    if (!from)                       \
        return ConsoleOperationNone; \
    Contact *to = from;

// void *context == Contact*

static void editString(String *str, const char *const text)
{
    if (!str)
        return;
    printf("Прошлое значение: %s\n", str->str ? str->str : "(пусто)");
    sFree(str);
    *str = sInputCCO(text);
}

static void editStringArray(DynamicArray *arr, const char *const title)
{
    startApp(fillMenuElements(
        arr, title, 7,
        "Добавить элемент", addValueSA,
        "Вставить в конец элемент", pushBackValueSA,
        "Удалить элемент", deleteValueSA,
        "Заменить элемент", rewriteValueSA,
        "Очистить массив", clearAllSA,
        "Перезаписать всё", rewriteAllSA,
        "Выйти", closeMenuSA));
}

ConsoleOperation editFirstName(void *context)
{
    saveAsContact(context, cnt);
    editString(&cnt->firstName, "Введите имя(обязательно): ");
    return ConsoleOperationNone;
}
ConsoleOperation editSecondName(void *context)
{
    saveAsContact(context, cnt);
    editString(&cnt->secondName, "Введите отчество: ");
    return ConsoleOperationNone;
}
ConsoleOperation editThirdName(void *context)
{
    saveAsContact(context, cnt);
    editString(&cnt->thirdName, "Введите отчество: ");
    return ConsoleOperationNone;
}
ConsoleOperation editWorkPlace(void *context)
{
    saveAsContact(context, cnt);
    editString(&cnt->workPlace, "Введите место работы: ");
    return ConsoleOperationNone;
}
ConsoleOperation editWorkPost(void *context)
{
    saveAsContact(context, cnt);
    editString(&cnt->workPost, "Введите должность: ");
    return ConsoleOperationNone;
}

ConsoleOperation editPhoneNumbers(void *context)
{
    saveAsContact(context, cnt);
    editStringArray(&cnt->phonesNumbers, "Изменение номеров телефона");
    return ConsoleOperationNone;
}
ConsoleOperation editEmails(void *context)
{
    saveAsContact(context, cnt);
    editStringArray(&cnt->emails, "Изменение почт");
    return ConsoleOperationNone;
}
ConsoleOperation editWebLink(void *context)
{
    saveAsContact(context, cnt);
    editStringArray(&cnt->webLinks, "Изменение ссылок на соц. сети");
    return ConsoleOperationNone;
}
ConsoleOperation editProfiles(void *context)
{
    saveAsContact(context, cnt);
    editStringArray(&cnt->profiles, "Изменение ссылок на мессенджеры");
    return ConsoleOperationNone;
}
ConsoleOperation closeEditMenu(void *context)
{
    saveAsContact(context, cnt);
    return ConsoleOperationCloseApp;
}
