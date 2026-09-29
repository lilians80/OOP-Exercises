// Тема 1: Въведение в ООП. Клас CPerson2 (STL string + изброим тип).
#include <cctype>     // isdigit
#include <cstdio>     // getchar
#include <iostream>
#include <string>
using namespace std;

// ===== II. Изброим тип за кодиране на пол =====
enum Gender
{
    MALE,       // мъж
    FEMALE,     // жена
    UNKNOWN     // неопределен
};

// ===== III. Клас CPerson2 =====
class CPerson2
{
// 1. struct -> class: членовете са ПРИВАТНИ по подразбиране,
//    затова достъпът до тях става само през функциите по-долу.
private:
    string m_strName;     // 1. масивът от символи -> STL тип string
    Gender m_gender;      // 1. полът -> изброим тип вместо масив от символи
    string m_strEGN;      // 1. нова променлива: ЕГН

// 3. Публична спецификация
public:
    // 3. Подразбиращ се конструктор
    CPerson2();

    // 3. Експлицитен конструктор
    CPerson2(const string& strName, const string& strGender, const string& strEGN);

    // 3. Функции за презапис (промяна) на член променливите
    void SetName(const string& strName);
    void SetGender(Gender gender);
    void SetGender(const string& strGender);   // предефинирана: приема текст
    bool SetEGN(const string& strEGN);         // записва само валидно ЕГН

    // 3. Функции за четене; const => не променят обекта
    const string& GetName() const;
    Gender GetGender() const;
    const string& GetEGN() const;

    // 2. Преобразуване между изброимия тип и читаем текст
    Gender stringToGender(const string& strGender) const;
    string genderToString() const;

    // 3. Валидизатор на ЕГН (алгоритъм на ГРАО)
    bool isValid(const string& strEGN);

    void Print() const;
};

// ---------- 3. Конструктори ----------
CPerson2::CPerson2()
    : m_strName("Неизвестен"), m_gender(UNKNOWN), m_strEGN("")
{
}

CPerson2::CPerson2(const string& strName, const string& strGender, const string& strEGN)
    : m_strName(strName), m_gender(UNKNOWN), m_strEGN("")
{
    m_gender = stringToGender(strGender);
    SetEGN(strEGN);
}

// ---------- 3. Функции за презапис ----------
void CPerson2::SetName(const string& strName)
{
    m_strName = strName;
}

void CPerson2::SetGender(Gender gender)
{
    m_gender = gender;
}

void CPerson2::SetGender(const string& strGender)
{
    m_gender = stringToGender(strGender);
}

bool CPerson2::SetEGN(const string& strEGN)
{
    if (!isValid(strEGN))
        return false;          // невалидно ЕГН => старата стойност се запазва

    m_strEGN = strEGN;
    return true;
}

// ---------- 3. Функции за четене ----------
const string& CPerson2::GetName() const   { return m_strName; }
Gender        CPerson2::GetGender() const { return m_gender; }
const string& CPerson2::GetEGN() const    { return m_strEGN; }

// ---------- 2. Преобразуване на изброимия тип ----------
Gender CPerson2::stringToGender(const string& strGender) const
{
    if (strGender == "мъж" || strGender == "м" || strGender == "male")
        return MALE;
    if (strGender == "жена" || strGender == "ж" || strGender == "female")
        return FEMALE;
    return UNKNOWN;
}

string CPerson2::genderToString() const
{
    switch (m_gender)
    {
        case MALE:   return "мъж";
        case FEMALE: return "жена";
        default:     return "неопределен";
    }
}

