#include <calc/DynamicLibrarys.h>

#include "calc/LibraryContext.h"
#include "utils/DynamicArray.h"
#include <stdio.h>
#include <string.h>

static bool isDynamicLibrary(const char *const path)
{
    FILE *f = fopen(path, "rb");
    if (!f)
        return false;

    ElfW(Ehdr) ehdr;
    const size_t n = fread(&ehdr, 1, sizeof(ehdr), f);
    fclose(f);

    if (n != sizeof(ehdr))
        return false;
    if (memcmp(ehdr.e_ident, ELFMAG, SELFMAG) != 0)
        return false;

    return ehdr.e_type == ET_DYN;
}

bool dlInit(DynamicLibrarys *const dl, const char *const path)
{
    if (!dl || !path)
        return false;

    DIR *const dir = opendir(path);
    if (!dir)
        return false;

    *dl = daInit(sizeof(LibraryContext), clFreeVoid);

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char      libPath[PATH_MAX];
        const int written = snprintf(libPath, sizeof(libPath), "%s/%s", path, entry->d_name);
        if (written < 0 || (size_t)written >= sizeof(libPath))
            continue;

        struct stat st;
        if (stat(libPath, &st) == -1 || !S_ISREG(st.st_mode))
            continue;

        if (!isDynamicLibrary(libPath))
            continue;

        LibraryContext context;
        if (!clInit(&context, libPath))
            continue;

        if (!daPushBack(dl, &context))
            clFree(&context);
    }

    closedir(dir);
    return true;
}

bool dlFree(DynamicLibrarys *const dl)
{
    daFree(dl);
}