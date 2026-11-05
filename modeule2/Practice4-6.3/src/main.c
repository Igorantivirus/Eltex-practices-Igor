#include <stdbool.h>
#include <stdlib.h>

#include <ui/Calc.h>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Первый аргумент должен быть - путь к папке с библиотеками");
        return EXIT_FAILURE;
    }

    CalcAppConfig config = {
        .libsPath = argv[1],
        .libsError = "Ошибка: н удалось загрузить библиотеку",
        .title = "Калькулятор",
        .enter = "Введите выражение (или 'exit' для выхода): ",
        .exit = "Выход из программы.",
        .error = "Ошибка: неверное выражение.",
        .result = "Результат: ",
        .exitCommand = "exit",
    };

    calcApp(&config);

    return EXIT_SUCCESS;
}