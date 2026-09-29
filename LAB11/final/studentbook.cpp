// Тема 11: STL, контейнери, итератори, алгоритми.
// CStudent      - факултетен номер и успех.
// CStudentBook  - студентска книжка: студент + два УСПОРЕДНИ списъка
//                 (дисциплини и точки по тях).

#include <algorithm>  // max_element, min_element, find
#include <cstdio>     // getchar
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <numeric>    // accumulate
#include <stdexcept>  // runtime_error
#include <string>
using namespace std;

// ============================================================
//  Помощно: подравняване при кирилица (setw брои байтове)
// ============================================================
static size_t utf8Length(const string& str)
{
    size_t nLen = 0;
    for (size_t i = 0; i < str.length(); i++)
        if ((static_cast<unsigned char>(str[i]) & 0xC0) != 0x80)
            nLen++;
    return nLen;
}

static string padRight(const string& str, size_t nWidth)
{
    size_t nLen = utf8Length(str);
    if (nLen >= nWidth) return str;
    return str + string(nWidth - nLen, ' ');
}

// Подравняване ВДЯСНО - за заглавията на числовите колони.
static string padLeft(const string& str, size_t nWidth)
{
    size_t nLen = utf8Length(str);
    if (nLen >= nWidth) return str;
    return string(nWidth - nLen, ' ') + str;
}

// ============================================================
//  I. Клас CStudent
// ============================================================
class CStudent
{
private:
    string m_strFN;       // факултетен номер
    double m_dGrade;      // успех

public:
    CStudent();                                            // I.1
    CStudent(const string& strFN, double dGrade);          // I.2

    const string& getFN() const;
    double getGrade() const;

    void setGrade(double dGrade);                          // I.3 презапис на успех

    CStudent& operator=(const CStudent& stud);             // I.6

    friend ostream& operator<<(ostream& toStream, const CStudent& stud);   // I.4
    friend istream& operator>>(istream& fromStream, CStudent& stud);       // I.5
};

CStudent::CStudent() : m_strFN("(няма)"), m_dGrade(0.0) {}

CStudent::CStudent(const string& strFN, double dGrade)
    : m_strFN(strFN), m_dGrade(dGrade) {}

const string& CStudent::getFN() const { return m_strFN; }
double CStudent::getGrade() const     { return m_dGrade; }

// ---------- I.3 Презапис на успех ----------
void CStudent::setGrade(double dGrade)
{
    if (dGrade < 2.0 || dGrade > 6.0)
    {
        if (dGrade != 0.0)       // 0.0 значи "още няма успех" - допустимо е
        {
            cout << "  (отхвърлен успех: " << dGrade << ")" << endl;
            return;
        }
    }
    m_dGrade = dGrade;
}

// ---------- I.6 Оператор за присвояване ----------
// Класът няма указатели, така че генерираният би бил коректен.
// Пишем го, защото условието го изисква.
CStudent& CStudent::operator=(const CStudent& stud)
{
    if (this == &stud) return *this;
    m_strFN  = stud.m_strFN;
    m_dGrade = stud.m_dGrade;
    return *this;
}

// ---------- I.4 Извеждане ----------
ostream& operator<<(ostream& toStream, const CStudent& stud)
{
    toStream << "ф.н. " << stud.m_strFN << ", успех " << fixed << setprecision(2)
             << stud.m_dGrade;
    return toStream;
}

// ---------- I.5 Четене ----------
istream& operator>>(istream& fromStream, CStudent& stud)
{
    string strFN;
    double dGrade = 0.0;
    if (fromStream >> strFN >> dGrade)
    {
        stud.m_strFN  = strFN;
        stud.m_dGrade = dGrade;
    }
    return fromStream;
}

// ============================================================
//  II. Клас CStudentBook
// ============================================================
class CStudentBook
{
private:
    CStudent     m_student;        // обект от клас I
    list<string> m_lstSubjects;    // имена на дисциплини
    list<int>    m_lstPoints;      // точки, СЪОТВЕТСТВАЩИ на имената

    // Изчислява и връща списък с оценки по дисциплините
    list<int> calcScoresList() const;

public:
    CStudentBook();
    explicit CStudentBook(const CStudent& stud);           // II.1
    explicit CStudentBook(const string& strFileName);      // II.2

    const CStudent& getStudent() const;
    const list<string>& getSubjects() const;
    const list<int>& getPoints() const;

    ostream& Output(ostream& toStream) const;              // II.3

    int getMaxPoints() const;                              // II.4
    int getMinPoints() const;                              // II.5

    void addPoints(const string& strSubject, int nPoints); // II.6
    double calcAverage();                                  // II.7

    friend ostream& operator<<(ostream& toStream, const CStudentBook& book);  // II.8
    friend istream& operator>>(istream& fromStream, CStudentBook& book);      // II.9
};

CStudentBook::CStudentBook()
    : m_student(), m_lstSubjects(), m_lstPoints() {}

