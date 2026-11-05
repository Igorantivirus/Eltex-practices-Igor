#pragma once

#include <calc/CalcNodes.h>
#include <calc/Callbacks.h>
#include <utils/BinTree.h>

typedef struct
{
    Node                  *root; // Корень синтаксического дерева
    CallTreeAdditionalInfo info;
} CallTree;

void ctInit(CallTree *tree, Node *node, BinTree binOperationFunctionsTYree);
void ctFree(CallTree *tree);
void ctChangeNodeRoot(CallTree *tree, Node *newRoot);
double calc(CallTree *const tree);
