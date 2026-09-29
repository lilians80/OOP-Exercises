// Тема 3: Динамична памет в класа. Деструктор, дълбоко копиране,
//         оператор за присвояване. "Правилото на тримата".
//
// Надгражда класовете CPerson, CAddress и CStudent от упражнение 2:
// към CStudent се добавя динамичен масив от оценки.

#include <cctype>     // isdigit
#include <cstdio>     // getchar
#include <iostream>
#include <string>
using namespace std;

enum Gender
{
    MALE,       // мъж
    FEMALE,     // жена
    UNKNOWN     // неопределен
};

// ============================================================
//  CPerson - без промяна спрямо упражнение 2
// ============================================================
class CPerson
{
protected:
    string m_strName;
    Gender m_gender;
    string m_strEGN;

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

    ostream& Output(ostream& toStream) const;
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
    if (strGender == "мъж" || strGender == "м" || strGender == "male")   return MALE;
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
    for (int i = 0; i < 9; i++) nSum += (strEGN[i] - '0') * arrWeights[i];

    int nCheck = nSum % 11;
    if (nCheck == 10) nCheck = 0;
    return nCheck == (strEGN[9] - '0');
}

ostream& CPerson::Output(ostream& toStream) const
{
    toStream << m_strName << " (" << genderToString() << "), ЕГН: "
             << (m_strEGN.empty() ? string("(няма)") : m_strEGN);
    return toStream;
}

// ============================================================
//  CAddress - без промяна спрямо упражнение 2
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

    ostream& Output(ostream& toStream) const;
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

ostream& CAddress::Output(ostream& toStream) const
{
    toStream << m_strCity << " " << m_strPostCode << ", " << m_strStreet;
    return toStream;
}

// ============================================================
//  ДЕМОНСТРАЦИЯ: клас БЕЗ правилото на тримата
//  Показва какво прави генерираният от компилатора копиращ конструктор,
//  когато класът държи указател към динамична памет.
// ============================================================
class CBadGrades
{
public:
    double* m_pdGrades;     // указател към динамичен масив
    size_t  m_nCount;

    CBadGrades(size_t nCount)
        : m_pdGrades(new double[nCount]), m_nCount(nCount)
    {
        for (size_t i = 0; i < nCount; i++)
            m_pdGrades[i] = 0.0;
    }

    ~CBadGrades() { delete[] m_pdGrades; }

    // НЯМА копиращ конструктор и НЯМА оператор =.
    // Компилаторът генерира свои - те копират САМИЯ УКАЗАТЕЛ.
};

// ============================================================
//  CStudent - надграден: динамичен масив от оценки
// ============================================================
class CStudent : public CPerson
{
private:
    string   m_strFN;
    CAddress m_address;

    // 1. Динамичната част - това налага правилото на тримата
    double*  m_pdGrades;     // указател към масива с оценки
    size_t   m_nCount;       // колко оценки има в момента
    size_t   m_nCapacity;    // за колко има заделено място

    // Помощна: заделя нов масив с двоен размер и пренася старите оценки
    void grow();

public:
    // Конструктори
    CStudent();
    CStudent(const string& strName, const CAddress& addr, const string& strEGN);
    CStudent(const string& strName, const CAddress& addr, const string& strEGN,
             const string& strFN);

    // 2. ПРАВИЛОТО НА ТРИМАТА
    CStudent(const CStudent& stud);              // а) копиращ конструктор
    CStudent& operator=(const CStudent& stud);   // б) оператор за присвояване
    ~CStudent();                                 // в) деструктор

    // Работа с оценките
    void   AddGrade(double dGrade);
    double GetGrade(size_t nIndex) const;
    size_t GetGradeCount() const;
    double GetAverage() const;

    const string&   GetFN() const;
    const CAddress& GetAddress() const;
    void SetFN(const string& strFN);
    void SetAddress(const CAddress& addr);

    ostream& Output(ostream& toStream) const;
};

// ---------- Помощна функция ----------
void CStudent::grow()
{
    size_t nNew = (m_nCapacity == 0) ? 4 : m_nCapacity * 2;
    double* pdNew = new double[nNew];

    for (size_t i = 0; i < m_nCount; i++)     // пренасяме старите стойности
        pdNew[i] = m_pdGrades[i];

    delete[] m_pdGrades;                      // освобождаваме стария масив
    m_pdGrades  = pdNew;
    m_nCapacity = nNew;
}

// ---------- Конструктори ----------
CStudent::CStudent()
    : CPerson(), m_strFN("(няма)"), m_address(),
      m_pdGrades(nullptr), m_nCount(0), m_nCapacity(0)
{
}

CStudent::CStudent(const string& strName, const CAddress& addr, const string& strEGN)
    : CPerson(strName, "неопределен", strEGN), m_strFN("(няма)"), m_address(addr),
      m_pdGrades(nullptr), m_nCount(0), m_nCapacity(0)
{
}