// ---------- II.1 Експлицитен конструктор с обект студент ----------
CStudentBook::CStudentBook(const CStudent& stud)
    : m_student(stud), m_lstSubjects(), m_lstPoints() {}

// ---------- II.2 Експлицитен конструктор с име на файл ----------
CStudentBook::CStudentBook(const string& strFileName)
    : m_student(), m_lstSubjects(), m_lstPoints()
{
    ifstream iFile(strFileName.c_str());
    if (!iFile)
        throw runtime_error("Не мога да отворя файла: " + strFileName);

    iFile >> *this;

    if (m_lstSubjects.empty())
        throw runtime_error("Файлът " + strFileName + " не съдържа дисциплини");
}

const CStudent&     CStudentBook::getStudent() const  { return m_student; }
const list<string>& CStudentBook::getSubjects() const { return m_lstSubjects; }
const list<int>&    CStudentBook::getPoints() const   { return m_lstPoints; }

// ---------- Точки -> оценка ----------
// Скалата е обявена на едно място, за да не се повтаря из кода.
list<int> CStudentBook::calcScoresList() const
{
    list<int> lstScores;
    for (list<int>::const_iterator it = m_lstPoints.begin();
         it != m_lstPoints.end(); ++it)
    {
        int nPts = *it;
        int nScore;
        if      (nPts >= 88) nScore = 6;      // отличен
        else if (nPts >= 75) nScore = 5;      // мн. добър
        else if (nPts >= 63) nScore = 4;      // добър
        else if (nPts >= 50) nScore = 3;      // среден
        else                 nScore = 2;      // слаб
        lstScores.push_back(nScore);
    }
    return lstScores;
}

// ---------- II.3 Извеждане ----------
ostream& CStudentBook::Output(ostream& toStream) const
{
    toStream << "Студентска книжка: " << m_student << endl;
    // padLeft, а не setw: заглавията са на кирилица (виж utf8Length)
    toStream << "  " << padRight("дисциплина", 14) << padLeft("точки", 7)
             << padLeft("оценка", 8) << endl;

    list<int> lstScores = calcScoresList();

    // ДВА успоредни списъка се обхождат с ДВА итератора едновременно.
    list<string>::const_iterator itSub = m_lstSubjects.begin();
    list<int>::const_iterator    itPts = m_lstPoints.begin();
    list<int>::const_iterator    itScr = lstScores.begin();

    while (itSub != m_lstSubjects.end() && itPts != m_lstPoints.end())
    {
        toStream << "  " << padRight(*itSub, 14)
                 << setw(7) << right << *itPts
                 << setw(8) << right << *itScr << endl;
        ++itSub;
        ++itPts;
        ++itScr;
    }
    return toStream;
}

ostream& operator<<(ostream& toStream, const CStudentBook& book)
{
    return book.Output(toStream);
}

// ---------- II.9 Четене ----------
// Формат: <ф.номер> <успех> CR, после по <дисциплина> <точки> на ред.
istream& operator>>(istream& fromStream, CStudentBook& book)
{
    if (!(fromStream >> book.m_student))
        return fromStream;

    book.m_lstSubjects.clear();
    book.m_lstPoints.clear();

    string strSubject;
    int    nPoints = 0;
    while (fromStream >> strSubject >> nPoints)
    {
        book.m_lstSubjects.push_back(strSubject);
        book.m_lstPoints.push_back(nPoints);
    }
    return fromStream;
}

// ---------- II.4 Максимални точки ----------
int CStudentBook::getMaxPoints() const
{
    if (m_lstPoints.empty()) return 0;
    return *max_element(m_lstPoints.begin(), m_lstPoints.end());
}

// ---------- II.5 Минимални точки ----------
int CStudentBook::getMinPoints() const
{
    if (m_lstPoints.empty()) return 0;
    return *min_element(m_lstPoints.begin(), m_lstPoints.end());
}

// ---------- II.6 Добавяне на точки по дисциплина ----------
// Ако дисциплината вече я има - точките се ДОБАВЯТ към нейните.
// Ако я няма - добавя се нов ред в двата списъка.
void CStudentBook::addPoints(const string& strSubject, int nPoints)
{
    list<string>::iterator itSub = m_lstSubjects.begin();
    list<int>::iterator    itPts = m_lstPoints.begin();

    while (itSub != m_lstSubjects.end() && itPts != m_lstPoints.end())
    {
        if (*itSub == strSubject)
        {
            *itPts += nPoints;             // намерена - добавяме
            return;
        }
        ++itSub;
        ++itPts;
    }

    // Не е намерена - добавяме нова двойка В ДВАТА списъка.
    m_lstSubjects.push_back(strSubject);
    m_lstPoints.push_back(nPoints);
}

