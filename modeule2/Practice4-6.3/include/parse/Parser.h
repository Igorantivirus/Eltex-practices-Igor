#pragma once

#include <calc/CalcNodes.h>
#include <utils/BinTree.h>

#include "StringView.h"
#include "calc/Callbacks.h"

// Разбор выражения: возвращает корень AST или NULL (ошибка синтаксиса)
Node *parse(StringView expr, CallTreeAdditionalInfo* info);




