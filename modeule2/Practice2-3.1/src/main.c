#include "rules/Changer.h"
#include "rules/ModeParse.h"
#include "rules/ModeString.h"
#include "utils/String.h"
#include "utils/StringView.h"
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <ui/App.h>
#include <ui/menu/MainMenu.h>

// int main(void)
// {
//     mode_t modeContext = INVALID_MODE;

//     MenuElements elems = fillMenuElements(
//         &modeContext, printContext, "Права файлов.", 7,
//         "Сконвертировать права", convertRulers,
//         "Проверить права файла", checkFileRulers,
//         "Изменить права", changeRulers,
//         "Очистить сохранённые права", clearContextRules,
//         "Вывести сохранённые права", printContextRules,
//         "Очистить консоль", clearConsoleRulers,
//         "Закрыть приложение", closeAppRulers);

//     startApp(elems);

//     freeMenuElements(&elems);

//     return 0;
// }

static void printAllRepresentaion(mode_t mode)
{
    ModeString stringView = modeToStringRWX(mode);
    ModeString binaryView = modeToStringBin(mode);
    printf("Битовое представление:   %s\n", binaryView.mode);
    printf("Строковое представление: %s\n", stringView.mode);
    printf("Численное представление: %03o\n", mode);
}

char *checkArgument(int argc, char **argv, int *iptr, const char *check)
{
    int i = *iptr;
    if (strcmp(argv[i], check) == 0 && i + 1 < argc)
    {
        ++(*iptr);
        return argv[i + 1];
    }
    return NULL;
}

bool convertRulersCall(StringView sv, mode_t *saved)
{
    mode_t mode;
    if (interpStringToMode(sv, &mode) != ModeParseResultOk)
    {
        puts("Неверный ввод.");
        return false;
    }
    printAllRepresentaion(mode);
    *saved = mode;
    return true;
}

bool checkFileRulersCall(StringView sv, mode_t *saved)
{
    struct stat stt;
    if (stat(sv.str, &stt) == -1)
    {

        puts("Ошибка получения данных о файле. Вероятно файла не существует.");
        return false;
    }
    *saved = stt.st_mode & 0777;
    printAllRepresentaion(*saved);
    return true;
}

bool changeRulersCall(StringView sv, mode_t *saved)
{
    mode_t mode = *saved;

    if (mode == INVALID_MODE)
    {
        String      file = sInputCCO("Введите путь к файлу: ");
        struct stat stt;
        if (stat(sv.str, &stt) == -1)
        {
            sFree(&file);
            puts("Файл не найден");
            return false;
        }
        mode = stt.st_mode & 0777;
        sFree(&file);
    }
    else
        puts("Данные подгружены из контекста");

    if (!changeMode(&mode, sv))
    {
        puts("Неверный аргумент. Контекст не изменён.");
        return false;
    }
    printAllRepresentaion(mode);
    return ConsoleOperationNone;
}

int main(int argc, char **argv)
{
    mode_t mode;
    for (int i = 0; i < argc; ++i)
    {
        char *arg;
        if ((arg = checkArgument(argc, argv, &i, "-c")))
            if (!convertRulersCall(svFromChars(arg), &mode))
                return -1;
        if ((arg = checkArgument(argc, argv, &i, "-f")))
            if (!checkFileRulersCall(svFromChars(arg), &mode))
                return -1;
        if ((arg = checkArgument(argc, argv, &i, "-chmod")))
            if (!changeRulersCall(svFromChars(arg), &mode))
                return -1;
    }
    return 0;
}