// ---------- II.7 Среден успех ----------
// Изчислява средното от ОЦЕНКИТЕ и го записва в член-променливата студент.
double CStudentBook::calcAverage()
{
    list<int> lstScores = calcScoresList();
    if (lstScores.empty())
    {
        m_student.setGrade(0.0);
        return 0.0;
    }

    int nSum = accumulate(lstScores.begin(), lstScores.end(), 0);
    double dAvg = static_cast<double>(nSum) / lstScores.size();

    m_student.setGrade(dAvg);              // записва се в обекта студент
    return dAvg;
}

// ============================================================
//  III. Главна функция
// ============================================================
int main()
{
    cout << fixed << setprecision(2);

    // ===== III.1 Създаване на обекти от I и II =====
    cout << "===== III.1 Създаване на обекти =====" << endl;
    CStudent stud("61462103", 0.0);
    cout << "  студент: " << stud << endl;

    CStudentBook book(stud);               // II.1 - по обект студент
    cout << "  книжка, създадена по обект студент:" << endl;
    cout << book;

    // ===== III.2 Добавяне на точки по 5 дисциплини =====
    cout << endl << "===== III.2 Добавяне на точки (addPoints) =====" << endl;
    book.addPoints("OOP1", 20);
    book.addPoints("OOP2", 40);
    book.addPoints("OOP3", 60);
    book.addPoints("OOP4", 80);
    book.addPoints("OOP5", 100);
    cout << book;

    // addPoints върху съществуваща дисциплина ДОБАВЯ точки
    cout << "--- addPoints(\"OOP1\", 45) върху съществуваща дисциплина ---" << endl;
    book.addPoints("OOP1", 45);
    cout << "  OOP1 вече има " << book.getPoints().front() << " точки" << endl;
    cout << "  брой дисциплини остава: " << book.getSubjects().size() << endl;

    // Връщаме стойността, за да съвпаднат следващите тестове с условието
    book.addPoints("OOP1", -45);

    // ===== III.3 Среден успех =====
    cout << endl << "===== III.3 Среден успех (calcAverage) =====" << endl;
    cout << "  успех преди: " << book.getStudent().getGrade() << endl;
    double dAvg = book.calcAverage();
    cout << "  calcAverage() върна: " << dAvg << endl;
    cout << "  успех след:  " << book.getStudent().getGrade()
         << "   <- записан в обекта студент" << endl;

    // ===== III.4 Извеждане на всички данни =====
    cout << endl << "===== III.4 Всички данни =====" << endl;
    cout << book;

    // ===== III.5 и III.6 Макс и мин точки =====
    cout << endl << "===== III.5-III.6 Макс и мин точки =====" << endl;
    cout << "  максимални точки: " << book.getMaxPoints() << endl;
    cout << "  минимални точки:  " << book.getMinPoints() << endl;

    // ===== III.7 Създаване чрез файл =====
    cout << endl << "===== III.7 Създаване чрез файл =====" << endl;

    // а) несъществуващ файл
    try
    {
        CStudentBook bookBad("няма-такъв-файл.txt");
        cout << bookBad;
    }
    catch (const exception& ex)
    {
        cout << "  хванато изключение: " << ex.what() << endl;
    }

    // б) истинският файл
    try
    {
        CStudentBook bookFile("Tema11.txt");
        cout << "--- прочетено от Tema11.txt ---" << endl;
        cout << bookFile;

        bookFile.calcAverage();
        cout << "--- след calcAverage() ---" << endl;
        cout << bookFile;
        cout << "  макс: " << bookFile.getMaxPoints()
             << ", мин: " << bookFile.getMinPoints() << endl;

        // Извеждане във файл - същият оператор
        ofstream oFile("Tema11_out.txt");
        if (oFile)
        {
            oFile << fixed << setprecision(2);
            oFile << bookFile;
            oFile.close();
            cout << "  записано и в Tema11_out.txt" << endl;
        }
    }
    catch (const exception& ex)
    {
        cout << "  хванато изключение: " << ex.what() << endl;
    }

    // ===== Операции със списъка =====
    cout << endl << "===== Операции с list =====" << endl;
    list<int> lstPts = book.getPoints();
    cout << "  size()   = " << lstPts.size() << endl;
    cout << "  front()  = " << lstPts.front() << endl;
    cout << "  back()   = " << lstPts.back() << endl;

    // list няма operator[] - обхожда се само с итератори
    cout << "  съдържание: ";
    for (list<int>::const_iterator it = lstPts.begin(); it != lstPts.end(); ++it)
        cout << *it << " ";
    cout << endl;

    // list има СОБСТВЕН sort - std::sort не работи върху него
    lstPts.sort();
    cout << "  след lstPts.sort(): ";
    for (list<int>::const_iterator it = lstPts.begin(); it != lstPts.end(); ++it)
        cout << *it << " ";
    cout << endl;

    lstPts.reverse();
    cout << "  след reverse():     ";
    for (list<int>::const_iterator it = lstPts.begin(); it != lstPts.end(); ++it)
        cout << *it << " ";
    cout << endl;

    // Задържа конзолния прозорец отворен при стартиране с F5 (debug)
    getchar();
    return 0;
}
