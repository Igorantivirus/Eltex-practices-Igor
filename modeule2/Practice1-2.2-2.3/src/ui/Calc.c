#include <math.h>
#include <stdio.h>

#include <calc/CalcTree.h>
#include <parse/Parser.h>
#include <parse/StringView.h>
#include <utils/String.h>

#include <ui/Calc.h>
#include <ui/BaseCalcBTInit.h>

void calcApp(const CalcAppConfig *const config)
{
    puts(config->title);

    CallTree tree;
    ctInit(&tree, NULL, initDefaultBinOpTree());

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
}
