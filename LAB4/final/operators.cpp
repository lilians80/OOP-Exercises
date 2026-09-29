// Тема 4: Приятелски функции и оператори.
// Надгражда класовете CPerson, CAddress и CStudent от упражнение 3:
// функциите Output се заменят с глобални оператори <<, добавят се
// оператори за сравнение по възраст и извеждане във файл.

#include <cctype>     // isdigit
#include <cstdio>     // getchar
#include <fstream>    // fstream
#include <iomanip>    // setw, setfill
#include <iostream>
#include <sstream>    // ostringstream
#include <string>
using namespace std;

enum Gender
{
    MALE,       // мъж
    FEMALE,     // жена
    UNKNOWN     // неопределен
};

// ============================================================
//  CPerson
// ============================================================
class CPerson
{
protected:
    string m_strName;
    Gender m_gender;
    string m_strEGN;

// 2. Частна функция: разлага ЕГН на година, месец и ден.
//    Частна е, защото е вътрешна подробност - отвън се ползва GetBirthKey.
private:
    void getYMD(const string& strEGN, int& iYear, int& iMonth, int& iDay) const;

public:
    CPerson();
    CPerson(const string& strName, const string& strGender, const string& strEGN);
    CPerson(const CPerson& pers);

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

    // Дата на раждане като едно число ГГГГММДД: по-малко число = по-възрастен.
    int GetBirthKey() const;
    string GetBirthDate() const;              // "08.03.1975"

    // 3. Глобална приятелска функция (оператор) за извеждане
    friend ostream& operator<<(ostream& toStream, const CPerson& pers);
};

CPerson::CPerson()
    : m_strName("Неизвестен"), m_gender(UNKNOWN), m_strEGN("") {}

CPerson::CPerson(const string& strName, const string& strGender, const string& strEGN)
    : m_strName(strName), m_gender(UNKNOWN), m_strEGN("")
{
    m_gender = stringToGender(strGender);
    SetEGN(strEGN);
}

CPerson::CPerson(const CPerson& pers)
    : m_strName(pers.m_strName), m_gender(pers.m_gender), m_strEGN(pers.m_strEGN) {}

void CPerson::SetName(const string& strName)      { m_strName = strName; }
void CPerson::SetGender(Gender gender)            { m_gender = gender; }
void CPerson::SetGender(const string& strGender)  { m_gender = stringToGender(strGender); }

bool CPerson::SetEGN(const string& strEGN)
{
    if (!isValid(strEGN)) return false;
    m_strEGN = strEGN;
    return true;
}

const string& CPerson::GetName() const   { return m_strName; }
Gender        CPerson::GetGender() const { return m_gender; }
const string& CPerson::GetEGN() const    { return m_strEGN; }

