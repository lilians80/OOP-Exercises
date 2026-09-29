// Тема 9: Контейнерни класове, алгоритми.
// CExamRow - един ред от условието на изпитна задача.
// CStudent - студент с вектор от такива редове и сумарни точки.

#include <algorithm>  // sort, find, count_if, max_element
#include <cstdio>     // getchar
#include <iomanip>    // setw, left, right
#include <iostream>
#include <numeric>    // accumulate
#include <string>
#include <vector>
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

// ============================================================
//  I. Клас CExamRow - един ред от условието
// ============================================================
class CExamRow
{
// Скрити член променливи
private:
    string m_strPart;       // име на частта, напр. "I."
    int    m_nPoints;       // точки
    string m_strContent;    // съдържание, напр. "Създаване_на_обекти"

public:
    // I.1 Три конструктора
    CExamRow();                                                     // подразбиращ се
    CExamRow(const string& strPart, int nPoints, const string& strContent);
    CExamRow(const CExamRow& row);                                  // копиращ

    // I.5 Четене на точките
    int getPoints() const;

    // Допълнителни акцесори - нужни при извеждането
    const string& getPart() const;
    const string& getContent() const;

    // I.2 Сравнение за по-малко ПО ИМЕ НА ЧАСТТА
    bool operator<(const CExamRow& row) const;

    // I.3 Сравнение за равенство ПО ИМЕ НА ЧАСТТА И СЪДЪРЖАНИЕ
    bool operator==(const CExamRow& row) const;

    // I.4 Извеждане в поток
    friend ostream& operator<<(ostream& toStream, const CExamRow& row);

    // I.6 Статична функция: текущи точки + обект -> нови точки
    static int addPoints(int nCurrent, const CExamRow& row);
};

// ---------- I.1 Конструктори ----------
CExamRow::CExamRow()
    : m_strPart("(няма)"), m_nPoints(0), m_strContent("(няма)") {}

CExamRow::CExamRow(const string& strPart, int nPoints, const string& strContent)
    : m_strPart(strPart), m_nPoints(nPoints), m_strContent(strContent) {}

CExamRow::CExamRow(const CExamRow& row)
    : m_strPart(row.m_strPart), m_nPoints(row.m_nPoints), m_strContent(row.m_strContent) {}

// ---------- I.5 Четене ----------
int CExamRow::getPoints() const              { return m_nPoints; }
const string& CExamRow::getPart() const      { return m_strPart; }
const string& CExamRow::getContent() const   { return m_strContent; }

// ---------- I.2 Сравнение за по-малко: САМО по име на частта ----------
bool CExamRow::operator<(const CExamRow& row) const
{
    return m_strPart < row.m_strPart;
}

// ---------- I.3 Равенство: по име на частта И съдържание ----------
// Точките НЕ участват - същата задача, решена за други точки, е същата задача.
bool CExamRow::operator==(const CExamRow& row) const
{
    return m_strPart == row.m_strPart && m_strContent == row.m_strContent;
}

// ---------- I.4 Извеждане ----------
ostream& operator<<(ostream& toStream, const CExamRow& row)
{
    toStream << padRight(row.m_strPart, 6)
             << setw(4) << right << row.m_nPoints << " т.  "
             << row.m_strContent;
    return toStream;
}

// ---------- I.6 Статична функция за натрупване ----------
// Сигнатурата (натрупано, елемент) -> натрупано е точно тази, която
// очаква алгоритъмът accumulate от <numeric>.
int CExamRow::addPoints(int nCurrent, const CExamRow& row)
{
    return nCurrent + row.m_nPoints;
}

// ============================================================
//  II. Клас CStudent
// ============================================================
class CStudent
{
private:
    string           m_strName;    // име на студента
    vector<CExamRow> m_vecRows;    // вектор от обекти от клас CExamRow
    int              m_nTotal;     // сумарни точки от изпита

public:
    // II.1 Конструктори
    CStudent();
    CStudent(const string& strName, const vector<CExamRow>& vecRows);

    // II.3 Четене на променливите
    const string& getName() const;
    const vector<CExamRow>& getRows() const;
    int getTotal() const;

    // II.2 Извеждане на вектора на указан поток
    ostream& Output(ostream& toStream) const;

    // II.4 Изпитване: по подадения вектор-еталон изчислява и записва точките
    int examination(const vector<CExamRow>& vecEtalon);

    // II.5 Извеждане: име и сумарни точки
    friend ostream& operator<<(ostream& toStream, const CStudent& stud);

