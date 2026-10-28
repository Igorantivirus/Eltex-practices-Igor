#include <ui/menu/MainMenu.h>

#include <stdio.h>

#include <ui/menu/InputOutput.h>

int printMenuWithInput(const MenuElements *const elemets)
{
    printf("=-=%s=-=\n", elemets->title);
    for (int i = 0; i < elemets->size; ++i)
        printf("%d: %s\n", i + 1, elemets->elements[i].pointName);

    uint32_t res = 0;
    do
    {
        res = inReadUInt32("Введите действие: ", 0);
    } while (res <= 0 || res > elemets->size);
    return res - 1;
}

void startApp(MenuElements elements)
{
    while (elements.context != NULL)
    {
        int              res = printMenuWithInput(&elements);
        ConsoleOperation oper = elements.elements[res].operation(elements.context);
        if (oper == ConsoleOperationCloseApp)
            elements.context = NULL;
        else if (oper == ConsoleOperationClearConsole)
            printf("\033[2J\033[H");
    }
}
