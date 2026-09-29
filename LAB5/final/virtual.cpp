// Тема 5: Виртуални функции, множествено наследяване.
// Надгражда класовете от упражнение 4: добавят се оператори за четене от
// поток, абстрактен клас CGSM и CStudent става наследник на ДВА класа.

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

    friend ostream& operator<<(ostream& toStream, const CAddress& addr);
    // 1. Оператор за ЧЕТЕНЕ: формат <Град> <Пощ.код> <Адрес>
    friend istream& operator>>(istream& fromStream, CAddress& addr);
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

ostream& operator<<(ostream& toStream, const CAddress& addr)
{
    toStream << addr.m_strCity << " " << addr.m_strPostCode << ", " << addr.m_strStreet;
    return toStream;
}

// ---------- 1. Четене на адрес ----------
istream& operator>>(istream& fromStream, CAddress& addr)
{
    // Редът е точно като във файла: град, пощенски код, улица.
    fromStream >> addr.m_strCity >> addr.m_strPostCode >> addr.m_strStreet;
    return fromStream;
}

// ============================================================
//  CPerson - вече с ВИРТУАЛНИ функции
// ============================================================
class CPerson
{
protected:
    string m_strName;
    Gender m_gender;
    string m_strEGN;

private:
    void getYMD(const string& strEGN, int& iYear, int& iMonth, int& iDay) const;

public:
    CPerson();
    CPerson(const string& strName, const string& strGender, const string& strEGN);
    CPerson(const CPerson& pers);

    // Виртуален деструктор - задължителен при наследяване с указатели
    virtual ~CPerson();

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

    int GetBirthKey() const;
    string GetBirthDate() const;

    // ВИРТУАЛНИ функции за вход/изход - наследникът ги подменя
    virtual ostream& Output(ostream& toStream) const;
    virtual istream& Input(istream& fromStream);

    friend ostream& operator<<(ostream& toStream, const CPerson& pers);
    // 2. Оператор за четене на личност
    friend istream& operator>>(istream& fromStream, CPerson& pers);
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

CPerson::~CPerson() {}

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
    else return;

    iYear = nYear; iMonth = nMonth; iDay = nDay;
}

