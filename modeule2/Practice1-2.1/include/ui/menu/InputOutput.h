#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <teleboock/Contact.h>
#include <utils/DynamicArray.h>

uint32_t inReadUInt32(const char *const text, uint32_t defErr);

DynamicArray inReadStringArray(const char *const text, const char *const next);

bool inReadBool(const char *const text);

void inReadContact(Contact *cnt);

void outPrintContact(const Contact *cnt, const char *const text);
