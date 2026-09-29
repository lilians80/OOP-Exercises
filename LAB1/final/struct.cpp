// Тема 1: Въведение в ООП. Структури на C срещу класове на C++.

// Visual Studio отказва strncpy без този ред. Трябва да е ПРЕДИ include-овете.
#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>     // getchar
#include <cstring>    // strncpy
#include <iostream>
using namespace std;

// ===== I. Структура CPerson (масиви от символи, стил C) =====
struct CPerson
{
    // Членовете на struct са публични по подразбиране.
    // 64 байта = 32 букви: кирилицата в UTF-8 е по 2 байта на буква.
    char m_szName[64];
    char m_szGender[32];

    // 3. Подразбиращ се конструктор
    CPerson() { SetName("Неизвестен"); SetGender("неопределен"); }

    // 3. Експлицитен конструктор (с параметри)
    CPerson(const char* szName, const char* szGender)
    { SetName(szName); SetGender(szGender); }

    // 1. Функции за запис; strncpy + ръчна нула => няма препълване
    void SetName(const char* sz)
    {
        if (!sz) sz = "";
        strncpy(m_szName, sz, sizeof(m_szName) - 1);
        m_szName[sizeof(m_szName) - 1] = '\0';
    }
    void SetGender(const char* sz)
    {
        if (!sz) sz = "";
        strncpy(m_szGender, sz, sizeof(m_szGender) - 1);
        m_szGender[sizeof(m_szGender) - 1] = '\0';
    }

    // 2. Функции за четене; const => не променят обекта
    const char* GetName() const { return m_szName; }
    const char* GetGender() const { return m_szGender; }

    void Print() const
    {
        cout << GetName() << " (" << GetGender() << ")" << endl;
    }
};

// ===== 4. Главна функция: променливи от различни типове =====
int main()
{
    // а) обикновени обекти - двата конструктора
    CPerson person1;                          // подразбиращ се конструктор
    CPerson person2("Иван Петров", "мъж");    // експлицитен конструктор

    cout << "--- Обикновени обекти ---" << endl;
    person1.Print();
    person2.Print();

    // б) промяна на данните чрез функциите за запис
    cout << "--- След промяна ---" << endl;
    person1.SetName("Мария Иванова");
    person1.SetGender("жена");
    person1.Print();

    // в) масив от обекти
    CPerson people[3] = {
        CPerson("Георги Стоянов", "мъж"),
        CPerson("Елена Димитрова", "жена"),
        CPerson()                             // подразбиращите се стойности
    };

    cout << "--- Масив от обекти ---" << endl;
    const int nCount = sizeof(people) / sizeof(people[0]);
    for (int i = 0; i < nCount; i++)
        people[i].Print();

    // г) указател към обект в динамичната памет
    cout << "--- Указател (динамична памет) ---" << endl;
    CPerson* pPerson = new CPerson("Петър Колев", "мъж");
    pPerson->Print();                         // достъп чрез ->
    delete pPerson;                           // освобождаване на паметта
    pPerson = nullptr;

    // д) референция - псевдоним на съществуващ обект
    cout << "--- Референция ---" << endl;
    CPerson& refPerson = person2;
    refPerson.SetGender("неопределен");        // променя самия person2
    person2.Print();

    // е) константен обект - позволени са само const функциите за четене
    cout << "--- Константен обект ---" << endl;
    const CPerson cPerson("Анна Тодорова", "жена");
    cout << cPerson.GetName() << " / " << cPerson.GetGender() << endl;
    // cPerson.SetName("Друго");              // грешка: обектът е константен

    // Задържа конзолния прозорец отворен при стартиране с F5 (debug)
    getchar();
    return 0;
}
