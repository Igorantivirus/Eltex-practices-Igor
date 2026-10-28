#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <utils/BinTree.h>

#include <calc/Callbacks.h>

Node *nodeInit(NodeType type)
{
    Node *n = malloc(sizeof(Node));
    memset(n, 0, sizeof(Node));
    n->type = type;
    return n;
}
void nodeFree(Node *node, CallTreeAdditionalInfo *info)
{
    if(!node || !info)
        return;
    NodeCallBacks *ncb = (NodeCallBacks *)btGetAt(&info->callBacks, &node->type);
    if (!ncb)
        return;
    if(ncb->free)
        ncb->free(&node->data, info);
    free(node);
}
double nodeCalc(Node *node, CallTreeAdditionalInfo *const info)
{
    NodeCallBacks *ncb = (NodeCallBacks *)btGetAt(&info->callBacks, &node->type);
    if (!ncb)
        return nan("");
    return ncb->calc(&node->data, info);
}

// ValueNode
void freeValueNode(NodeData *data, CallTreeAdditionalInfo *info)
{
}
double calcValueNode(NodeData *data, CallTreeAdditionalInfo *info)
{
    return data ? data->value.value : nan("");
}
// OpNode
void freeBinOpNode(NodeData *data, CallTreeAdditionalInfo *info)
{
    nodeFree(data->binop.left, info);
    nodeFree(data->binop.right, info);
}
double calcBinOpNode(NodeData *data, CallTreeAdditionalInfo *info)
{
    if (!data || !info)
        return nan("");

    BinOpNode   *node = &data->binop;
    const double left = nodeCalc(node->left, info);
    const double right = nodeCalc(node->right, info);
    const char   op = node->op;

    OperatorInfo *calcFunction = (OperatorInfo *)btGetAt(&info->binOpTree, &op);
    return !calcFunction ? nan("") : calcFunction->function(left, right);
}
