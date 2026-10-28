#include <ui/EditSArrayMenu.h>

#include <ui/menu/InputOutput.h>

#define saveAsDynamicArray(from, to) \
    if (!from)                       \
        return ConsoleOperationNone; \
    DynamicArray *to = from;

// void *context == DynamicArray<String>*;

ConsoleOperation rewriteValueSA(void *context)
{
    saveAsDynamicArray(context, array);
    uint32_t index = inReadUInt32("Введите индекс: ", 0);
    if (index == 0 || index > array->size)
    {
        printf("Неверный индекс\n");
        return ConsoleOperationNone;
    }
    String newValue = sInputCCO("Введите новое значение: ");
    daSetAt(array, index - 1,&newValue);
    return ConsoleOperationNone;
}
ConsoleOperation deleteValueSA(void *context)
{
    saveAsDynamicArray(context, array);
    uint32_t index = inReadUInt32("Введите индекс: ", 0);
    if (index == 0 || index > array->size)
    {
        printf("Неверный индекс\n");
        return ConsoleOperationNone;
    }
    daRemove(array, index - 1);
    return ConsoleOperationNone;
}
ConsoleOperation pushBackValueSA(void *context)
{
    saveAsDynamicArray(context, array);
    uint32_t index = inReadUInt32("Введите индекс: ", 0);
    if (index == 0 || index > array->size)
    {
        printf("Неверный индекс\n");
        return ConsoleOperationNone;
    }
    daPushBackValue(array, sInputCCO("Введите новое значение: "), String);
    return ConsoleOperationNone;
}
ConsoleOperation addValueSA(void *context)
{
    saveAsDynamicArray(context, array);
    uint32_t index = inReadUInt32("Введите индекс (может быть на 1 больше размера для вставки в конец): ", 0);
    if (index == 0 || index > array->size + 1)
    {
        printf("Неверный индекс\n");
        return ConsoleOperationNone;
    }
    daInsertValue(array, index - 1, sInputCCO("Введите новое значение: "), String);
    return ConsoleOperationNone;
}
ConsoleOperation clearAllSA(void *context)
{
    saveAsDynamicArray(context, array);
    daClear(array);
    return ConsoleOperationCloseApp;
}
ConsoleOperation rewriteAllSA(void *context)
{
    saveAsDynamicArray(context, array);
    daFree(array);
    *array = inReadStringArray("Введите новый массив (пустая строка для завершения)", "Далее: ");
    return ConsoleOperationCloseApp;
}
ConsoleOperation closeMenuSA(void *context)
{
    return ConsoleOperationCloseApp;
}
