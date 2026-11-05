#include <calc/LibraryContext.h>

#include <stdio.h>
#include <string.h>

bool clInit(LibraryContext *const self, const char *const path)
{
    if (!self || !path)
        return false;

#define fatalreturn                 \
    dlclose(self->handle);          \
    memset(self, 0, sizeof(*self)); \
    return false;

    memset(self, 0, sizeof(*self));

    self->handle = dlopen(path, RTLD_NOW | RTLD_LOCAL);
    if (!self->handle)
    {
        const char *err = dlerror();
        printf("Ошибка загрузки библиотеки %s: %s\n", path, err ? err : "unknown");
        return false;
    }

    const char *err;

    dlerror();
    LibraryFunction fn = dlsym(self->handle, "mathCalcFunction");
    err = dlerror();
    if (err || !fn)
    {
        printf("Функция mathCalcFunction в %s: %s\n", path, err ? err : "символ отсутствует");
        fatalreturn;
    }
    self->function = fn;

    dlerror();
    const char *opPtr = dlsym(self->handle, "mathCalcOperationSymbol");
    err = dlerror();
    if (err || !opPtr)
    {
        printf("Символ mathCalcOperationSymbol в %s: %s\n",
               path, err ? err : "символ отсутствует");
        fatalreturn;
    }
    self->operation = *opPtr;

    dlerror();
    const uint32_t *prPtr = dlsym(self->handle, "mathCalcPriority");
    err = dlerror();
    if (err || !prPtr)
    {
        printf("Символ mathCalcPriority в %s: %s\n", path, err ? err : "символ отсутствует");
        fatalreturn;
    }
    self->priority = *prPtr;

    return true;
#undef fatalreturn
}

void clFree(LibraryContext *const self)
{
    if (!self || !self->handle)
        return;
    dlclose(self->handle);
    memset(self, 0, sizeof(*self));
}

void clFreeVoid(void *const self)
{
    clFree((LibraryContext *)self);
}