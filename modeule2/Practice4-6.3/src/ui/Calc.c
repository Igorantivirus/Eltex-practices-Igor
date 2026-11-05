#include "calc/DynamicLibrarys.h"
#include "calc/LibraryContext.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include <calc/CalcTree.h>
#include <parse/Parser.h>
#include <parse/StringView.h>
#include <utils/String.h>

#include <ui/BaseCalcBTInit.h>
#include <ui/Calc.h>

void calcApp(const CalcAppConfig *const config)
{
    puts(config->title);

    DynamicLibrarys libs;
    if (!dlInit(&libs, config->libsPath))
    {
        puts(config->libsError);
        return;
    }

    for(size_t i = 0; i < libs.size; ++i)
    {
        LibraryContext* lib = daGetAt(&libs, i);
        printf("%c ", lib->operation);
    }
    printf("\n");

    CallTree tree;
    ctInit(&tree, NULL, initOpersByLibs(&libs));

    while (true)
    {
        String s = sInputCCO(config->enter);
        if (sCompareChars(&s, config->exitCommand) == 0)
        {
            sFree(&s);
            break;
        }
        Node *node = parse(svFromString(s), &tree.info);
        if (!node)
        {
            puts(config->error);
            sFree(&s);
            continue;
        }
        ctChangeNodeRoot(&tree, node);
        double resultNum = calc(&tree);
        if (isnan(resultNum))
            puts(config->error);
        else
            printf("%s%lf\n", config->result, resultNum);
        sFree(&s);
    }
    ctFree(&tree);
    dlFree(&libs);
}
