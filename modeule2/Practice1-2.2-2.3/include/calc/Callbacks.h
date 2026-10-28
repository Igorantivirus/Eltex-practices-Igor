#pragma once

#include <utils/BinTree.h>

#include "CalcNodes.h"

typedef struct CallTreeAdditionalInfo CallTreeAdditionalInfo;

typedef double (*DoubleOperandFunction)(double left, double right);
typedef void   (*NodeFreeFunction)(NodeData *data, CallTreeAdditionalInfo *info); // Очистить ноду по её типу
typedef double (*NodeCalcFunction)(NodeData *data, CallTreeAdditionalInfo *info); // Вызывать ноду по её типу

typedef struct
{
    NodeFreeFunction free;
    NodeCalcFunction calc;
} NodeCallBacks;

typedef struct
{
    DoubleOperandFunction function;
    uint32_t              priority;
} OperatorInfo;

struct CallTreeAdditionalInfo
{
    BinTree binOpTree; // <char, OperatorInfo>; Бинарные операции
    BinTree callBacks; // <NodeType, NodeCallBacks>; Информация по типу: как удалять, как вызывать
};

Node *nodeInit(NodeType type);
void  nodeFree(Node *node, CallTreeAdditionalInfo *info);
double nodeCalc(Node *node, CallTreeAdditionalInfo *const info);

// ValueNode
void freeValueNode(NodeData *data, CallTreeAdditionalInfo *info);
double calcValueNode(NodeData *data, CallTreeAdditionalInfo *info);
// OpNode
void freeBinOpNode(NodeData *data, CallTreeAdditionalInfo *info);
double calcBinOpNode(NodeData *data, CallTreeAdditionalInfo *info);