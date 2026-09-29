// Тема 10: STL, контейнери, итератори, алгоритми.
// CPerson - личност с име и ЕГН.
// CCity - град с вектор от личности; търсене и премахване на дубликати.

#include <algorithm>  // sort, unique, remove, find, count
#include <cstdio>     // getchar
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>  // runtime_error
#include <string>
#include <vector>
using namespace std;

// ============================================================
//  I. Клас CPerson
// ============================================================
class CPerson
{
// Скрити член променливи
private:
    string m_strName;     // име
    string m_strEGN;      // уникален идентификатор (ЕГН)

public:
    CPerson();                                                  // нужен за вектора
    // 1. Експлицитен конструктор
    CPerson(const string& strEGN, const string& strName);

    // Функции за достъп - само тези, които наистина трябват
    const string& getName() const;
    const string& getEGN() const;

    // 2. Сортиране по ЕГН в нарастващ ред
    bool operator<(const CPerson& pers) const;

    // 3. Търсене по ЕГН
    bool operator==(const CPerson& pers) const;

    // 4. Извеждане в поток
    friend ostream& operator<<(ostream& toStream, const CPerson& pers);

    // 5. Четене от поток
    friend istream& operator>>(istream& fromStream, CPerson& pers);
};

CPerson::CPerson() : m_strName("(няма)"), m_strEGN("") {}

CPerson::CPerson(const string& strEGN, const string& strName)
    : m_strName(strName), m_strEGN(strEGN) {}

const string& CPerson::getName() const { return m_strName; }
const string& CPerson::getEGN() const  { return m_strEGN; }

// ---------- 2. Подредба по ЕГН ----------
bool CPerson::operator<(const CPerson& pers) const
{
    return m_strEGN < pers.m_strEGN;
}

// ---------- 3. Равенство САМО по ЕГН ----------
// ЕГН е уникалният идентификатор: двама души с едно ЕГН са едно лице,
// независимо как са изписани имената им.
bool CPerson::operator==(const CPerson& pers) const
{
    return m_strEGN == pers.m_strEGN;
}

// ---------- 4. Извеждане ----------
ostream& operator<<(ostream& toStream, const CPerson& pers)
{
    toStream << setw(12) << left << pers.m_strEGN << " " << pers.m_strName;
    return toStream;
}

// ---------- 5. Четене ----------
// Форматът на файла е <ЕГН> <име> - в този ред.
istream& operator>>(istream& fromStream, CPerson& pers)
{
    string strEGN, strName;
    if (fromStream >> strEGN >> strName)
    {
        pers.m_strEGN  = strEGN;
        pers.m_strName = strName;
    }
    return fromStream;
}

// ============================================================
//  II. Клас CCity
// ============================================================
class CCity
{
private:
    string          m_strName;       // име на града
    vector<CPerson> m_vecPersons;    // вектор от обекти от I

public:
    CCity();
    // 1. Експлицитен конструктор с параметър име на файл; хвърля изключение
    explicit CCity(const string& strFileName);

    const string& getName() const;
    const vector<CPerson>& getPersons() const;
    size_t size() const;

    // 2. Извеждане на вектора на изходен поток
    ostream& Output(ostream& toStream) const;

    // 3. Връща дублираните по ЕГН лица
    vector<CPerson> getDuplicates() const;

    // 4. Премахва дубликатите от член-променливата (оставя по един)
    void removeDuplicates();

    // 5. Премахва от вектора елементите на подадения вектор
    void removeVector(const vector<CPerson>& toRemove);

    friend ostream& operator<<(ostream& toStream, const CCity& city);
    // 6. Четене от поток
    friend istream& operator>>(istream& fromStream, CCity& city);
};

CCity::CCity() : m_strName("(няма град)"), m_vecPersons() {}

// ---------- 1. Конструктор по име на файл ----------
CCity::CCity(const string& strFileName)
    : m_strName("(няма град)"), m_vecPersons()
{
    ifstream iFile(strFileName.c_str());
    if (!iFile)
        throw runtime_error("Не мога да отворя файла: " + strFileName);

    iFile >> *this;                  // използваме оператора за четене

    if (m_vecPersons.empty())
        throw runtime_error("Файлът " + strFileName + " не съдържа записи");
}

