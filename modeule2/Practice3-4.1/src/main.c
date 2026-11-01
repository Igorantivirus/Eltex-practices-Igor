#include <teleboock/Teleboock.h>
#include <ui/MainMenu.h>
#include <ui/menu/MainMenu.h>

int main()
{
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif

    Teleboock tb;
    tbInit(&tb);

    startApp(fillMenuElements(
        &tb, "Главное меню", 6,
        "Добавить контакт", addContact,
        "Редактировать контакт", editContact,
        "Удалить контакт", delContact,
        "Вывести контакты", printContacts,
        "Очистить консоль", clearConsole,
        "Выход", closeApp));

    tbFree(&tb);

    return 0;
}