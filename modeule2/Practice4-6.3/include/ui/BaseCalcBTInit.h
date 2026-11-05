#pragma once

#include <calc/Callbacks.h>
#include <calc/DynamicLibrarys.h>
#include <utils/BinTree.h>

double addF(double left, double right);
double subF(double left, double right);
double mulF(double left, double right);
double divF(double left, double right);
double powF(double left, double right);

BinTree initOpersByLibs(const DynamicLibrarys *const dl);
BinTree initDefaultBinOpTree();