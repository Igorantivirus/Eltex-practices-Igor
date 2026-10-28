#include "unity.h"

#include "calc/CalcNodes.h"
#include "calc/CalcTree.h"
#include "parse/Parser.h"
#include "parse/StringView.h"
#include <math.h>
#include <ui/BaseCalcBTInit.h>

double test(char *const expression)
{
    CallTree tree;
    ctInit(&tree, NULL, initDefaultBinOpTree());
    Node *node = parse(svFromCharsArray(expression), &tree.info);
    ctChangeNodeRoot(&tree, node);
    double resultNum = calc(&tree);
    ctFree(&tree);
    return resultNum;
}