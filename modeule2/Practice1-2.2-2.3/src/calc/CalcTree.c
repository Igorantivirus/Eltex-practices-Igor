#include <math.h>

#include <string.h>

#include <calc/CalcTree.h>

static int typeCompare(const void *a, const void *b)
{
    const int ca = *(const NodeType *)a;
    const int cb = *(const NodeType *)b;
    return (ca > cb) - (ca < cb);
}

void ctInit(CallTree *tree, Node *node, BinTree binOperationFunctionsTYree)
{
    tree->root = node;
    tree->info.callBacks = btInitType(NodeType, NodeCallBacks, typeCompare, NULL);
    tree->info.binOpTree = binOperationFunctionsTYree;

    NodeType type;

    NodeCallBacks valueCb;
    valueCb.free = freeValueNode;
    valueCb.calc = calcValueNode;
    type = NodeTypeValue;
    btInsert(&tree->info.callBacks, &type, &valueCb);

    NodeCallBacks binopCb;
    binopCb.free = freeBinOpNode;
    binopCb.calc = calcBinOpNode;
    type = NodeTypeBinOp;
    btInsert(&tree->info.callBacks, &type, &binopCb);
}
void ctFree(CallTree *tree)
{
    if (!tree)
        return;
    nodeFree(tree->root, &tree->info);
    btFree(&tree->info.callBacks);
    btFree(&tree->info.binOpTree);
    memset(tree, 0, sizeof(CallTree));
}

void ctChangeNodeRoot(CallTree *tree, Node *newRoot)
{
    if (!tree || !newRoot)
        return;
    nodeFree(tree->root, &tree->info);
    tree->root = newRoot;
}

double calc(CallTree *const tree)
{
    if (!tree)
        return nan("");
    return nodeCalc(tree->root, &tree->info);
}
