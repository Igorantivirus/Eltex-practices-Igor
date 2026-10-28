#pragma once

#include <utils/DynamicArray.h>
#include <utils/String.h>

typedef struct
{
    String firstName;  // Имя
    String secondName; // Фамилия
    String thirdName;  // Отчество

    String workPlace; // Место работы
    String workPost;  // Должность

    DynamicArray /*String*/ phonesNumbers; // Телефонные номера
    DynamicArray /*String*/ emails;        // Почты
    DynamicArray /*String*/ webLinks;      // Ссылки на соц. сети
    DynamicArray /*String*/ profiles;      // Мессенджеры

} Contact;

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
