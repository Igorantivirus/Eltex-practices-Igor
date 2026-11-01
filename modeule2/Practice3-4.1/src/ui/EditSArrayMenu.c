#include "utils/SortedList.h"
#include <ui/EditSArrayMenu.h>

#include <ui/menu/InputOutput.h>

#define saveAsDynamicArray(from, to) \
    if (!from)                       \
        return ConsoleOperationNone; \
    SortedList *to = from;

// void *context == DynamicArray<String>*;

ConsoleOperation deleteValueSA(void *context)
{
    saveAsDynamicArray(context, array);
    uint32_t index = inReadUInt32("Введите индекс: ", 0);
    if (index == 0 || index > array->size)
    {
        printf("Неверный индекс\n");
        return ConsoleOperationNone;
    }
    slRemove(array, index - 1);
    return ConsoleOperationNone;
}
ConsoleOperation addValueSA(void *context)
{
    saveAsDynamicArray(context, array);
    String res = sInputCCO("Введите новое значение: ");
    slInsert(array, &res);
    return ConsoleOperationNone;
}
ConsoleOperation clearAllSA(void *context)
{
    saveAsDynamicArray(context, array);
    slClear(array);
    return ConsoleOperationCloseApp;
}
ConsoleOperation rewriteAllSA(void *context)
{
    saveAsDynamicArray(context, array);
    slFree(array);
    *array = inReadStringArray("Введите новый массив (пустая строка для завершения)", "Далее: ");
    return ConsoleOperationCloseApp;
}
ConsoleOperation closeMenuSA(void *context)
{
    return ConsoleOperationCloseApp;
}
