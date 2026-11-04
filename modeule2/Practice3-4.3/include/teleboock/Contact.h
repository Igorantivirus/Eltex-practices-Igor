#pragma once

#include <utils/SortedList.h>
#include <utils/String.h>

typedef struct
{
    String firstName;  // Имя
    String secondName; // Фамилия
    String thirdName;  // Отчество

    String workPlace; // Место работы
    String workPost;  // Должность

    SortedList /*String*/ phonesNumbers; // Телефонные номера
    SortedList /*String*/ emails;        // Почты
    SortedList /*String*/ webLinks;      // Ссылки на соц. сети
    SortedList /*String*/ profiles;      // Мессенджеры

} Contact;

int cmpContact(const void *const left, const void *const right);

Contact contEmptyContact();
void contInit(Contact *const self, const String firstName, const String secondName);
void contInitWithInfo(
    Contact *const self,
    const String   firstName,
    const String   secondName,
    const String   thirdName,
    const String   workPlace,
    const String   workPost);

void constFreeContact(Contact *const self);
void constFreeContactVoid(void *const self);
