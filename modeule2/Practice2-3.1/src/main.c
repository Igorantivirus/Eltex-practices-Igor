#include <sys/types.h>

#include <ui/App.h>
#include <ui/menu/MainMenu.h>

int main(void)
{
    mode_t modeContext = INVALID_MODE;

    MenuElements elems = fillMenuElements(
        &modeContext, printContext, "Права файлов.", 7,
        "Сконвертировать права", convertRulers,
        "Проверить права файла", checkFileRulers,
        "Изменить права", changeRulers,
        "Очистить сохранённые права", clearContextRules,
        "Вывести сохранённые права", printContextRules,
        "Очистить консоль", clearConsoleRulers,
        "Закрыть приложение", closeAppRulers);

    startApp(elems);

    freeMenuElements(&elems);

    // mode_t m = 0;

    // if (interpStringToMode(svFromChars("rw-r----x"), &m) != ModeParseResultOk)
    // {
    //     fprintf(stderr, "Ошибка парсинга\n");
    //     return 1;
    // }

    // printf("oct: %03o\n", m);
    // printf("bin: ");
    // printBinary(m);
    // printf("str: %s\n", modeToStringRWX(m).mode);

    // /* Проверим обратный путь: из восьмеричной строки */
    // mode_t m2 = 0;
    // if (interpStringToMode(svFromChars("641"), &m2) != ModeParseResultOk)
    // {
    //     fprintf(stderr, "Ошибка парсинга 2\n");
    //     return 1;
    // }
    // printf("\noct: %03o\n", m2);
    // printf("bin: ");
    // printBinary(m2);
    // printf("str: %s\n", modeToStringRWX(m2).mode);

    // /* Проверим невалидные случаи */
    // mode_t tmp;
    // printf("\ninvalid \"800\": %d\n", interpStringToMode(svFromChars("800"), &tmp));
    // printf("invalid \"rw-r--r-x-\": %d\n", interpStringToMode(svFromChars("rw-r--r-x-"), &tmp));
    // printf("invalid \"rwq-----\": %d\n", interpStringToMode(svFromChars("rwq-----"), &tmp));

    return 0;
}