bool CPerson::isValid(const string& strEGN)
{
    if (strEGN.length() != 10) return false;
    for (size_t i = 0; i < strEGN.length(); i++)
        if (!isdigit(static_cast<unsigned char>(strEGN[i]))) return false;

    int nYear, nMonth, nDay;
    getYMD(strEGN, nYear, nMonth, nDay);
    if (nYear == 0) return false;

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

int CPerson::GetBirthKey() const
{
    int nYear, nMonth, nDay;
    getYMD(m_strEGN, nYear, nMonth, nDay);
    if (nYear == 0) return 0;
    return nYear * 10000 + nMonth * 100 + nDay;
}

string CPerson::GetBirthDate() const
{
    int nYear, nMonth, nDay;
    getYMD(m_strEGN, nYear, nMonth, nDay);
    if (nYear == 0) return "(няма)";

    ostringstream oss;
    oss << setfill('0') << setw(2) << nDay  << "."
                        << setw(2) << nMonth << "."
                        << setw(4) << nYear;
    return oss.str();
}

// ---------- Виртуалните функции за вход/изход ----------
ostream& CPerson::Output(ostream& toStream) const
{
    toStream << m_strName << " (" << genderToString() << "), ЕГН: "
             << (m_strEGN.empty() ? string("(няма)") : m_strEGN)
             << ", роден: " << GetBirthDate();
    return toStream;
}

istream& CPerson::Input(istream& fromStream)
{
    // Формат: <име> <ЕГН>
    string strName, strEGN;
    if (fromStream >> strName >> strEGN)
    {
        m_strName = strName;
        SetEGN(strEGN);               // невалидно ЕГН просто не се записва
    }
    return fromStream;
}

// ---------- Операторите просто делегират на виртуалните функции ----------
ostream& operator<<(ostream& toStream, const CPerson& pers)
{
    return pers.Output(toStream);     // ТУК се случва виртуалното извикване
}

istream& operator>>(istream& fromStream, CPerson& pers)
{
    return pers.Input(fromStream);
}

// ============================================================
//  4. Абстрактен клас CGSM
// ============================================================
class CGSM
{
// Частни член променливи - наследникът стига до тях през функциите по-долу.
private:
    string m_strModel;      // модел, напр. "NOKIA"
    int    m_nTariff;       // тарифен план, напр. 20

public:
    CGSM();                                              // подразбиращ се
    CGSM(const string& strModel, int nTariff);           // експлицитен
    virtual ~CGSM();                                     // виртуален деструктор

    const string& GetModel() const;
    int  GetTariff() const;
    void SetModel(const string& strModel);
    void SetTariff(int nTariff);

    // НАПЪЛНО ВИРТУАЛНИ функции: = 0 значи "няма тяло тук".
    // Всеки наследник е длъжен да ги напише.
    virtual ostream& OutputGSM(ostream& toStream) const = 0;
    virtual string   getGSMNumber() const = 0;
    virtual istream& InputGSM(istream& fromStream) = 0;
};

CGSM::CGSM() : m_strModel("(няма)"), m_nTariff(0) {}

CGSM::CGSM(const string& strModel, int nTariff)
    : m_strModel(strModel), m_nTariff(nTariff) {}

CGSM::~CGSM() {}

const string& CGSM::GetModel() const  { return m_strModel; }
int  CGSM::GetTariff() const          { return m_nTariff; }
void CGSM::SetModel(const string& strModel) { m_strModel = strModel; }
void CGSM::SetTariff(int nTariff)           { m_nTariff = nTariff; }

// ============================================================
//  5. CStudent - наследник на ДВА класа
// ============================================================
class CStudent : public CPerson, public CGSM
{
private:
    string   m_strFN;
    CAddress m_address;
    string   m_strPhone;    // 5. номер на телефон

    double*  m_pdGrades;
    size_t   m_nCount;
    size_t   m_nCapacity;

    void grow();

public:
    CStudent();
    CStudent(const string& strName, const CAddress& addr, const string& strEGN);
    CStudent(const string& strName, const CAddress& addr, const string& strEGN,
             const string& strFN);
    CStudent(const string& strName, const CAddress& addr, const string& strEGN,
             const string& strFN, const string& strModel, int nTariff,
             const string& strPhone);
    CStudent(const CStudent& stud);
    CStudent& operator=(const CStudent& stud);
    virtual ~CStudent();

    void   AddGrade(double dGrade);
    double GetGrade(size_t nIndex) const;
    size_t GetGradeCount() const;
    double GetAverage() const;

    const string&   GetFN() const;
    const CAddress& GetAddress() const;
    const string&   GetPhone() const;
    void SetFN(const string& strFN);
    void SetAddress(const CAddress& addr);
    void SetPhone(const string& strPhone);

    // Подменяме виртуалните функции на CPerson
    virtual ostream& Output(ostream& toStream) const;
    virtual istream& Input(istream& fromStream);

    // 4. Задължителните три функции на CGSM
    virtual ostream& OutputGSM(ostream& toStream) const;
    virtual string   getGSMNumber() const;
    virtual istream& InputGSM(istream& fromStream);

    // 6. ЧЛЕН-оператор (не приятелски) за сравнение по тарифен план
    bool operator>(const CStudent& studR) const;

    friend void ConsoleInputEGN(CStudent& stud);
    // 3. Оператор за четене на студент
    friend istream& operator>>(istream& fromStream, CStudent& stud);

    // Оператори за сравнение по възраст от упражнение 4
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
    m_pdGrades = pdNew;
    m_nCapacity = nNew;
}

CStudent::CStudent()
    : CPerson(), CGSM(), m_strFN("(няма)"), m_address(), m_strPhone("(няма)"),
      m_pdGrades(nullptr), m_nCount(0), m_nCapacity(0) {}

CStudent::CStudent(const string& strName, const CAddress& addr, const string& strEGN)
    : CPerson(strName, "неопределен", strEGN), CGSM(),
      m_strFN("(няма)"), m_address(addr), m_strPhone("(няма)"),
      m_pdGrades(nullptr), m_nCount(0), m_nCapacity(0) {}

CStudent::CStudent(const string& strName, const CAddress& addr, const string& strEGN,
                   const string& strFN)
    : CPerson(strName, "неопределен", strEGN), CGSM(),
      m_strFN(strFN), m_address(addr), m_strPhone("(няма)"),
      m_pdGrades(nullptr), m_nCount(0), m_nCapacity(0) {}

CStudent::CStudent(const string& strName, const CAddress& addr, const string& strEGN,
                   const string& strFN, const string& strModel, int nTariff,
                   const string& strPhone)
    : CPerson(strName, "неопределен", strEGN), CGSM(strModel, nTariff),
      m_strFN(strFN), m_address(addr), m_strPhone(strPhone),
      m_pdGrades(nullptr), m_nCount(0), m_nCapacity(0) {}

// Копиращият конструктор вика копиращите конструктори на ДВАТА базови класа.
CStudent::CStudent(const CStudent& stud)
    : CPerson(stud), CGSM(stud),
      m_strFN(stud.m_strFN), m_address(stud.m_address), m_strPhone(stud.m_strPhone),
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

    CPerson::operator=(stud);         // базов клас 1
    CGSM::operator=(stud);            // базов клас 2
    m_strFN    = stud.m_strFN;
    m_address  = stud.m_address;
    m_strPhone = stud.m_strPhone;

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
const string&   CStudent::GetPhone() const   { return m_strPhone; }

void CStudent::SetFN(const string& strFN)         { m_strFN = strFN; }
void CStudent::SetAddress(const CAddress& addr)   { m_address = addr; }
void CStudent::SetPhone(const string& strPhone)   { m_strPhone = strPhone; }

// ---------- Подменените виртуални функции на CPerson ----------
ostream& CStudent::Output(ostream& toStream) const
{
    CPerson::Output(toStream);        // наследената част
    toStream << ", ф.н.: " << m_strFN << ", адрес: " << m_address;

    toStream << ", GSM: ";
    OutputGSM(toStream);              // частта от CGSM

    toStream << ", оценки: ";
    if (m_nCount == 0)
        toStream << "(няма)";
    else
    {
        for (size_t i = 0; i < m_nCount; i++)
            toStream << m_pdGrades[i] << (i + 1 < m_nCount ? " " : "");
        toStream << " | ср. успех: " << GetAverage();
    }
    return toStream;
}

istream& CStudent::Input(istream& fromStream)
{
    // Формат (3 реда):
    //   <име> <ЕГН>
    //   <Град> <Пощ.код> <Адрес>
    //   <Фак№> <МаркаGSM> <ТарифенПлан> <Тел№>
    CPerson::Input(fromStream);       // ред 1
    fromStream >> m_address;          // ред 2
    fromStream >> m_strFN;            // ред 3, първа част
    InputGSM(fromStream);             // ред 3, останалото
    return fromStream;
}

// ---------- 4. Трите задължителни функции на CGSM ----------
ostream& CStudent::OutputGSM(ostream& toStream) const
{
    toStream << GetModel() << " (план " << GetTariff() << ", тел. "
             << getGSMNumber() << ")";
    return toStream;
}

string CStudent::getGSMNumber() const { return m_strPhone; }

istream& CStudent::InputGSM(istream& fromStream)
{
    string strModel, strPhone;
    int nTariff = 0;
    if (fromStream >> strModel >> nTariff >> strPhone)
    {
        SetModel(strModel);
        SetTariff(nTariff);
        m_strPhone = strPhone;
    }
    return fromStream;
}

// ---------- 6. Член-оператор за сравнение по тарифен план ----------
// Само ЕДИН параметър: лявата страна е самият обект (this).
bool CStudent::operator>(const CStudent& studR) const
{
    return GetTariff() > studR.GetTariff();
}

// ---------- 3. Оператор за четене на студент ----------
istream& operator>>(istream& fromStream, CStudent& stud)
{
    return stud.Input(fromStream);
}

// ---------- Оператори от упражнение 4 ----------
bool operator==(const CStudent& studL, const CStudent& studR)
{ return studL.GetBirthKey() == studR.GetBirthKey(); }

bool operator!=(const CStudent& studL, const CStudent& studR)
{ return !(studL == studR); }

bool operator<(const CStudent& studL, const CStudent& studR)
{ return studL.GetBirthKey() > studR.GetBirthKey(); }

bool operator>=(const CStudent& studL, const CStudent& studR)
{ return !(studL < studR); }

void ConsoleInputEGN(CStudent& stud)
{
    cout << "  Въведете ЕГН за " << stud.m_strName << ": ";
    string strEGN;
    if (!(cin >> strEGN))
    {
        cout << endl << "  (няма въведени данни)" << endl;
        return;
    }
    if (stud.isValid(strEGN))
    {
        stud.m_strEGN = strEGN;
        cout << "  Прието. Нова дата на раждане: " << stud.GetBirthDate() << endl;
    }
    else
        cout << "  Невалидно ЕГН. Остава старото: " << stud.m_strEGN << endl;
}

// ============================================================
//  7-8. Главна функция
// ============================================================
int main()
{
    cout.setf(ios::fixed);
    cout.precision(2);

    // ===== 1. Четене на адрес от текстов поток =====
    cout << "===== 1. operator>> за CAddress =====" << endl;
    {
        istringstream iss("Varna 9010 Studentska#1");
        CAddress addr;
        iss >> addr;
        cout << "  прочетено: " << addr << endl;
    }

    // ===== 2. Четене на личност =====
    cout << endl << "===== 2. operator>> за CPerson =====" << endl;
    {
        istringstream iss("Ivanov 7503081239");
        CPerson pers;
        iss >> pers;
        cout << "  прочетено: " << pers << endl;
    }

    // ===== Виртуални функции: проблемът от упражнение 2 е решен =====
    cout << endl << "===== Виртуални функции =====" << endl;
    CAddress addrVarna("Studentska#1", "9010", "Varna");
    CStudent studDemo("Георги Стоянов", addrVarna, "7503081239", "21621301",
                      "NOKIA", 20, "0888000000");
    studDemo.SetGender("мъж");
    studDemo.AddGrade(5.50);

    CPerson* pPerson = &studDemo;             // указател към БАЗОВ клас
    cout << "  през CStudent&: " << studDemo << endl;
    cout << "  през CPerson*:  " << *pPerson << endl;
    cout << "  (двата реда са еднакви - това прави virtual)" << endl;

    // ===== Абстрактният клас през указател CGSM* =====
    cout << endl << "===== 4. Абстрактният клас CGSM =====" << endl;
    CGSM* pGSM = &studDemo;                   // студентът Е и CGSM
    cout << "  през CGSM* -> OutputGSM: ";
    pGSM->OutputGSM(cout) << endl;
    cout << "  през CGSM* -> getGSMNumber: " << pGSM->getGSMNumber() << endl;
    // CGSM gsm;          // грешка: абстрактен клас не може да се създаде

    // ===== 8. Четене на масив от студенти от файл =====
    cout << endl << "===== 8. Четене от файл Tema05.txt =====" << endl;
    const int MAX_STUD = 10;
    CStudent arrStud[MAX_STUD];
    int nRead = 0;

    fstream iFile("Tema05.txt", ios_base::in);
    if (!iFile)
    {
        cout << "  Файлът Tema05.txt не е намерен!" << endl;
    }
    else
    {
        while (nRead < MAX_STUD && (iFile >> arrStud[nRead]))
            nRead++;
        iFile.close();
        cout << "  Прочетени " << nRead << " студента:" << endl;
        for (int i = 0; i < nRead; i++)
            cout << "  " << (i + 1) << ". " << arrStud[i] << endl;
    }

    if (nRead >= 2)
    {
        // ===== 6. Член-оператор > по тарифен план =====
        cout << endl << "===== 6. Сравнение по тарифен план (член-оператор) =====" << endl;
        for (int i = 0; i < nRead; i++)
            cout << "  " << arrStud[i].GetName() << ": план "
                 << arrStud[i].GetTariff() << endl;

        cout << "--- Резултати ---" << endl;
        cout << "  " << arrStud[0].GetName() << " > " << arrStud[1].GetName()
             << " (по план): " << (arrStud[0] > arrStud[1] ? "да" : "не") << endl;
        cout << "  " << arrStud[1].GetName() << " > " << arrStud[0].GetName()
             << " (по план): " << (arrStud[1] > arrStud[0] ? "да" : "не") << endl;

        // Двата вида сравнение върху едни и същи обекти
        cout << "--- Две различни сравнения, едни и същи обекти ---" << endl;
        cout << "  по ВЪЗРАСТ  (глобален <):  " << arrStud[0].GetName() << " < "
             << arrStud[2].GetName() << " = "
             << (arrStud[0] < arrStud[2] ? "да" : "не") << endl;
        cout << "  по ТАРИФА   (член >):      " << arrStud[0].GetName() << " > "
             << arrStud[2].GetName() << " = "
             << (arrStud[0] > arrStud[2] ? "да" : "не") << endl;

        // Най-скъпият тарифен план
        int nMax = 0;
        for (int i = 1; i < nRead; i++)
            if (arrStud[i] > arrStud[nMax])
                nMax = i;
        cout << "  Най-висок тарифен план: " << arrStud[nMax].GetName()
             << " (" << arrStud[nMax].GetTariff() << ")" << endl;
    }

    // ===== Масив от указатели към базовия клас =====
    cout << endl << "===== Полиморфизъм през масив от указатели =====" << endl;
    CPerson  persPlain("Иван Петров", "мъж", "8512034566");
    CPerson* arrPtr[3] = { &persPlain, &studDemo, &arrStud[0] };
    for (int i = 0; i < 3; i++)
        cout << "  [" << i << "] " << *arrPtr[i] << endl;

    // ===== Виртуален деструктор =====
    cout << endl << "===== Виртуален деструктор =====" << endl;
    CPerson* pDyn = new CStudent("Петър Колев", addrVarna, "0147222342", "21621399",
                                 "APPLE", 15, "0888000009");
    cout << "  " << *pDyn << endl;
    delete pDyn;      // вика ~CStudent, защото деструкторът е virtual
    cout << "  обектът е унищожен коректно" << endl;

    // Задържа конзолния прозорец отворен при стартиране с F5 (debug)
    getchar();
    return 0;
}
