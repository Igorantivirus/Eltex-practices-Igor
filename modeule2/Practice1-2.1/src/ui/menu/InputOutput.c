#include <ui/menu/InputOutput.h>

uint32_t inReadUInt32(const char *const text, uint32_t defErr)
{
    printf("%s", text);
    uint32_t res = defErr;
    scanf("%u", &res);

    int c;
    for (int c = getchar(); c != '\n' && c != EOF; c = getchar())
        ;
    return res;
}

DynamicArray inReadStringArray(const char *const text, const char *const next)
{
    printf("%s\n", text);
    DynamicArray arr = daInitType(String, sFreeVoid);
    String       str;
    for (str = sInputCCO(next); str.size > 0; str = sInputCCO(next))
        daPushBack(&arr, &str);
    return arr;
}

bool inReadBool(const char *const text)
{
    printf("%s", text);
    int c;
    do {
        c = getchar();
    } while (c == ' ' || c == '\t' || c == '\n' || c == '\r');
    if (c == EOF)
        return false;
    const bool result = (c == 'y' || c == 'Y');
    while (c != EOF && c != '\n')
        c = getchar();
    return result;
}

void inReadContact(Contact *cnt)
{
    if (!cnt)
        return;
    cnt->firstName = sInitEmpty();
    do
    {
        sFree(&cnt->firstName);
        cnt->firstName = sInputCCO("Введите имя(обязательно): ");
    } while (cnt->firstName.size == 0);
    cnt->secondName = sInitEmpty();
    do
    {
        sFree(&cnt->secondName);
        cnt->secondName = sInputCCO("Введите фамилию(обязательно): ");
    } while (cnt->secondName.size == 0);

    cnt->thirdName = sInputCCO("Введите отчество: ");
    cnt->workPlace = sInputCCO("Введите место работы: ");
    cnt->workPost = sInputCCO("Введите должность: ");

    cnt->phonesNumbers = inReadStringArray("Введите телефонные номера(пустая строка для завершения): ", "Введите телефонный номер: ");
    cnt->emails = inReadStringArray("Введите почты(пустая строка для завершения): ", "Введите почту: ");
    cnt->webLinks = inReadStringArray("Введите ссылки на соц. сети(пустая строка для завершения): ", "Введите ссылку на соц. сеть: ");
    cnt->profiles = inReadStringArray("Введите мессенджеры(пустая строка для завершения): ", "Введите мессенджер: ");
}

void outPrintContact(const Contact *cnt, const char *const text)
{
    if (!cnt)
        return;
    printf("%s\n", text);
    printf("Имя: %s\n", cnt->firstName.str);
    printf("Фамилия: %s\n", cnt->secondName.str);
    printf("Отчество: %s\n", cnt->thirdName.str);
    printf("Место работы: %s\n", cnt->workPlace.str);
    printf("Должность: %s\n", cnt->workPost.str);

    for (uint32_t i = 0; i < cnt->phonesNumbers.size; i++)
        printf("Телефонный номер #%u: %s\n", i + 1, ((String *)daGetAt(&cnt->phonesNumbers, i))->str);
    for (uint32_t i = 0; i < cnt->emails.size; i++)
        printf("Почта #%u: %s\n", i + 1, ((String *)daGetAt(&cnt->emails, i))->str);
    for (uint32_t i = 0; i < cnt->webLinks.size; i++)
        printf("Ссылка на соц. сеть #%u: %s\n", i + 1, ((String *)daGetAt(&cnt->webLinks, i))->str);
    for (uint32_t i = 0; i < cnt->profiles.size; i++)
        printf("Мессенджер #%u: %s\n", i + 1, ((String *)daGetAt(&cnt->profiles, i))->str);
}
