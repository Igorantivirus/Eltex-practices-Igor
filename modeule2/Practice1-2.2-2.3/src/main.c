#include <ui/Calc.h>

int main(void)
{
    CalcAppConfig config = {
        .title       = "Калькулятор",
        .enter       = "Введите выражение (или 'exit' для выхода): ",
        .exit        = "Выход из программы.",
        .error       = "Ошибка: неверное выражение.",
        .result      = "Результат: ",
        .exitCommand = "exit",
    };

    calcApp(&config);

    return 0;
}