CStudent::CStudent(const string& strName, const CAddress& addr, const string& strEGN,
                   const string& strFN)
    : CPerson(strName, "неопределен", strEGN), m_strFN(strFN), m_address(addr),
      m_pdGrades(nullptr), m_nCount(0), m_nCapacity(0)
{
}

// ---------- а) Копиращ конструктор с ДЪЛБОКО копиране ----------
CStudent::CStudent(const CStudent& stud)
    : CPerson(stud), m_strFN(stud.m_strFN), m_address(stud.m_address),
      m_pdGrades(nullptr), m_nCount(stud.m_nCount), m_nCapacity(stud.m_nCapacity)
{
    if (m_nCapacity > 0)
    {
        m_pdGrades = new double[m_nCapacity];   // СОБСТВЕНА памет
        for (size_t i = 0; i < m_nCount; i++)   // копираме СТОЙНОСТИТЕ
            m_pdGrades[i] = stud.m_pdGrades[i];
    }
}

// ---------- б) Оператор за присвояване ----------
CStudent& CStudent::operator=(const CStudent& stud)
{
    // 1. Защита от самоприсвояване: stud1 = stud1;
    if (this == &stud)
        return *this;

    // 2. Базовата и вградената част - чрез техните оператори =
    CPerson::operator=(stud);
    m_strFN   = stud.m_strFN;
    m_address = stud.m_address;

    // 3. Старата памет се освобождава ПРЕДИ да се заеме нова
    delete[] m_pdGrades;
    m_pdGrades = nullptr;

    // 4. Дълбоко копиране, както в копиращия конструктор
    m_nCount    = stud.m_nCount;
    m_nCapacity = stud.m_nCapacity;
    if (m_nCapacity > 0)
    {
        m_pdGrades = new double[m_nCapacity];
        for (size_t i = 0; i < m_nCount; i++)
            m_pdGrades[i] = stud.m_pdGrades[i];
    }

    // 5. Връщаме *this, за да работи веригата a = b = c;
    return *this;
}

// ---------- в) Деструктор ----------
CStudent::~CStudent()
{
    delete[] m_pdGrades;      // delete[], защото е заделено с new[]
    m_pdGrades = nullptr;
}

// ---------- Работа с оценките ----------
void CStudent::AddGrade(double dGrade)
{
    if (dGrade < 2.0 || dGrade > 6.0)         // проверка за валидност
    {
        cout << "  (отхвърлена оценка: " << dGrade << ")" << endl;
        return;
    }
    if (m_nCount == m_nCapacity)
        grow();
    m_pdGrades[m_nCount] = dGrade;
    m_nCount++;
}

double CStudent::GetGrade(size_t nIndex) const
{
    if (nIndex >= m_nCount)
        return 0.0;                           // невалиден индекс
    return m_pdGrades[nIndex];
}

size_t CStudent::GetGradeCount() const { return m_nCount; }

double CStudent::GetAverage() const
{
    if (m_nCount == 0)
        return 0.0;                           // защита от деление на нула
    double dSum = 0.0;
    for (size_t i = 0; i < m_nCount; i++)
        dSum += m_pdGrades[i];
    return dSum / m_nCount;
}

// ---------- Четене / презапис ----------
const string&   CStudent::GetFN() const      { return m_strFN; }
const CAddress& CStudent::GetAddress() const { return m_address; }

void CStudent::SetFN(const string& strFN)       { m_strFN = strFN; }
void CStudent::SetAddress(const CAddress& addr) { m_address = addr; }

// ---------- Извеждане ----------
ostream& CStudent::Output(ostream& toStream) const
{
    CPerson::Output(toStream);
    toStream << ", ф.н.: " << m_strFN << ", адрес: ";
    m_address.Output(toStream);

    toStream << ", оценки: ";
    if (m_nCount == 0)
        toStream << "(няма)";
    else
    {
        for (size_t i = 0; i < m_nCount; i++)
            toStream << m_pdGrades[i] << (i + 1 < m_nCount ? " " : "");
        toStream << " | среден успех: " << GetAverage();
    }
    return toStream;
}

