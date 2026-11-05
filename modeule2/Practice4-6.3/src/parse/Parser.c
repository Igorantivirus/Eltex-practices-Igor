#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

#include <calc/CalcNodes.h>
#include <calc/Callbacks.h>
#include <utils/BinTree.h>

#include <parse/Parser.h>

// Структура состояния парсера
typedef struct
{
    const char             *cur;
    const char             *end;
    bool                    hasError;
    CallTreeAdditionalInfo *info;
} Parser;

// Пропуск пробельных символов
static void skipWhitespace(Parser *p)
{
    while (p->cur < p->end && isspace((unsigned char)*p->cur))
        p->cur++;
}

static Node *parseExpression(Parser *p, uint32_t minPriority);

// Неопределённый операнд (число или выражение в скобках)
static Node *parsePrimary(Parser *p)
{
    skipWhitespace(p);

    if (p->cur >= p->end)
    {
        p->hasError = true;
        return NULL; // Неожиданный конец строки (например, "2 + ")
    }

    // Обработка выражения в скобках: ( expr )
    if (*p->cur == '(')
    {
        p->cur++; // Пропускаем '('
        Node *subExpr = parseExpression(p, 0);

        skipWhitespace(p);
        if (p->hasError || p->cur >= p->end || *p->cur != ')')
        {
            // Скобка не закрыта
            p->hasError = true;
            nodeFree(subExpr, p->info);
            return NULL;
        }

        p->cur++; // Пропускаем ')'
        return subExpr;
    }

    // Обработка числа
    if (isdigit((unsigned char)*p->cur) || *p->cur == '.')
    {
        char  *endPtr = NULL;
        double val = strtod(p->cur, &endPtr);

        if (endPtr == p->cur)
        {
            p->hasError = true;
            return NULL;
        }

        p->cur = endPtr;

        // Число должно читаться целиком: "123abc" - это не корректный ввод
        if (isalnum((unsigned char)*p->cur))
        {
            p->hasError = true;
            return NULL;
        }

        Node *node = nodeInit(NodeTypeValue);
        node->data.value.value = val;
        return node;
    }

    // ДВойной оператор не символ - ошибка
    p->hasError = true;
    return NULL;
}
// Бинарные операции
static Node *parseExpression(Parser *p, uint32_t minPriority)
{
    Node *left = parsePrimary(p);
    if (!left || p->hasError)
        return NULL;

    while (true)
    {
        skipWhitespace(p);

        // Если дошли до конца или до закрывающей скобки - возвращаем левую часть наверх
        if (p->cur >= p->end || *p->cur == ')')
            break;

        char op = *p->cur;

        // Ищем операцию в дереве операций
        OperatorInfo *info = (OperatorInfo *)btGetAt(&p->info->binOpTree, &op);
        if (!info)
        {
            // Символ не является зарегистрированным оператором
            p->hasError = true;
            nodeFree(left, p->info);
            return NULL;
        }

        // Если приоритет операции меньше текущего - выходим (выполнится выше по дереву)
        if (info->priority < minPriority)
            break;

        // Поглощаем символ операции
        p->cur++;

        // Рекурсивно парсим правую часть.
        // +1 обеспечивает строгую левую ассоциативность (например: 1 - 2 - 3 == (1 - 2) - 3)
        Node *right = parseExpression(p, info->priority + 1);
        if (!right || p->hasError)
        {
            p->hasError = true;
            nodeFree(left, p->info);
            nodeFree(right, p->info);
            return NULL;
        }

        // Собираем бинарный узел
        Node *opNode = nodeInit(NodeTypeBinOp);
        opNode->data.binop.op = op;
        opNode->data.binop.left = left;
        opNode->data.binop.right = right;

        left = opNode;
    }

    return left;
}

// --- Главная точка входа ---
Node *parse(StringView expr, CallTreeAdditionalInfo *info)
{
    if (!expr.str || expr.size == 0 || !info)
        return NULL;

    Parser p;
    p.cur = expr.str;
    p.end = expr.str + expr.size;
    p.hasError = false;
    p.info = info;

    Node *root = parseExpression(&p, 0);

    skipWhitespace(&p);

    // Если остались нераспарсенные символы (например, "2 + 2 3" или лишняя ')') - это ошибка
    if (p.cur != p.end || p.hasError)
    {
        nodeFree(root, info);
        return NULL;
    }

    return root;
}
