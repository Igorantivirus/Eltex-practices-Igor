#pragma once

typedef struct Node Node;

typedef enum
{
    NodeTypeValue,
    NodeTypeBinOp,
} NodeType;

///////////////////////////
typedef struct
{
    double value;
} ValueNode;

typedef struct
{
    char  op;
    Node *left;
    Node *right;
} BinOpNode;
///////////////////////////

typedef union
{
    ValueNode value;
    BinOpNode binop;
} NodeData;

struct Node
{
    NodeType type;
    NodeData data;
};