    // II.6 Сравнение "<" по сумарни точки
    bool operator<(const CStudent& stud) const;
};

// ---------- II.1 Конструктори ----------
CStudent::CStudent()
    : m_strName("(няма име)"), m_vecRows(), m_nTotal(0) {}

CStudent::CStudent(const string& strName, const vector<CExamRow>& vecRows)
    : m_strName(strName), m_vecRows(vecRows), m_nTotal(0) {}

// ---------- II.3 Четене ----------
const string& CStudent::getName() const              { return m_strName; }
const vector<CExamRow>& CStudent::getRows() const    { return m_vecRows; }
int CStudent::getTotal() const                       { return m_nTotal; }

// ---------- II.2 Извеждане на вектора ----------
ostream& CStudent::Output(ostream& toStream) const
{
    toStream << m_strName << "  (" << m_vecRows.size() << " реда)" << endl;
    for (size_t i = 0; i < m_vecRows.size(); i++)
        toStream << "     " << m_vecRows[i] << endl;
    return toStream;
}

// ---------- II.4 Изпитване ----------
// За всеки ред на студента търсим същия ред в еталона (operator==).
// Ако го има - добавяме точките ОТ ЕТАЛОНА, не тези на студента.
int CStudent::examination(const vector<CExamRow>& vecEtalon)
{
    m_nTotal = 0;
    for (size_t i = 0; i < m_vecRows.size(); i++)
    {
        vector<CExamRow>::const_iterator it =
            find(vecEtalon.begin(), vecEtalon.end(), m_vecRows[i]);

        if (it != vecEtalon.end())
            m_nTotal = CExamRow::addPoints(m_nTotal, *it);   // I.6
    }
    return m_nTotal;
}

// ---------- II.5 Извеждане: име и сумарни точки ----------
ostream& operator<<(ostream& toStream, const CStudent& stud)
{
    toStream << padRight(stud.m_strName, 18) << setw(4) << right << stud.m_nTotal << " т.";
    return toStream;
}

// ---------- II.6 Сравнение по сумарни точки ----------
bool CStudent::operator<(const CStudent& stud) const
{
    return m_nTotal < stud.m_nTotal;
}