// ---------- 3. Валидизатор на ЕГН ----------
// ЕГН = ГГ ММ ДД РРР К (10 цифри)
//   - месецът кодира и века: +20 => 18xx, +40 => 20xx
//   - К е контролна цифра по тегла 2,4,8,5,10,9,7,3,6
bool CPerson2::isValid(const string& strEGN)
{
    // а) точно 10 символа, само цифри
    if (strEGN.length() != 10)
        return false;
    for (size_t i = 0; i < strEGN.length(); i++)
        if (!isdigit(static_cast<unsigned char>(strEGN[i])))
            return false;

    // б) валидна дата на раждане
    int nYear  = (strEGN[0] - '0') * 10 + (strEGN[1] - '0');
    int nMonth = (strEGN[2] - '0') * 10 + (strEGN[3] - '0');
    int nDay   = (strEGN[4] - '0') * 10 + (strEGN[5] - '0');

    if (nMonth >= 1 && nMonth <= 12)
        nYear += 1900;
    else if (nMonth >= 21 && nMonth <= 32)
    {   nYear += 1800; nMonth -= 20;   }
    else if (nMonth >= 41 && nMonth <= 52)
    {   nYear += 2000; nMonth -= 40;   }
    else
        return false;                       // несъществуващ месец

    int arrDays[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    bool bLeap = (nYear % 4 == 0 && nYear % 100 != 0) || (nYear % 400 == 0);
    if (nMonth == 2 && bLeap)
        arrDays[1] = 29;
    if (nDay < 1 || nDay > arrDays[nMonth - 1])
        return false;                       // несъществуващ ден

    // в) контролна цифра
    const int arrWeights[9] = { 2, 4, 8, 5, 10, 9, 7, 3, 6 };
    int nSum = 0;
    for (int i = 0; i < 9; i++)
        nSum += (strEGN[i] - '0') * arrWeights[i];

    int nCheck = nSum % 11;
    if (nCheck == 10)
        nCheck = 0;

    return nCheck == (strEGN[9] - '0');
}

void CPerson2::Print() const
{
    cout << GetName() << " (" << genderToString() << "), ЕГН: "
         << (GetEGN().empty() ? string("(няма)") : GetEGN()) << endl;
}

// ===== 5. Главна функция: тестове =====
int main()
{
    // --- а) обикновени обекти: двата конструктора ---
    cout << "--- Обикновени обекти ---" << endl;
    CPerson2 person1;                                            // подразбиращ се
    CPerson2 person2("Иван Петров", "мъж", "1111111110");        // експлицитен
    person1.Print();
    person2.Print();

    // --- б) промяна чрез функциите за презапис ---
    cout << "--- След промяна ---" << endl;
    person1.SetName("Мария Иванова");
    person1.SetGender(FEMALE);                                   // чрез изброимия тип
    person1.SetEGN("1111111104");
    person1.Print();

    // --- в) масив от обекти ---
    cout << "--- Масив от обекти ---" << endl;
    CPerson2 people[3] = {
        CPerson2("Георги Стоянов", "м", "1111111110"),
        CPerson2("Елена Димитрова", "ж", "1111111104"),
        CPerson2()
    };
    const int nCount = sizeof(people) / sizeof(people[0]);
    for (int i = 0; i < nCount; i++)
        people[i].Print();

    // --- г) указател към обект в динамичната памет ---
    cout << "--- Указател (динамична памет) ---" << endl;
    CPerson2* pPerson = new CPerson2("Петър Колев", "мъж", "1111111110");
    pPerson->Print();
    delete pPerson;
    pPerson = nullptr;

    // --- д) референция: псевдоним на съществуващ обект ---
    cout << "--- Референция ---" << endl;
    CPerson2& refPerson = person2;
    refPerson.SetGender("жена");                                 // променя самия person2
    person2.Print();

    // --- е) константен обект: само const функции ---
    cout << "--- Константен обект ---" << endl;
    const CPerson2 cPerson("Анна Тодорова", "жена", "1111111104");
    cout << cPerson.GetName() << " / " << cPerson.genderToString()
         << " / " << cPerson.GetEGN() << endl;
    // cPerson.SetName("Друго");        // грешка: обектът е константен

    // --- ж) тест на stringToGender / genderToString ---
    cout << "--- Тест: текст -> Gender -> текст ---" << endl;
    const char* arrTests[] = { "мъж", "жена", "м", "ж", "male", "female", "нещо" };
    CPerson2 tester;
    for (size_t i = 0; i < sizeof(arrTests) / sizeof(arrTests[0]); i++)
    {
        tester.SetGender(string(arrTests[i]));
        cout << "  \"" << arrTests[i] << "\" -> " << tester.GetGender()
             << " -> \"" << tester.genderToString() << "\"" << endl;
    }

    // --- з) тест на валидизатора на ЕГН ---
    cout << "--- Тест: валидност на ЕГН ---" << endl;
    const char* arrEGN[] = {
        "1111111110",   // валидно (пример от условието)
        "1111111104",   // валидно (пример от условието)
        "1111111111",   // грешна контролна цифра
        "111111111",    // по-малко от 10 цифри
        "111111111a",   // съдържа буква
        "1199111110",   // несъществуващ месец (99)
        "1111321110"    // несъществуващ ден (32)
    };
    CPerson2 validator;
    for (size_t i = 0; i < sizeof(arrEGN) / sizeof(arrEGN[0]); i++)
        cout << "  " << arrEGN[i] << " -> "
             << (validator.isValid(arrEGN[i]) ? "валидно" : "НЕвалидно") << endl;

    // --- и) SetEGN отхвърля невалидна стойност ---
    cout << "--- Тест: SetEGN пази старата стойност ---" << endl;
    CPerson2 person3("Стефан Николов", "мъж", "1111111110");
    cout << "  преди:  " << person3.GetEGN() << endl;
    bool bResult = person3.SetEGN("1234567890");                 // невалидно
    cout << "  SetEGN(\"1234567890\") върна: " << (bResult ? "true" : "false") << endl;
    cout << "  след:   " << person3.GetEGN() << endl;

    // Задържа конзолния прозорец отворен при стартиране с F5 (debug)
    getchar();
    return 0;
}