Gender CPerson::stringToGender(const string& strGender) const
{
    if (strGender == "мъж" || strGender == "м" || strGender == "male")    return MALE;
    if (strGender == "жена" || strGender == "ж" || strGender == "female") return FEMALE;
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

bool CPerson::isValid(const string& strEGN)
{
    if (strEGN.length() != 10) return false;
    for (size_t i = 0; i < strEGN.length(); i++)
        if (!isdigit(static_cast<unsigned char>(strEGN[i]))) return false;

    int nYear, nMonth, nDay;
    getYMD(strEGN, nYear, nMonth, nDay);
    if (nYear == 0) return false;             // getYMD сигнализира грешен месец с 0

    int arrDays[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    bool bLeap = (nYear % 4 == 0 && nYear % 100 != 0) || (nYear % 400 == 0);
    if (nMonth == 2 && bLeap) arrDays[1] = 29;
    if (nDay < 1 || nDay > arrDays[nMonth - 1]) return false;

    const int arrWeights[9] = { 2, 4, 8, 5, 10, 9, 7, 3, 6 };
    int nSum = 0;
    for (int i = 0; i < 9; i++) nSum += (strEGN[i] - '0') * arrWeights[i];

    int nCheck = nSum % 11;
    if (nCheck == 10) nCheck = 0;
    return nCheck == (strEGN[9] - '0');
}

// ---------- 2. Частната функция getYMD ----------
// Месецът в ЕГН кодира и века: +20 => 18xx, +40 => 20xx.
// При невалиден месец връща iYear = 0 - това е сигналът за грешка.
void CPerson::getYMD(const string& strEGN, int& iYear, int& iMonth, int& iDay) const
{
    iYear = iMonth = iDay = 0;
    if (strEGN.length() != 10) return;

    int nYear  = (strEGN[0] - '0') * 10 + (strEGN[1] - '0');
    int nMonth = (strEGN[2] - '0') * 10 + (strEGN[3] - '0');
    int nDay   = (strEGN[4] - '0') * 10 + (strEGN[5] - '0');

    if (nMonth >= 1 && nMonth <= 12)        nYear += 1900;
    else if (nMonth >= 21 && nMonth <= 32) { nYear += 1800; nMonth -= 20; }
    else if (nMonth >= 41 && nMonth <= 52) { nYear += 2000; nMonth -= 40; }
    else return;                              // остава iYear = 0

    iYear  = nYear;
    iMonth = nMonth;
    iDay   = nDay;
}

int CPerson::GetBirthKey() const
{
    int nYear, nMonth, nDay;
    getYMD(m_strEGN, nYear, nMonth, nDay);
    if (nYear == 0) return 0;                 // няма или невалидно ЕГН
    return nYear * 10000 + nMonth * 100 + nDay;
}

string CPerson::GetBirthDate() const
{
    int nYear, nMonth, nDay;
    getYMD(m_strEGN, nYear, nMonth, nDay);
    if (nYear == 0) return "(няма)";

    // ostringstream е поток, който пише в низ вместо на екрана.
    ostringstream oss;
    oss << setfill('0') << setw(2) << nDay  << "."
                        << setw(2) << nMonth << "."
                        << setw(4) << nYear;
    return oss.str();
}

// ---------- 3. Оператор за извеждане на личност ----------
ostream& operator<<(ostream& toStream, const CPerson& pers)
{
    toStream << pers.m_strName << " (" << pers.genderToString() << "), ЕГН: "
             << (pers.m_strEGN.empty() ? string("(няма)") : pers.m_strEGN)
             << ", роден: " << pers.GetBirthDate();
    return toStream;
}

// ============================================================
//  CAddress
// ============================================================
class CAddress
{
private:
    string m_strStreet;
    string m_strPostCode;
    string m_strCity;

public:
    CAddress();
    CAddress(const string& strStreet, const string& strPostCode, const string& strCity);
    CAddress(const CAddress& addr);

    const string& GetStreet() const;
    const string& GetPostCode() const;
    const string& GetCity() const;
    void SetStreet(const string& strStreet);
    void SetPostCode(const string& strPostCode);
    void SetCity(const string& strCity);

    // 1. Глобална приятелска функция (оператор) за извеждане
    friend ostream& operator<<(ostream& toStream, const CAddress& addr);
};

CAddress::CAddress()
    : m_strStreet("(няма улица)"), m_strPostCode("0000"), m_strCity("(няма град)") {}

CAddress::CAddress(const string& strStreet, const string& strPostCode, const string& strCity)
    : m_strStreet(strStreet), m_strPostCode(strPostCode), m_strCity(strCity) {}

CAddress::CAddress(const CAddress& addr)
    : m_strStreet(addr.m_strStreet), m_strPostCode(addr.m_strPostCode),
      m_strCity(addr.m_strCity) {}

const string& CAddress::GetStreet() const   { return m_strStreet; }
const string& CAddress::GetPostCode() const { return m_strPostCode; }
const string& CAddress::GetCity() const     { return m_strCity; }

void CAddress::SetStreet(const string& strStreet)     { m_strStreet = strStreet; }
void CAddress::SetPostCode(const string& strPostCode) { m_strPostCode = strPostCode; }
void CAddress::SetCity(const string& strCity)         { m_strCity = strCity; }

// ---------- 1. Оператор за извеждане на адрес ----------
ostream& operator<<(ostream& toStream, const CAddress& addr)
{
    toStream << addr.m_strCity << " " << addr.m_strPostCode << ", " << addr.m_strStreet;
    return toStream;
}

// ============================================================
//  CStudent
// ============================================================
class CStudent : public CPerson
{
private:
    string   m_strFN;
    CAddress m_address;

    double*  m_pdGrades;
    size_t   m_nCount;
    size_t   m_nCapacity;

    void grow();

public:
    CStudent();
    CStudent(const string& strName, const CAddress& addr, const string& strEGN);
    CStudent(const string& strName, const CAddress& addr, const string& strEGN,
             const string& strFN);
    CStudent(const CStudent& stud);
    CStudent& operator=(const CStudent& stud);
    ~CStudent();

    void   AddGrade(double dGrade);
    double GetGrade(size_t nIndex) const;
    size_t GetGradeCount() const;
    double GetAverage() const;

    const string&   GetFN() const;
    const CAddress& GetAddress() const;
    void SetFN(const string& strFN);
    void SetAddress(const CAddress& addr);

    // 4. Приятелска функция за конзолен вход към ЕГН
    friend void ConsoleInputEGN(CStudent& stud);

    // 5. Приятелски оператор за извеждане
    friend ostream& operator<<(ostream& toStream, const CStudent& stud);

    // 6. Приятелски оператори за сравнение ПО ВЪЗРАСТ
    friend bool operator==(const CStudent& studL, const CStudent& studR);
    friend bool operator!=(const CStudent& studL, const CStudent& studR);
    friend bool operator< (const CStudent& studL, const CStudent& studR);
    friend bool operator>=(const CStudent& studL, const CStudent& studR);
};

void CStudent::grow()
{
    size_t nNew = (m_nCapacity == 0) ? 4 : m_nCapacity * 2;
    double* pdNew = new double[nNew];
    for (size_t i = 0; i < m_nCount; i++) pdNew[i] = m_pdGrades[i];
    delete[] m_pdGrades;
    m_pdGrades  = pdNew;
    m_nCapacity = nNew;
}

CStudent::CStudent()
    : CPerson(), m_strFN("(няма)"), m_address(),
      m_pdGrades(nullptr), m_nCount(0), m_nCapacity(0) {}

CStudent::CStudent(const string& strName, const CAddress& addr, const string& strEGN)
    : CPerson(strName, "неопределен", strEGN), m_strFN("(няма)"), m_address(addr),
      m_pdGrades(nullptr), m_nCount(0), m_nCapacity(0) {}

CStudent::CStudent(const string& strName, const CAddress& addr, const string& strEGN,
                   const string& strFN)
    : CPerson(strName, "неопределен", strEGN), m_strFN(strFN), m_address(addr),
      m_pdGrades(nullptr), m_nCount(0), m_nCapacity(0) {}

CStudent::CStudent(const CStudent& stud)
    : CPerson(stud), m_strFN(stud.m_strFN), m_address(stud.m_address),
      m_pdGrades(nullptr), m_nCount(stud.m_nCount), m_nCapacity(stud.m_nCapacity)
{
    if (m_nCapacity > 0)
    {
        m_pdGrades = new double[m_nCapacity];
        for (size_t i = 0; i < m_nCount; i++) m_pdGrades[i] = stud.m_pdGrades[i];
    }
}

CStudent& CStudent::operator=(const CStudent& stud)
{
    if (this == &stud) return *this;

    CPerson::operator=(stud);
    m_strFN   = stud.m_strFN;
    m_address = stud.m_address;

    delete[] m_pdGrades;
    m_pdGrades = nullptr;

    m_nCount    = stud.m_nCount;
    m_nCapacity = stud.m_nCapacity;
    if (m_nCapacity > 0)
    {
        m_pdGrades = new double[m_nCapacity];
        for (size_t i = 0; i < m_nCount; i++) m_pdGrades[i] = stud.m_pdGrades[i];
    }
    return *this;
}

CStudent::~CStudent()
{
    delete[] m_pdGrades;
    m_pdGrades = nullptr;
}

void CStudent::AddGrade(double dGrade)
{
    if (dGrade < 2.0 || dGrade > 6.0) return;
    if (m_nCount == m_nCapacity) grow();
    m_pdGrades[m_nCount] = dGrade;
    m_nCount++;
}

double CStudent::GetGrade(size_t nIndex) const
{
    if (nIndex >= m_nCount) return 0.0;
    return m_pdGrades[nIndex];
}

size_t CStudent::GetGradeCount() const { return m_nCount; }

double CStudent::GetAverage() const
{
    if (m_nCount == 0) return 0.0;
    double dSum = 0.0;
    for (size_t i = 0; i < m_nCount; i++) dSum += m_pdGrades[i];
    return dSum / m_nCount;
}

const string&   CStudent::GetFN() const      { return m_strFN; }
const CAddress& CStudent::GetAddress() const { return m_address; }

void CStudent::SetFN(const string& strFN)       { m_strFN = strFN; }
void CStudent::SetAddress(const CAddress& addr) { m_address = addr; }

// ---------- 4. Приятелска функция за конзолен вход ----------
// Приятелска е, за да пише направо в m_strEGN след собствената си проверка.
void ConsoleInputEGN(CStudent& stud)
{
    cout << "  Въведете ЕГН за " << stud.m_strName << ": ";

    string strEGN;
    if (!(cin >> strEGN))
    {
        cout << endl << "  (няма въведени данни - ЕГН остава " << stud.m_strEGN << ")" << endl;
        return;
    }

    if (stud.isValid(strEGN))
    {
        stud.m_strEGN = strEGN;               // достъп до protected член - позволено
        cout << "  Прието. Нова дата на раждане: " << stud.GetBirthDate() << endl;
    }
    else
    {
        cout << "  Невалидно ЕГН. Остава старото: " << stud.m_strEGN << endl;
    }
}

// ---------- 5. Оператор за извеждане на студент ----------
ostream& operator<<(ostream& toStream, const CStudent& stud)
{
    // Наследената част - чрез оператора на CPerson.
    toStream << static_cast<const CPerson&>(stud);

    toStream << ", ф.н.: " << stud.m_strFN
             << ", адрес: " << stud.m_address;     // чрез оператора на CAddress

    toStream << ", оценки: ";
    if (stud.m_nCount == 0)
        toStream << "(няма)";
    else
    {
        for (size_t i = 0; i < stud.m_nCount; i++)
            toStream << stud.m_pdGrades[i] << (i + 1 < stud.m_nCount ? " " : "");
        toStream << " | ср. успех: " << stud.GetAverage();
    }
    return toStream;
}

// ---------- 6. Оператори за сравнение по възраст ----------
// GetBirthKey() е ГГГГММДД. По-МАЛКО число = по-РАНО роден = по-ВЪЗРАСТЕН.
// Затова "по-малък по възраст" (по-млад) означава по-ГОЛЯМ ключ.
bool operator==(const CStudent& studL, const CStudent& studR)
{
    return studL.GetBirthKey() == studR.GetBirthKey();
}

bool operator!=(const CStudent& studL, const CStudent& studR)
{
    return !(studL == studR);                 // винаги през ==, за да не се разминат
}

bool operator<(const CStudent& studL, const CStudent& studR)
{
    return studL.GetBirthKey() > studR.GetBirthKey();
}

bool operator>=(const CStudent& studL, const CStudent& studR)
{
    return !(studL < studR);                  // винаги през <
}

// ============================================================
//  7-8. Главна функция
// ============================================================
int main()
{
    cout.setf(ios::fixed);
    cout.precision(2);

    CAddress addrVarna("Studentska #1", "9010", "Varna");
    CAddress addrSofia("Tsar Osvoboditel #12", "1000", "Sofia");

    // ===== 1. Оператор << за CAddress =====
    cout << "===== 1. Оператор << за CAddress =====" << endl;
    CAddress addrDefault;
    cout << "  addrVarna:   " << addrVarna << endl;
    cout << "  addrSofia:   " << addrSofia << endl;
    cout << "  addrDefault: " << addrDefault << endl;

    // Верижно извеждане - двата адреса на един ред
    cout << "  верижно: " << addrVarna << "  |  " << addrSofia << endl;

    // ===== 2-3. getYMD и оператор << за CPerson =====
    cout << endl << "===== 2-3. Оператор << за CPerson =====" << endl;
    CPerson pers1("Иван Петров", "мъж", "7503081239");
    CPerson pers2("Мария Иванова", "жена", "0147222342");
    CPerson pers3;
    cout << "  " << pers1 << endl;
    cout << "  " << pers2 << endl;
    cout << "  " << pers3 << endl;

    cout << "--- Разлагане на ЕГН (през GetBirthDate) ---" << endl;
    const char* arrEGN[] = { "7503081239", "8512034566", "9005157895", "0147222342" };
    CPerson persTest;
    for (size_t i = 0; i < sizeof(arrEGN) / sizeof(arrEGN[0]); i++)
    {
        persTest.SetEGN(arrEGN[i]);
        cout << "  " << arrEGN[i] << " -> " << persTest.GetBirthDate()
             << "  (ключ " << persTest.GetBirthKey() << ")" << endl;
    }

    // ===== 5. Оператор << за CStudent =====
    cout << endl << "===== 5. Оператор << за CStudent =====" << endl;
    CStudent studOld("Георги Стоянов", addrVarna, "7503081239", "21621301");
    CStudent studMid("Елена Димитрова", addrSofia, "8512034566", "21621302");
    CStudent studYng("Петър Колев", addrVarna, "0147222342", "21621303");
    CStudent studSame("Анна Тодорова", addrSofia, "9005157895", "21621304");
    CStudent studTwin("Стефан Николов", addrVarna, "9005157909", "21621305");

    studOld.SetGender("мъж");   studOld.AddGrade(5.50); studOld.AddGrade(4.00);
    studMid.SetGender("жена");  studMid.AddGrade(6.00); studMid.AddGrade(5.00);
    studYng.SetGender("мъж");   studYng.AddGrade(3.00);
    studSame.SetGender("жена"); studSame.AddGrade(4.50);
    studTwin.SetGender("мъж");

    cout << "  " << studOld << endl;
    cout << "  " << studMid << endl;
    cout << "  " << studYng << endl;

    // ===== 6. Оператори за сравнение по възраст =====
    cout << endl << "===== 6. Сравнение по възраст =====" << endl;
    cout << "  studOld  роден " << studOld.GetBirthDate()  << endl;
    cout << "  studMid  роден " << studMid.GetBirthDate()  << endl;
    cout << "  studYng  роден " << studYng.GetBirthDate()  << endl;
    cout << "  studSame роден " << studSame.GetBirthDate() << endl;
    cout << "  studTwin роден " << studTwin.GetBirthDate() << endl;

    cout << "--- Резултати ---" << endl;
    cout << "  studSame == studTwin (една дата): " << (studSame == studTwin ? "да" : "не") << endl;
    cout << "  studOld  == studYng:              " << (studOld  == studYng  ? "да" : "не") << endl;
    cout << "  studOld  != studYng:              " << (studOld  != studYng  ? "да" : "не") << endl;
    cout << "  studYng  <  studOld (по-млад):    " << (studYng  <  studOld  ? "да" : "не") << endl;
    cout << "  studOld  <  studYng:              " << (studOld  <  studYng  ? "да" : "не") << endl;
    cout << "  studOld  >= studYng (по-възрастен или равен): "
         << (studOld >= studYng ? "да" : "не") << endl;
    cout << "  studSame >= studTwin (равни):     " << (studSame >= studTwin ? "да" : "не") << endl;

    // Сортиране по възраст - от най-възрастния към най-младия
    cout << "--- Подредба по възраст (най-възрастният първи) ---" << endl;
    CStudent arrStud[5] = { studYng, studSame, studOld, studTwin, studMid };
    const int nCount = 5;
    for (int i = 0; i < nCount - 1; i++)
        for (int j = 0; j < nCount - 1 - i; j++)
            if (arrStud[j] < arrStud[j + 1])       // ляв е по-млад -> разменяме
            {
                CStudent tmp = arrStud[j];
                arrStud[j] = arrStud[j + 1];
                arrStud[j + 1] = tmp;
            }
    for (int i = 0; i < nCount; i++)
        cout << "  " << (i + 1) << ". " << arrStud[i].GetName()
             << " - " << arrStud[i].GetBirthDate() << endl;

    // ===== 8. Извеждане във файлов поток =====
    cout << endl << "===== 8. Извеждане във файл Tema04.txt =====" << endl;
    fstream oFile("Tema04.txt", ios_base::out);
    if (!oFile)
    {
        cout << "  Файлът не може да бъде отворен за запис!" << endl;
    }
    else
    {
        oFile.setf(ios::fixed);
        oFile.precision(2);

        oFile << "Студенти, подредени по възраст" << endl;
        oFile << "==============================" << endl;
        for (int i = 0; i < nCount; i++)
            oFile << (i + 1) << ". " << arrStud[i] << endl;   // СЪЩИЯТ оператор <<

        oFile.close();
        cout << "  Записани " << nCount << " реда. Отворете Tema04.txt." << endl;
    }

    // Прочитаме файла обратно, за да се види резултатът на конзолата
    fstream iFile("Tema04.txt", ios_base::in);
    if (iFile)
    {
        cout << "--- Съдържание на Tema04.txt ---" << endl;
        string strLine;
        while (getline(iFile, strLine))
            cout << "  " << strLine << endl;
        iFile.close();
    }

    // ===== 4. Приятелска функция за конзолен вход =====
    cout << endl << "===== 4. ConsoleInputEGN =====" << endl;
    cout << "  преди: " << studYng << endl;
    ConsoleInputEGN(studYng);
    cout << "  след:  " << studYng << endl;

    // Задържа конзолния прозорец отворен при стартиране с F5 (debug)
    getchar();
    return 0;
}
