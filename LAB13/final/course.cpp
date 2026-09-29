// Тема 13: Асоциативни контейнери, алгоритми.
// Student - фак. номер и група.
// Exam    - упражнение с вектор от студенти.
// Cource  - дисциплина с вектор от упражнения. (Името е както в условието.)

#include <algorithm>  // sort, find
#include <cstdio>     // getchar
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <sstream>    // istringstream
#include <stdexcept>  // runtime_error
#include <string>
#include <vector>
using namespace std;

// ============================================================
//  I. Клас Student
// ============================================================
class Student
{
// Скрити членове
private:
    string m_strFN;      // факултетен номер
    int    m_nGroup;     // група

public:
    Student();                                          // подразбиращ се
    Student(const string& strFN, int nGroup);           // експлицитен

    // Четене / промяна на членовете
    const string& getFN() const;
    int  getGroup() const;
    void setFN(const string& strFN);
    void setGroup(int nGroup);

    // Сравнение за по-малък по факултетен номер
    bool operator<(const Student& stud) const;

    // Потоков вход/изход
    friend ostream& operator<<(ostream& toStream, const Student& stud);
    friend istream& operator>>(istream& fromStream, Student& stud);
};

Student::Student() : m_strFN("(няма)"), m_nGroup(0) {}

Student::Student(const string& strFN, int nGroup)
    : m_strFN(strFN), m_nGroup(nGroup) {}

const string& Student::getFN() const { return m_strFN; }
int  Student::getGroup() const       { return m_nGroup; }
void Student::setFN(const string& strFN) { m_strFN = strFN; }
void Student::setGroup(int nGroup)       { m_nGroup = nGroup; }

// ---------- Подредба по факултетен номер ----------
// От този оператор зависи и подредбата, и УНИКАЛНОСТТА в set:
// set смята за еднакви два елемента, при които нито a<b, нито b<a.
bool Student::operator<(const Student& stud) const
{
    return m_strFN < stud.m_strFN;
}

ostream& operator<<(ostream& toStream, const Student& stud)
{
    toStream << stud.m_strFN << "/" << stud.m_nGroup;
    return toStream;
}

istream& operator>>(istream& fromStream, Student& stud)
{
    string strFN;
    int nGroup = 0;
    if (fromStream >> strFN >> nGroup)
    {
        stud.m_strFN  = strFN;
        stud.m_nGroup = nGroup;
    }
    return fromStream;
}

// ============================================================
//  II. Клас Exam
// ============================================================
class Exam
{
private:
    string          m_strName;      // име на упражнението
    vector<Student> m_vecStudents;  // студентите, с които се провежда

public:
    Exam();                                     // подразбиращ се
    explicit Exam(const string& strName);       // само по име на упражнението

    const string& getName() const;
    void setName(const string& strName);
    const vector<Student>& getStudents() const;

    void addStudent(const Student& stud);
    void sortStudents();                        // по факултетен номер

    bool operator<(const Exam& exam) const;     // по име на упражнение
    bool operator==(const Exam& exam) const;    // по име на упражнение

    friend ostream& operator<<(ostream& toStream, const Exam& exam);
    friend istream& operator>>(istream& fromStream, Exam& exam);
};

Exam::Exam() : m_strName("(няма)"), m_vecStudents() {}

Exam::Exam(const string& strName) : m_strName(strName), m_vecStudents() {}

const string& Exam::getName() const                  { return m_strName; }
void Exam::setName(const string& strName)            { m_strName = strName; }
const vector<Student>& Exam::getStudents() const     { return m_vecStudents; }

void Exam::addStudent(const Student& stud) { m_vecStudents.push_back(stud); }

// ---------- Сортиране на студентите по фак. номер ----------
void Exam::sortStudents()
{
    sort(m_vecStudents.begin(), m_vecStudents.end());   // operator< на Student
}

bool Exam::operator<(const Exam& exam) const  { return m_strName < exam.m_strName; }
bool Exam::operator==(const Exam& exam) const { return m_strName == exam.m_strName; }

ostream& operator<<(ostream& toStream, const Exam& exam)
{
    toStream << exam.m_strName << " (" << exam.m_vecStudents.size() << "):";
    for (size_t i = 0; i < exam.m_vecStudents.size(); i++)
        toStream << " " << exam.m_vecStudents[i];
    return toStream;
}

// ---------- Четене на ЕДИН ред от файла ----------
// Форматът е <име> <фн> <група> <фн> <група>... до края на РЕДА,
// затова тук се чете ред и се разлага с istringstream.
istream& operator>>(istream& fromStream, Exam& exam)
{
    string strLine;
    // Прескачаме евентуални празни редове.
    while (getline(fromStream, strLine))
        if (!strLine.empty() && strLine.find_first_not_of(" \t\r") != string::npos)
            break;

    if (strLine.empty() || strLine.find_first_not_of(" \t\r") == string::npos)
        return fromStream;

    istringstream iss(strLine);
    if (!(iss >> exam.m_strName))
        return fromStream;

    exam.m_vecStudents.clear();
    Student stud;
    while (iss >> stud)                    // до края на РЕДА, не на файла
        exam.m_vecStudents.push_back(stud);

    return fromStream;
}

