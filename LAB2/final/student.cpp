// Тема 2: Производност и наследяване. Вградени класове.
// CAddress (вграден клас) + CStudent (наследник на CPerson от тема 1).

#include <cctype>     // isdigit
#include <cstdio>     // getchar
#include <iostream>
#include <string>
using namespace std;

// ===== Изброим тип от тема 1 =====
enum Gender
{
    MALE,       // мъж
    FEMALE,     // жена
    UNKNOWN     // неопределен
};

// ============================================================
//  Класът CPerson от тема 1 — БАЗОВ клас за CStudent
// ============================================================
class CPerson
{
// protected вместо private: наследниците виждат данните,
// но за външния свят те остават скрити както при private.
protected:
    string m_strName;
    Gender m_gender;
    string m_strEGN;

public:
    CPerson();
    CPerson(const string& strName, const string& strGender, const string& strEGN);
    CPerson(const CPerson& pers);                  // копиращ конструктор

    void SetName(const string& strName);
    void SetGender(Gender gender);
    void SetGender(const string& strGender);
    bool SetEGN(const string& strEGN);

    const string& GetName() const;
    Gender GetGender() const;
    const string& GetEGN() const;

    Gender stringToGender(const string& strGender) const;
    string genderToString() const;
    bool isValid(const string& strEGN);

    ostream& Output(ostream& toStream) const;
};

// ---------- Конструктори ----------
CPerson::CPerson()
    : m_strName("Неизвестен"), m_gender(UNKNOWN), m_strEGN("")
{
}

CPerson::CPerson(const string& strName, const string& strGender, const string& strEGN)
    : m_strName(strName), m_gender(UNKNOWN), m_strEGN("")
{
    m_gender = stringToGender(strGender);
    SetEGN(strEGN);
}

// Копиращ конструктор: създава нов обект като точно копие на подадения.
CPerson::CPerson(const CPerson& pers)
    : m_strName(pers.m_strName), m_gender(pers.m_gender), m_strEGN(pers.m_strEGN)
{
}

// ---------- Презапис ----------
void CPerson::SetName(const string& strName)      { m_strName = strName; }
void CPerson::SetGender(Gender gender)            { m_gender = gender; }
void CPerson::SetGender(const string& strGender)  { m_gender = stringToGender(strGender); }

bool CPerson::SetEGN(const string& strEGN)
{
    if (!isValid(strEGN))
        return false;
    m_strEGN = strEGN;
    return true;
}

// ---------- Четене ----------
const string& CPerson::GetName() const   { return m_strName; }
Gender        CPerson::GetGender() const { return m_gender; }
const string& CPerson::GetEGN() const    { return m_strEGN; }

// ---------- Преобразуване на изброимия тип ----------
Gender CPerson::stringToGender(const string& strGender) const
{
    if (strGender == "мъж" || strGender == "м" || strGender == "male")
        return MALE;
    if (strGender == "жена" || strGender == "ж" || strGender == "female")
        return FEMALE;
    return UNKNOWN;
}

string CPerson::genderToString() const
{
    switch (m_gender)
    {
        case MALE:   return "мъж";
        case FEMALE: return "жена";
        default:     return "неопределен";
    }
}

