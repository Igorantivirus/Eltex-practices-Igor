#include <ui/menu/ElementMenu.h>

#include <stdarg.h>
#include <stdlib.h>

MenuElements fillMenuElements(void *context, ContextOperationBeroreNext oper, const char *const title, const size_t size, ...)
{
    MenuElements elements;
    elements.title = title;
    elements.context = context;
    elements.size = size;
    elements.oper = oper;

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
void freeMenuElements(MenuElements *const self)
{
    if (!self)
        return;
    free(self->elements);
    self->elements = NULL;
    self->context = NULL;
    self->title = NULL;
    self->size = 0;
}