// ============================================================
//  III. Клас Cource (дисциплина)
// ============================================================
class Cource
{
private:
    string       m_strName;      // име на дисциплината
    vector<Exam> m_vecExams;     // упражнения, които включва

public:
    Cource();                                        // подразбиращ се
    explicit Cource(const string& strFileName);      // само по име на файл

    const string& getName() const;
    void setName(const string& strName);
    const vector<Exam>& getExams() const;

    // Множество от студентите, подредени по фак. номер
    set<Student> getStudentSet() const;

    // Списък от неповтарящи се групи за ЗАДАДЕНО упражнение
    list<int> getGroups(const string& strExamName) const;

    // Карта: име на упражнение -> списък от неповтарящи се групи
    map<string, list<int> > getGroupsMap() const;

    friend ostream& operator<<(ostream& toStream, const Cource& cource);
    friend istream& operator>>(istream& fromStream, Cource& cource);
};

Cource::Cource() : m_strName("(няма)"), m_vecExams() {}

Cource::Cource(const string& strFileName) : m_strName("(няма)"), m_vecExams()
{
    ifstream iFile(strFileName.c_str());
    if (!iFile)
        throw runtime_error("Не мога да отворя файла: " + strFileName);

    iFile >> *this;

    if (m_vecExams.empty())
        throw runtime_error("Файлът " + strFileName + " не съдържа упражнения");
}

const string& Cource::getName() const           { return m_strName; }
void Cource::setName(const string& strName)     { m_strName = strName; }
const vector<Exam>& Cource::getExams() const    { return m_vecExams; }

// ---------- Четене на цялата дисциплина ----------
istream& operator>>(istream& fromStream, Cource& cource)
{
    // Първият ред е името на дисциплината.
    if (!getline(fromStream, cource.m_strName))
        return fromStream;

    // Махаме евентуален '\r' от файл, записан под Windows.
    while (!cource.m_strName.empty() &&
           (cource.m_strName.back() == '\r' || cource.m_strName.back() == ' '))
        cource.m_strName.erase(cource.m_strName.size() - 1);

    cource.m_vecExams.clear();
    Exam exam;
    while (fromStream >> exam)
        cource.m_vecExams.push_back(exam);

    return fromStream;
}

ostream& operator<<(ostream& toStream, const Cource& cource)
{
    toStream << "Дисциплина " << cource.m_strName
             << " (" << cource.m_vecExams.size() << " упражнения)" << endl;
    for (size_t i = 0; i < cource.m_vecExams.size(); i++)
        toStream << "    " << cource.m_vecExams[i] << endl;
    return toStream;
}

// ---------- Множество от студентите ----------
// set сам подрежда по operator< и сам премахва повторенията.
set<Student> Cource::getStudentSet() const
{
    set<Student> setStudents;
    for (size_t i = 0; i < m_vecExams.size(); i++)
    {
        const vector<Student>& vec = m_vecExams[i].getStudents();
        for (size_t j = 0; j < vec.size(); j++)
            setStudents.insert(vec[j]);      // повторните просто не влизат
    }
    return setStudents;
}

// ---------- Неповтарящи се групи за дадено упражнение ----------
list<int> Cource::getGroups(const string& strExamName) const
{
    list<int> lstGroups;

    for (size_t i = 0; i < m_vecExams.size(); i++)
    {
        if (m_vecExams[i].getName() != strExamName)
            continue;

        const vector<Student>& vec = m_vecExams[i].getStudents();
        for (size_t j = 0; j < vec.size(); j++)
        {
            int nGroup = vec[j].getGroup();
            // Добавяме само ако групата още я няма в списъка.
            if (find(lstGroups.begin(), lstGroups.end(), nGroup) == lstGroups.end())
                lstGroups.push_back(nGroup);
        }
    }

    lstGroups.sort();          // собствената функция на list
    return lstGroups;
}

// ---------- Карта: упражнение -> групи ----------
map<string, list<int> > Cource::getGroupsMap() const
{
    map<string, list<int> > mapResult;
    for (size_t i = 0; i < m_vecExams.size(); i++)
    {
        const string& strName = m_vecExams[i].getName();
        mapResult[strName] = getGroups(strName);
    }
    return mapResult;
}

// ============================================================
//  IV. Шаблонен клас функция SetPrinter
// ============================================================
template <class T>
class SetPrinter
{
public:
    void operator()(const set<T>& setData) const
    {
        cout << "  множество (" << setData.size() << "):";
        for (typename set<T>::const_iterator it = setData.begin();
             it != setData.end(); ++it)
            cout << " " << *it;
        cout << endl;
    }
};

// ============================================================
//  V. Шаблонен клас функция ListPrinter
// ============================================================
template <class T>
class ListPrinter
{
public:
    void operator()(const string& strName, const list<T>& lstData) const
    {
        cout << "  " << strName << " (" << lstData.size() << "):";
        for (typename list<T>::const_iterator it = lstData.begin();
             it != lstData.end(); ++it)
            cout << " " << *it;
        cout << endl;
    }
};