// ============================================================
//  III. Главна функция
// ============================================================
int main()
{
    // ===== Еталонът: пълното условие на задачата =====
    vector<CExamRow> vecEtalon;
    vecEtalon.push_back(CExamRow("I.",   10, "Създаване_на_клас_с_частни_членове"));
    vecEtalon.push_back(CExamRow("II.",  15, "Създаване_на_обекти_с_експлицитен_конструктор"));
    vecEtalon.push_back(CExamRow("III.", 20, "Предефиниране_на_оператор_за_извеждане"));
    vecEtalon.push_back(CExamRow("IV.",  25, "Работа_с_вектор_от_обекти"));
    vecEtalon.push_back(CExamRow("V.",   30, "Прилагане_на_алгоритъм_от_STL"));

    // ===== III.1 Два обекта от клас CStudent =====
    CStudent examiner("ИЗПИТВАЩ (еталон)", vecEtalon);

    // Студентът е решил част от задачите; една с различно съдържание.
    vector<CExamRow> vecStud;
    vecStud.push_back(CExamRow("I.",   10, "Създаване_на_клас_с_частни_членове"));
    vecStud.push_back(CExamRow("II.",  15, "Създаване_на_обекти_с_експлицитен_конструктор"));
    vecStud.push_back(CExamRow("III.",  0, "Предефиниране_на_оператор_за_въвеждане"));
    vecStud.push_back(CExamRow("V.",   30, "Прилагане_на_алгоритъм_от_STL"));

    CStudent student("Иван Петров", vecStud);

    cout << "===== III.1 Извеждане чрез Output =====" << endl;
    cout << "--- " ;
    examiner.Output(cout);
    cout << "--- ";
    student.Output(cout);

    // ===== III.2 Изпитване и извеждане чрез << =====
    cout << endl << "===== III.2 Изпитване =====" << endl;
    int nResult = student.examination(vecEtalon);
    cout << "  examination() върна: " << nResult << " точки" << endl;
    cout << "  чрез оператор <<:    " << student << endl;

    cout << "--- Как се получи резултатът ---" << endl;
    const vector<CExamRow>& vecRows = student.getRows();
    for (size_t i = 0; i < vecRows.size(); i++)
    {
        vector<CExamRow>::const_iterator it =
            find(vecEtalon.begin(), vecEtalon.end(), vecRows[i]);
        cout << "  " << padRight(vecRows[i].getPart(), 6)
             << (it != vecEtalon.end()
                 ? "намерен в еталона -> +" + to_string(it->getPoints()) + " т."
                 : string("НЕ съвпада с еталона -> +0 т."))
             << endl;
    }

    // ===== Алгоритми върху вектора =====
    cout << endl << "===== Алгоритми от STL =====" << endl;

    // accumulate със статичната функция I.6
    int nMaxPossible = accumulate(vecEtalon.begin(), vecEtalon.end(),
                                  0, CExamRow::addPoints);
    cout << "  максимално възможни точки (accumulate): " << nMaxPossible << endl;
    cout << "  получени от студента:                   " << student.getTotal() << endl;
    cout << "  процент:                                "
         << (student.getTotal() * 100 / nMaxPossible) << "%" << endl;

    // max_element с operator<
    vector<CExamRow>::const_iterator itMax =
        max_element(vecEtalon.begin(), vecEtalon.end());
    cout << "  последна част по азбучен ред (max_element): " << itMax->getPart() << endl;

    // count_if с ламбда
    int nHeavy = static_cast<int>(count_if(vecEtalon.begin(), vecEtalon.end(),
                      [](const CExamRow& row) { return row.getPoints() >= 20; }));
    cout << "  задачи с 20 или повече точки (count_if):    " << nHeavy << endl;

    // sort с operator< на CExamRow - по ИМЕ НА ЧАСТТА
    cout << "--- sort по име на частта (operator< на CExamRow) ---" << endl;
    vector<CExamRow> vecSorted = vecEtalon;
    sort(vecSorted.begin(), vecSorted.end());
    for (size_t i = 0; i < vecSorted.size(); i++)
        cout << "  " << vecSorted[i] << endl;

    // ===== Класация на студенти: sort с operator< на CStudent =====
    cout << endl << "===== Класация (sort по сумарни точки) =====" << endl;

    vector<CStudent> vecStudents;
    vecStudents.push_back(student);

    vector<CExamRow> vecA;
    vecA.push_back(vecEtalon[0]);
    vecA.push_back(vecEtalon[3]);
    CStudent studA("Елена Димитрова", vecA);
    studA.examination(vecEtalon);
    vecStudents.push_back(studA);

    vector<CExamRow> vecB = vecEtalon;                 // решил е всичко
    CStudent studB("Георги Стоянов", vecB);
    studB.examination(vecEtalon);
    vecStudents.push_back(studB);

    CStudent studC("Мария Иванова", vector<CExamRow>());   // празен вектор
    studC.examination(vecEtalon);
    vecStudents.push_back(studC);

    cout << "--- преди сортиране ---" << endl;
    for (size_t i = 0; i < vecStudents.size(); i++)
        cout << "  " << vecStudents[i] << endl;

    sort(vecStudents.begin(), vecStudents.end());       // възходящо, operator<
    cout << "--- след sort (възходящо по точки) ---" << endl;
    for (size_t i = 0; i < vecStudents.size(); i++)
        cout << "  " << vecStudents[i] << endl;

    // Низходящо - чрез обратни итератори
    cout << "--- низходящо (rbegin/rend) ---" << endl;
    for (vector<CStudent>::const_reverse_iterator it = vecStudents.rbegin();
         it != vecStudents.rend(); ++it)
        cout << "  " << *it << endl;

    // ===== Основни операции с вектора =====
    cout << endl << "===== Операции с vector =====" << endl;
    cout << "  size()      = " << vecEtalon.size() << endl;
    cout << "  empty()     = " << (vecEtalon.empty() ? "да" : "не") << endl;
    cout << "  front()     = " << vecEtalon.front().getPart() << endl;
    cout << "  back()      = " << vecEtalon.back().getPart() << endl;
    cout << "  at(2)       = " << vecEtalon.at(2).getPart() << endl;

    // at() проверява индекса и хвърля - за разлика от []
    try
    {
        // Първо работата, после извеждането - иначе редът излиза разкъсан.
        const string& strPart = vecEtalon.at(99).getPart();
        cout << "  at(99)      = " << strPart << endl;
    }
    catch (const out_of_range& ex)
    {
        // Текстът на съобщението е различен при различните компилатори.
        cout << "  at(99)      -> хванато out_of_range: " << ex.what() << endl;
    }

    // Задържа конзолния прозорец отворен при стартиране с F5 (debug)
    getchar();
    return 0;
}
