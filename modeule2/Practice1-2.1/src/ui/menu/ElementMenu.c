#include <ui/menu/ElementMenu.h>

MenuElements fillMenuElements(void *context, const char *const title, const size_t size, ...)
{
    MenuElements elements;
    elements.title = title;
    elements.context = context;
    elements.size = size;

    elements.elements = malloc(sizeof(ElementMenu) * size);

    va_list args;
    va_start(args, size);
    for (size_t i = 0; i < size; ++i)
    {
        elements.elements[i].pointName = va_arg(args, char *);
        elements.elements[i].operation = va_arg(args, Operation);
    }
    va_end(args);

    return elements;
}