// ============================================================
//  VI. Главна функция
// ============================================================
int main()
{
    // ===== Създаване на дисциплина по файл =====
    cout << "===== Създаване на Cource по файл =====" << endl;

    try
    {
        Cource bad("няма-такъв-файл.txt");
        cout << bad;
    }
    catch (const exception& ex)
    {
        cout << "  хванато изключение: " << ex.what() << endl;
    }

    Cource cource;
    try
    {
        cource = Cource("Tema13.txt");
        cout << "  прочетена дисциплина: " << cource.getName()
             << ", упражнения: " << cource.getExams().size() << endl;
    }
    catch (const exception& ex)
    {
        cout << "  хванато изключение: " << ex.what() << endl;
        getchar();
        return 1;
    }

    // ===== Извеждане =====
    cout << endl << "===== Извеждане (operator<<) =====" << endl;
    cout << cource;

    // ===== Сортиране на студентите в упражнение =====
    cout << endl << "===== Exam::sortStudents() =====" << endl;
    Exam exam1 = cource.getExams()[0];
    cout << "  преди: " << exam1 << endl;
    exam1.sortStudents();
    cout << "  след:  " << exam1 << endl;

    // ===== Множество от студентите =====
    cout << endl << "===== set<Student> - подредени и без повторения =====" << endl;

    size_t nTotal = 0;
    for (size_t i = 0; i < cource.getExams().size(); i++)
        nTotal += cource.getExams()[i].getStudents().size();

    set<Student> setStud = cource.getStudentSet();
    cout << "  общо записа във всички упражнения: " << nTotal << endl;
    cout << "  различни студенти в множеството:   " << setStud.size() << endl;

    SetPrinter<Student> printSet;              // IV. шаблонният клас функция
    printSet(setStud);

    // Търсене в множеството
    cout << "--- Търсене в множеството ---" << endl;
    const char* arrFind[] = { "106601", "096042", "999999" };
    for (size_t i = 0; i < sizeof(arrFind) / sizeof(arrFind[0]); i++)
    {
        Student target(arrFind[i], 0);         // групата няма значение за <
        // find на set търси по operator<, не с обхождане - затова е бързо.
        set<Student>::const_iterator it = setStud.find(target);
        cout << "    " << arrFind[i] << " -> ";
        if (it != setStud.end())
            cout << "намерен: " << *it << endl;
        else
            cout << "НЕ е намерен" << endl;
    }

    // ===== Неповтарящи се групи по упражнение =====
    cout << endl << "===== list<int> - групи за дадено упражнение =====" << endl;
    ListPrinter<int> printList;                // V. шаблонният клас функция

    const vector<Exam>& vecExams = cource.getExams();
    for (size_t i = 0; i < vecExams.size(); i++)
    {
        const string& strName = vecExams[i].getName();
        list<int> lstGroups = cource.getGroups(strName);
        printList("групи за " + strName, lstGroups);
    }

    printList("групи за НЕсъществуващо упражнение", cource.getGroups("НЯМА"));

    // ===== Картата упражнение -> групи =====
    cout << endl << "===== map<string, list<int>> =====" << endl;
    map<string, list<int> > mapGroups = cource.getGroupsMap();

    cout << "  размер на картата: " << mapGroups.size() << endl;
    for (map<string, list<int> >::const_iterator it = mapGroups.begin();
         it != mapGroups.end(); ++it)
    {
        cout << "    " << setw(8) << left << it->first << "->";
        for (list<int>::const_iterator itg = it->second.begin();
             itg != it->second.end(); ++itg)
            cout << " " << *itg;
        cout << endl;
    }

    // Достъп по ключ
    cout << "--- Достъп по ключ ---" << endl;
    map<string, list<int> >::const_iterator itFound = mapGroups.find("OOP1KP");
    if (itFound != mapGroups.end())
        printList("OOP1KP", itFound->second);

    cout << "  count(\"OOP1\")   = " << mapGroups.count("OOP1") << endl;
    cout << "  count(\"НЯМА\")   = " << mapGroups.count("НЯМА") << endl;

    // ===== Шаблоните работят и с други типове =====
    cout << endl << "===== Шаблоните са независими от типа =====" << endl;

    set<string> setNames;
    setNames.insert("Петров");
    setNames.insert("Иванов");
    setNames.insert("Георгиев");
    setNames.insert("Иванов");                 // повторение - не влиза

    SetPrinter<string> printSetStr;
    printSetStr(setNames);

    set<int> setNums;
    setNums.insert(42);
    setNums.insert(7);
    setNums.insert(42);
    setNums.insert(13);

    SetPrinter<int> printSetInt;
    printSetInt(setNums);

    list<string> lstWords;
    lstWords.push_back("едно");
    lstWords.push_back("две");
    lstWords.push_back("три");

    ListPrinter<string> printListStr;
    printListStr("думи", lstWords);

    // Задържа конзолния прозорец отворен при стартиране с F5 (debug)
    getchar();
    return 0;
}