const string& CCity::getName() const                 { return m_strName; }
const vector<CPerson>& CCity::getPersons() const     { return m_vecPersons; }
size_t CCity::size() const                           { return m_vecPersons.size(); }

// ---------- 2. Извеждане ----------
ostream& CCity::Output(ostream& toStream) const
{
    toStream << m_strName << " (" << m_vecPersons.size() << " лица)" << endl;
    for (size_t i = 0; i < m_vecPersons.size(); i++)
        toStream << "    " << m_vecPersons[i] << endl;
    return toStream;
}

ostream& operator<<(ostream& toStream, const CCity& city)
{
    return city.Output(toStream);
}

// ---------- 6. Четене ----------
istream& operator>>(istream& fromStream, CCity& city)
{
    // Първият ред е името на града, после по един запис на ред.
    if (!(fromStream >> city.m_strName))
        return fromStream;

    city.m_vecPersons.clear();
    CPerson pers;
    while (fromStream >> pers)
        city.m_vecPersons.push_back(pers);

    return fromStream;
}

// ---------- 3. Дублираните по ЕГН лица ----------
// Работим върху КОПИЕ: подредбата на член-променливата не се променя.
vector<CPerson> CCity::getDuplicates() const
{
    vector<CPerson> vecSorted = m_vecPersons;
    sort(vecSorted.begin(), vecSorted.end());        // по ЕГН, operator<

    vector<CPerson> vecResult;
    size_t i = 0;
    while (i < vecSorted.size())
    {
        // Намираме докъде се простира групата с едно и също ЕГН.
        size_t j = i + 1;
        while (j < vecSorted.size() && vecSorted[j] == vecSorted[i])
            j++;

        if (j - i > 1)                               // групата е с повече от един
            for (size_t k = i; k < j; k++)
                vecResult.push_back(vecSorted[k]);

        i = j;
    }
    return vecResult;
}

// ---------- 4. Премахване на дубликатите ----------
// Оставя по ЕДИН запис за всяко ЕГН.
void CCity::removeDuplicates()
{
    // unique премахва само СЪСЕДНИ повторения - затова първо sort.
    sort(m_vecPersons.begin(), m_vecPersons.end());

    vector<CPerson>::iterator itNewEnd =
        unique(m_vecPersons.begin(), m_vecPersons.end());

    // unique не смалява вектора - връща новия край. Изтриваме опашката.
    m_vecPersons.erase(itNewEnd, m_vecPersons.end());
}

// ---------- 5. Премахване на подадените елементи ----------
// operator== сравнява по ЕГН, значи се маха ВСЯКО лице с това ЕГН.
void CCity::removeVector(const vector<CPerson>& toRemove)
{
    for (size_t i = 0; i < toRemove.size(); i++)
    {
        // Идиомът remove-erase: remove избутва ненужните в края
        // и връща новия край; erase ги изтрива наистина.
        m_vecPersons.erase(
            remove(m_vecPersons.begin(), m_vecPersons.end(), toRemove[i]),
            m_vecPersons.end());
    }
}