// ---------- Валидизатор на ЕГН (алгоритъм на ГРАО) ----------
bool CPerson::isValid(const string& strEGN)
{
    if (strEGN.length() != 10)
        return false;
    for (size_t i = 0; i < strEGN.length(); i++)
        if (!isdigit(static_cast<unsigned char>(strEGN[i])))
            return false;

    int nYear  = (strEGN[0] - '0') * 10 + (strEGN[1] - '0');
    int nMonth = (strEGN[2] - '0') * 10 + (strEGN[3] - '0');
    int nDay   = (strEGN[4] - '0') * 10 + (strEGN[5] - '0');

    if (nMonth >= 1 && nMonth <= 12)        nYear += 1900;
    else if (nMonth >= 21 && nMonth <= 32) { nYear += 1800; nMonth -= 20; }
    else if (nMonth >= 41 && nMonth <= 52) { nYear += 2000; nMonth -= 40; }
    else return false;

    int arrDays[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    bool bLeap = (nYear % 4 == 0 && nYear % 100 != 0) || (nYear % 400 == 0);
    if (nMonth == 2 && bLeap) arrDays[1] = 29;
    if (nDay < 1 || nDay > arrDays[nMonth - 1]) return false;

    const int arrWeights[9] = { 2, 4, 8, 5, 10, 9, 7, 3, 6 };
    int nSum = 0;
    for (int i = 0; i < 9; i++)
        nSum += (strEGN[i] - '0') * arrWeights[i];

    int nCheck = nSum % 11;
    if (nCheck == 10) nCheck = 0;
    return nCheck == (strEGN[9] - '0');
}

// ---------- Извеждане ----------
ostream& CPerson::Output(ostream& toStream) const
{
    toStream << m_strName << " (" << genderToString() << "), ЕГН: "
             << (m_strEGN.empty() ? string("(няма)") : m_strEGN);
    return toStream;
}

// ============================================================
//  I. Клас CAddress
// ============================================================
class CAddress
{
// 1-3. Частни членове
private:
    string m_strStreet;      // 1. улица, напр. "Studentska #1"
    string m_strPostCode;    // 2. пощенски код, напр. "9010"
    string m_strCity;        // 3. град, напр. "Varna"

public:
    // 1. Конструктор по подразбиране
    CAddress();
    // 2. Експлицитен конструктор
    CAddress(const string& strStreet, const string& strPostCode, const string& strCity);
    // 3. Копиращ конструктор
    CAddress(const CAddress& addr);

    // Функции за четене / презапис
    const string& GetStreet() const;
    const string& GetPostCode() const;
    const string& GetCity() const;
    void SetStreet(const string& strStreet);
    void SetPostCode(const string& strPostCode);
    void SetCity(const string& strCity);

    // 4. Функция за извеждане
    ostream& Output(ostream& toStream) const;
};

// ---------- 1. Конструктор по подразбиране ----------
CAddress::CAddress()
    : m_strStreet("(няма улица)"), m_strPostCode("0000"), m_strCity("(няма град)")
{
}

// ---------- 2. Експлицитен конструктор ----------
CAddress::CAddress(const string& strStreet, const string& strPostCode, const string& strCity)
    : m_strStreet(strStreet), m_strPostCode(strPostCode), m_strCity(strCity)
{
}

// ---------- 3. Копиращ конструктор ----------
CAddress::CAddress(const CAddress& addr)
    : m_strStreet(addr.m_strStreet),
      m_strPostCode(addr.m_strPostCode),
      m_strCity(addr.m_strCity)
{
}

// ---------- Четене / презапис ----------
const string& CAddress::GetStreet() const   { return m_strStreet; }
const string& CAddress::GetPostCode() const { return m_strPostCode; }
const string& CAddress::GetCity() const     { return m_strCity; }

void CAddress::SetStreet(const string& strStreet)     { m_strStreet = strStreet; }
void CAddress::SetPostCode(const string& strPostCode) { m_strPostCode = strPostCode; }
void CAddress::SetCity(const string& strCity)         { m_strCity = strCity; }

// ---------- 4. Извеждане ----------
// Връща потока, за да може да се вериги: addr.Output(cout) << endl;
ostream& CAddress::Output(ostream& toStream) const
{
    toStream << m_strCity << " " << m_strPostCode << ", " << m_strStreet;
    return toStream;
}

// ============================================================
//  II. Клас CStudent - наследник на CPerson
// ============================================================
class CStudent : public CPerson
{
// 1-2. Частни членове
private:
    string   m_strFN;       // 1. факултетен номер
    CAddress m_address;     // 2. ВГРАДЕН обект от класа CAddress

public:
    // 1. Конструктор по подразбиране
    CStudent();
    // 2. Експлицитни конструктори
    CStudent(const string& strName, const CAddress& addr, const string& strEGN);
    CStudent(const string& strName, const CAddress& addr, const string& strEGN,
             const string& strFN);
    // 3. Копиращ конструктор
    CStudent(const CStudent& stud);

    // 5. Функции за четене / презапис
    const string& GetFN() const;
    const CAddress& GetAddress() const;
    void SetFN(const string& strFN);
    void SetAddress(const CAddress& addr);

    // 4. Функция за извеждане
    ostream& Output(ostream& toStream) const;
};

// ---------- 1. Конструктор по подразбиране ----------
// CPerson() и CAddress() се викат сами; списъкът е за яснота.
CStudent::CStudent()
    : CPerson(), m_strFN("(няма)"), m_address()
{
}

// ---------- 2. Експлицитен конструктор по име, адрес и ЕГН ----------
CStudent::CStudent(const string& strName, const CAddress& addr, const string& strEGN)
    : CPerson(strName, "неопределен", strEGN), m_strFN("(няма)"), m_address(addr)
{
}

// ---------- 2. Експлицитен конструктор по име, адрес, ЕГН и фак. номер ----------
CStudent::CStudent(const string& strName, const CAddress& addr, const string& strEGN,
                   const string& strFN)
    : CPerson(strName, "неопределен", strEGN), m_strFN(strFN), m_address(addr)
{
}

// ---------- 3. Копиращ конструктор ----------
// CPerson(stud) копира наследената част, m_address(stud.m_address) - вградената.
CStudent::CStudent(const CStudent& stud)
    : CPerson(stud), m_strFN(stud.m_strFN), m_address(stud.m_address)
{
}

// ---------- 5. Четене / презапис ----------
const string&   CStudent::GetFN() const      { return m_strFN; }
const CAddress& CStudent::GetAddress() const { return m_address; }

void CStudent::SetFN(const string& strFN)      { m_strFN = strFN; }
void CStudent::SetAddress(const CAddress& addr) { m_address = addr; }

// ---------- 4. Извеждане ----------
// Първо наследената част (CPerson::Output), после собствената.
ostream& CStudent::Output(ostream& toStream) const
{
    CPerson::Output(toStream);
    toStream << ", ф.н.: " << m_strFN << ", адрес: ";
    m_address.Output(toStream);
    return toStream;
}

// ============================================================
//  III. Главна функция
// ============================================================
int main()
{
    // ===== A. Обекти от класа CAddress =====
    cout << "===== A. Класът CAddress =====" << endl;

    // а) трите конструктора
    CAddress addr1;                                          // подразбиращ се
    CAddress addr2("Studentska #1", "9010", "Varna");        // експлицитен
    CAddress addr3(addr2);                                   // копиращ

    cout << "  addr1 (подразбиращ се): "; addr1.Output(cout) << endl;
    cout << "  addr2 (експлицитен):    "; addr2.Output(cout) << endl;
    cout << "  addr3 (копие на addr2): "; addr3.Output(cout) << endl;

    // б) копието е независимо от оригинала
    cout << "--- Тест: копието е независимо ---" << endl;
    addr3.SetStreet("Studentska #2");
    cout << "  addr2 след промяна на addr3: "; addr2.Output(cout) << endl;
    cout << "  addr3 след промяна:          "; addr3.Output(cout) << endl;

    // в) масив от обекти
    cout << "--- Масив от адреси ---" << endl;
    CAddress arrAddr[3] = {
        CAddress("Studentska #1", "9010", "Varna"),
        CAddress("Tsar Osvoboditel #12", "1000", "Sofia"),
        CAddress()
    };
    const int nAddrCount = sizeof(arrAddr) / sizeof(arrAddr[0]);
    for (int i = 0; i < nAddrCount; i++)
    {
        cout << "  [" << i << "] ";
        arrAddr[i].Output(cout) << endl;
    }

    // г) указател в динамичната памет
    cout << "--- Указател (динамична памет) ---" << endl;
    CAddress* pAddr = new CAddress("Slivnitsa #33", "9002", "Varna");
    cout << "  ";
    pAddr->Output(cout) << endl;
    delete pAddr;
    pAddr = nullptr;

    // д) референция
    cout << "--- Референция ---" << endl;
    CAddress& refAddr = addr1;
    refAddr.SetCity("Burgas");
    refAddr.SetPostCode("8000");
    cout << "  addr1 след промяна през refAddr: "; addr1.Output(cout) << endl;

    // е) константен обект - само const функциите
    cout << "--- Константен обект ---" << endl;
    const CAddress cAddr("Studentska #1", "9010", "Varna");
    cout << "  град: " << cAddr.GetCity() << ", код: " << cAddr.GetPostCode() << endl;
    // cAddr.SetCity("Ruse");        // грешка: обектът е константен

    // ===== Б. Обекти от класа CStudent =====
    cout << endl << "===== Б. Класът CStudent =====" << endl;

    // а) четирите конструктора
    CStudent stud1;                                                  // подразбиращ се
    CStudent stud2("Иван Петров", addr2, "1111111110");              // име, адрес, ЕГН
    CStudent stud3("Елена Димитрова", addr2, "1111111104", "21621301");
    CStudent stud4(stud3);                                           // копиращ

    cout << "  stud1: "; stud1.Output(cout) << endl;
    cout << "  stud2: "; stud2.Output(cout) << endl;
    cout << "  stud3: "; stud3.Output(cout) << endl;
    cout << "  stud4: "; stud4.Output(cout) << endl;

    // б) наследените функции работят върху наследника
    cout << "--- Наследените функции на CPerson ---" << endl;
    stud2.SetGender("мъж");
    stud3.SetGender(FEMALE);
    stud2.SetFN("21621300");
    cout << "  stud2: "; stud2.Output(cout) << endl;
    cout << "  stud3: "; stud3.Output(cout) << endl;
    cout << "  GetName() на stud3: " << stud3.GetName() << endl;
    cout << "  genderToString() на stud3: " << stud3.genderToString() << endl;

    // в) копието е независимо
    cout << "--- Тест: копието е независимо ---" << endl;
    stud4.SetName("Друго име");
    stud4.SetFN("00000000");
    cout << "  stud3 (оригинал): "; stud3.Output(cout) << endl;
    cout << "  stud4 (копие):    "; stud4.Output(cout) << endl;

    // г) масив от студенти
    cout << "--- Масив от студенти ---" << endl;
    CStudent arrStud[3] = {
        CStudent("Георги Стоянов", arrAddr[0], "1111111110", "21621302"),
        CStudent("Мария Иванова", arrAddr[1], "1111111104", "21621303"),
        CStudent()
    };
    const int nStudCount = sizeof(arrStud) / sizeof(arrStud[0]);
    for (int i = 0; i < nStudCount; i++)
    {
        cout << "  [" << i << "] ";
        arrStud[i].Output(cout) << endl;
    }

    // д) указател в динамичната памет
    cout << "--- Указател (динамична памет) ---" << endl;
    CStudent* pStud = new CStudent("Петър Колев", addr2, "1111111110", "21621304");
    cout << "  ";
    pStud->Output(cout) << endl;
    delete pStud;
    pStud = nullptr;

    // е) указател към базовия клас сочи към наследник
    cout << "--- Указател към базовия клас ---" << endl;
    CPerson* pPerson = &stud3;              // CStudent Е CPerson
    cout << "  през CPerson*: ";
    pPerson->Output(cout) << endl;          // вика CPerson::Output, НЕ CStudent::Output

    // ж) референция
    cout << "--- Референция ---" << endl;
    CStudent& refStud = stud2;
    refStud.SetAddress(CAddress("Slivnitsa #33", "9002", "Varna"));
    cout << "  stud2 след промяна през refStud: "; stud2.Output(cout) << endl;

    // з) константен обект
    cout << "--- Константен обект ---" << endl;
    const CStudent cStud("Анна Тодорова", addr2, "1111111104", "21621305");
    cout << "  ";
    cStud.Output(cout) << endl;
    // cStud.SetFN("11111111");        // грешка: обектът е константен

    // и) вграденият обект се чете през GetAddress()
    cout << "--- Достъп до вградения обект ---" << endl;
    cout << "  градът на cStud: " << cStud.GetAddress().GetCity() << endl;

    // Задържа конзолния прозорец отворен при стартиране с F5 (debug)
    getchar();
    return 0;
}
