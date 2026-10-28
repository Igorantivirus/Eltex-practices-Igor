#include <math.h>

#include <calc/Callbacks.h>
#include <utils/BinTree.h>

double addF(double left, double right)
{
    return left + right;
}
double subF(double left, double right)
{
    return left - right;
}
double mulF(double left, double right)
{
    return left * right;
}
double divF(double left, double right)
{
    if (right == 0)
        return nan("");
    return left / right;
}
double powF(double left, double right)
{
    return pow(left, right);
}

static int charCompare(const void *a, const void *b)
{
    const char ca = *(const char *)a;
    const char cb = *(const char *)b;
    return (ca > cb) - (ca < cb);
}

BinTree initDefaultBinOpTree()
{
    BinTree binOpTree = btInitType(char, OperatorInfo, charCompare, NULL);

    OperatorInfo oper;
    char         op;

    oper.function = addF;
    oper.priority = 1;
    op = '+';
    btInsert(&binOpTree, &op, &oper);

    oper.function = subF;
    oper.priority = 1;
    op = '-';
    btInsert(&binOpTree, &op, &oper);

    oper.function = mulF;
    oper.priority = 2;
    op = '*';
    btInsert(&binOpTree, &op, &oper);

    oper.function = divF;
    oper.priority = 2;
    op = '/';
    btInsert(&binOpTree, &op, &oper);

    oper.function = powF;
    oper.priority = 3;
    op = '^';
    btInsert(&binOpTree, &op, &oper);

    return binOpTree;
}