// ============================================================
//  III. Главна функция
// ============================================================
int main()
{
    // ===== 1. Създаване от файл с обработка на изключение =====
    cout << "===== 1. Създаване на обект от файл =====" << endl;

    // а) несъществуващ файл
    try
    {
        CCity cityBad("няма-такъв-файл.txt");
        cout << "  създаден: " << cityBad.getName() << endl;
    }
    catch (const exception& ex)
    {
        cout << "  хванато изключение: " << ex.what() << endl;
    }

    // б) истинският файл
    CCity city;
    try
    {
        city = CCity("Tema10.txt");
        cout << "  прочетен град: " << city.getName()
             << ", лица: " << city.size() << endl;
    }
    catch (const exception& ex)
    {
        cout << "  хванато изключение: " << ex.what() << endl;
        cout << "  Без данни програмата няма какво да прави." << endl;
        getchar();
        return 1;
    }

    // ===== 2. Извеждане =====
    cout << endl << "===== 2. Извеждане (operator<<) =====" << endl;
    cout << city;

    // ===== 3. Дублираните по ЕГН лица =====
    cout << endl << "===== 3. Дублирани по ЕГН =====" << endl;
    vector<CPerson> vecDup = city.getDuplicates();
    cout << "  намерени " << vecDup.size() << " записа с повтарящо се ЕГН:" << endl;
    for (size_t i = 0; i < vecDup.size(); i++)
        cout << "    " << vecDup[i] << endl;

    // Колко пъти се среща всяко ЕГН - с алгоритъма count
    cout << "--- Брой повторения (count) ---" << endl;
    const vector<CPerson>& vecAll = city.getPersons();
    vector<string> vecSeen;
    for (size_t i = 0; i < vecAll.size(); i++)
    {
        if (find(vecSeen.begin(), vecSeen.end(), vecAll[i].getEGN()) != vecSeen.end())
            continue;                        // вече сме броили това ЕГН
        vecSeen.push_back(vecAll[i].getEGN());

        long nCount = count(vecAll.begin(), vecAll.end(), vecAll[i]);
        cout << "    " << setw(12) << left << vecAll[i].getEGN()
             << " -> " << nCount << (nCount > 1 ? "  <- дубликат" : "") << endl;
    }

    // ===== 4. Премахване на дубликатите (остава по един) =====
    cout << endl << "===== 4. removeDuplicates() =====" << endl;
    CCity cityA = city;                      // работим върху копие
    cout << "  преди: " << cityA.size() << " лица" << endl;
    cityA.removeDuplicates();
    cout << "  след:  " << cityA.size() << " лица" << endl;
    cout << cityA;

    // ===== 5. Премахване на всички дублирани лица =====
    cout << endl << "===== 5. removeVector(дубликатите) =====" << endl;
    CCity cityB = city;                      // пак от оригинала
    cout << "  преди: " << cityB.size() << " лица" << endl;
    cityB.removeVector(vecDup);
    cout << "  след:  " << cityB.size() << " лица" << endl;
    cout << cityB;

    // ===== Разликата между 4 и 5 =====
    cout << endl << "===== Разликата между 4 и 5 =====" << endl;
    cout << "  оригинал:          " << city.size()  << " лица" << endl;
    cout << "  removeDuplicates:  " << cityA.size() << " лица (оставя по един от всяко ЕГН)" << endl;
    cout << "  removeVector:      " << cityB.size() << " лица (маха всички дублирани изцяло)" << endl;

    // ===== Търсене по ЕГН =====
    cout << endl << "===== Търсене по ЕГН (find + operator==) =====" << endl;
    const char* arrSearch[] = { "EGN0000003", "EGN0000001", "EGN9999999" };
    for (size_t i = 0; i < sizeof(arrSearch) / sizeof(arrSearch[0]); i++)
    {
        CPerson target(arrSearch[i], "");    // името няма значение за ==
        const vector<CPerson>& v = city.getPersons();
        vector<CPerson>::const_iterator it = find(v.begin(), v.end(), target);

        cout << "    " << setw(12) << left << arrSearch[i] << " -> ";
        if (it != v.end())
            cout << "намерен: " << *it << "  (позиция " << (it - v.begin()) << ")" << endl;
        else
            cout << "НЕ е намерен" << endl;
    }

    // ===== Сортиране на оригинала =====
    cout << endl << "===== Сортиране по ЕГН (operator<) =====" << endl;
    CCity cityC = city;
    vector<CPerson> vecSorted = cityC.getPersons();
    sort(vecSorted.begin(), vecSorted.end());
    for (size_t i = 0; i < vecSorted.size(); i++)
        cout << "    " << vecSorted[i] << endl;

    // Задържа конзолния прозорец отворен при стартиране с F5 (debug)
    getchar();
    return 0;
}