// ============================================================
//  Главна функция
// ============================================================
int main()
{
    cout.setf(ios::fixed);
    cout.precision(2);

    CAddress addrVarna("Studentska #1", "9010", "Varna");

    // ===== A. Проблемът: плитко копиране =====
    cout << "===== A. Плитко копиране (клас без правилото на тримата) =====" << endl;
    {
        CBadGrades a(3);
        a.m_pdGrades[0] = 5.50;

        CBadGrades b(a);          // генерираният копиращ конструктор

        cout << "  a[0] преди промяната на b: " << a.m_pdGrades[0] << endl;
        b.m_pdGrades[0] = 2.00;   // пипаме САМО b...
        cout << "  a[0] след  промяната на b: " << a.m_pdGrades[0]
             << "   <-- промени се и a!" << endl;

        cout << "  адрес на a.m_pdGrades: " << (a.m_pdGrades == b.m_pdGrades
             ? "СЪЩИЯТ като на b" : "различен от b") << endl;

        // Обезвреждаме двойното изтриване, за да не се срине програмата:
        b.m_pdGrades = nullptr;
    }

    // ===== Б. Решението: дълбоко копиране =====
    cout << endl << "===== Б. Дълбоко копиране (CStudent) =====" << endl;

    CStudent stud1("Иван Петров", addrVarna, "1111111110", "21621301");
    stud1.AddGrade(5.50);
    stud1.AddGrade(4.00);
    stud1.AddGrade(6.00);
    cout << "  stud1: "; stud1.Output(cout) << endl;

    cout << "--- Копиращ конструктор ---" << endl;
    CStudent stud2(stud1);                    // копиращ конструктор
    stud2.SetName("Елена Димитрова");
    stud2.SetFN("21621302");
    stud2.AddGrade(3.00);                     // добавяме САМО в копието
    cout << "  stud1 (оригинал): "; stud1.Output(cout) << endl;
    cout << "  stud2 (копие):    "; stud2.Output(cout) << endl;

    cout << "--- Оператор за присвояване ---" << endl;
    CStudent stud3("Георги Стоянов", addrVarna, "1111111104", "21621303");
    stud3.AddGrade(2.00);
    cout << "  stud3 преди: "; stud3.Output(cout) << endl;
    stud3 = stud1;                            // оператор =
    cout << "  stud3 след stud3 = stud1: "; stud3.Output(cout) << endl;

    stud3.AddGrade(4.50);                     // промяна само в stud3
    cout << "  stud1 след промяна на stud3: "; stud1.Output(cout) << endl;
    cout << "  stud3 след промяна:          "; stud3.Output(cout) << endl;

    cout << "--- Самоприсвояване ---" << endl;
    // В реален код самоприсвояването идва през псевдоним, а не буквално "a = a".
    CStudent& refStud1 = stud1;
    stud1 = refStud1;                         // не трябва да разруши обекта
    cout << "  stud1 след stud1 = refStud1: "; stud1.Output(cout) << endl;

    cout << "--- Верига от присвоявания ---" << endl;
    CStudent studA, studB;
    studA = studB = stud1;                    // работи, защото operator= връща *this
    cout << "  studA: "; studA.Output(cout) << endl;

    // ===== В. Растежът на масива =====
    cout << endl << "===== В. Растеж на динамичния масив =====" << endl;
    CStudent stud4("Мария Иванова", addrVarna, "1111111110", "21621304");
    const double arrGrades[] = { 6.00, 5.50, 5.00, 4.50, 4.00, 3.50 };
    const size_t nGrades = sizeof(arrGrades) / sizeof(arrGrades[0]);
    for (size_t i = 0; i < nGrades; i++)
    {
        stud4.AddGrade(arrGrades[i]);
        cout << "  след " << (i + 1) << " оценки: брой = " << stud4.GetGradeCount()
             << ", среден успех = " << stud4.GetAverage() << endl;
    }

    // ===== Г. Проверки за валидност =====
    cout << endl << "===== Г. Проверки =====" << endl;
    cout << "  AddGrade(7.00):" << endl;
    stud4.AddGrade(7.00);                     // извън 2.00-6.00
    cout << "  AddGrade(1.00):" << endl;
    stud4.AddGrade(1.00);                     // извън 2.00-6.00
    cout << "  брой оценки остава: " << stud4.GetGradeCount() << endl;
    cout << "  GetGrade(0)   = " << stud4.GetGrade(0) << endl;
    cout << "  GetGrade(100) = " << stud4.GetGrade(100) << "  (невалиден индекс)" << endl;

    CStudent studEmpty;
    cout << "  среден успех на празен студент = " << studEmpty.GetAverage() << endl;

    // ===== Д. Обекти от различни видове =====
    cout << endl << "===== Д. Различни видове променливи =====" << endl;

    CStudent arrStud[2] = { stud1, stud4 };   // масив, инициализиран с копия
    for (int i = 0; i < 2; i++)
    {
        cout << "  [" << i << "] ";
        arrStud[i].Output(cout) << endl;
    }

    CStudent* pStud = new CStudent(stud1);    // динамична памет
    cout << "  през указател: ";
    pStud->Output(cout) << endl;
    delete pStud;                             // тук се вика деструкторът
    pStud = nullptr;

    const CStudent cStud(stud1);              // константен обект
    cout << "  константен: ";
    cStud.Output(cout) << endl;

    // Задържа конзолния прозорец отворен при стартиране с F5 (debug)
    getchar();
    return 0;
}
