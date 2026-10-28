#include <ui/App.h>

#include <stdio.h>
#include <sys/stat.h>

#include <rules/Changer.h>
#include <rules/ModeParse.h>
#include <rules/ModeString.h>
#include <ui/menu/ConsoleOperation.h>
#include <utils/String.h>
#include <utils/StringView.h>

static void printAllRepresentaion(mode_t mode)
{
    ModeString stringView = modeToStringRWX(mode);
    ModeString binaryView = modeToStringBin(mode);
    printf("Битовое представление:   %s\n", binaryView.mode);
    printf("Строковое представление: %s\n", stringView.mode);
    printf("Численное представление: %03o\n", mode);
}

void setContext(void *context, mode_t mode)
{
    mode_t *modeCntxt = context;
    *modeCntxt = mode;
}
mode_t getContext(void *context)
{
    return *(mode_t *)(context);
}
void printContext(void* context)
{
    // mode_t mode = getContext(context);
    // if(mode == INVALID_MODE)
    //     return;
    // printf("Текущий контекст:\n");
    // printAllRepresentaion(mode);
    // puts("");
}

ConsoleOperation convertRulers(void *context)
{
    String rulers = sInputCCO("Введите права доступа (буквенное или символьное представление): ");

    mode_t mode;
    if (interpStringToMode(svFromString(rulers), &mode) != ModeParseResultOk)
    {
        sFree(&rulers);
        puts("Неверный ввод.");
        return ConsoleOperationNone;
    }
    printAllRepresentaion(mode);
    sFree(&rulers);
    setContext(context, mode);
    puts("Сохранено в контекст");
    return ConsoleOperationNone;
}
ConsoleOperation checkFileRulers(void *context)
{
    String      file = sInputCCO("Введите путь к файлу: ");
    struct stat stt;
    if (stat(file.str, &stt) == -1)
    {

        puts("Ошибка получения данных о файле. Вероятно файла не существует.");
        sFree(&file);
        return ConsoleOperationNone;
    }
    mode_t mode = stt.st_mode & 0777;
    printAllRepresentaion(mode);
    setContext(context, mode);
    puts("Сохранено в контекст");
    return ConsoleOperationNone;
}
ConsoleOperation changeRulers(void *context)
{
    mode_t mode = getContext(context);

    if (mode == INVALID_MODE)
    {
        String      file = sInputCCO("Введите путь к файлу: ");
        struct stat stt;
        if (stat(file.str, &stt) == -1)
        {
            puts("Файл не найден");
            sFree(&file);
            return ConsoleOperationNone;
        }
        mode = stt.st_mode & 0777;
        sFree(&file);
    }
    else
        puts("Данные подгружены из контекста");

    String command = sInputCCO("Введите аргументы\nchmod ");
    if (!changeMode(&mode, svFromString(command)))
    {
        sFree(&command);
        puts("Неверный аргумент. Контекст не изменён.");
        return ConsoleOperationNone;
    }
    sFree(&command);
    setContext(context, mode);
    puts("Новый контекст сохранён");
    return ConsoleOperationNone;
}
ConsoleOperation clearContextRules(void *context)
{
    setContext(context, INVALID_MODE);
    puts("Контекст очищен.");
    return ConsoleOperationNone;
}
ConsoleOperation printContextRules(void *context)
{
    mode_t mode = getContext(context);
    printAllRepresentaion(mode);
    return ConsoleOperationNone;
}
ConsoleOperation closeAppRulers(void *context)
{
    return ConsoleOperationCloseApp;
}
ConsoleOperation clearConsoleRulers(void *context)
{
    return ConsoleOperationClearConsole;
}