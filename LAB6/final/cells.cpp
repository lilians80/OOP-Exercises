// Тема 6: Абстрактни класове, типови преобразувания, изключения,
//         оператори и шаблони.
// Описател на клетка на таблица - базов абстрактен клас и два наследника.

#include <cstdio>     // getchar
#include <exception>
#include <iomanip>    // setw, left, right
#include <iostream>
#include <stdexcept>  // invalid_argument, out_of_range
#include <string>
using namespace std;

// ============================================================
//  Собствен клас за изключения
// ============================================================
class CCellException : public exception
{
private:
    string m_strMsg;

public:
    explicit CCellException(const string& strMsg) : m_strMsg(strMsg) {}

    // what() е виртуална в std::exception - подменяме я.
    // noexcept е задължително: базовата версия обещава да не хвърля.
    virtual const char* what() const noexcept { return m_strMsg.c_str(); }
};

// ============================================================
//  Помощна функция: подравняване при кирилица
// ============================================================
// setw брои БАЙТОВЕ, а кирилската буква в UTF-8 е 2 байта. Затова
// setw(10) върху "Иван" не добавя нищо - низът вече е 8 байта.
// Тази функция брои БУКВИ: началните байтове на UTF-8 знак са тези,
// при които горните два бита НЕ са 10.
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
//  I. Абстрактен клас CCellDescrBase
// ============================================================
class CCellDescrBase
{
// Скрити член променливи. protected, за да ги достигат наследниците.
protected:
    string m_str_val;     // стойност на клетката, представя цяло число или низ
    int    m_row;         // ред
    int    m_col;         // колона

public:
    // Границите на таблицата - статични, общи за всички клетки
    static const int MAX_ROW = 100;
    static const int MAX_COL = 26;

    // Конструктори
    CCellDescrBase();                                             // подразбиращ се
    CCellDescrBase(const string& strVal, int nRow, int nCol);     // експлицитен
    CCellDescrBase(const CCellDescrBase& cell);                   // копиращ

    virtual ~CCellDescrBase();                                    // виртуален!

    // Акцесори / мутатори
    int  getRow() const;
    int  getCol() const;
    void setRow(int nRow);        // хвърля CCellException при невалиден ред
    void setCol(int nCol);        // хвърля CCellException при невалидна колона

    const string& getValue() const;
    void setValue(const string& strVal);

    // Адрес на клетката в човешки вид: A1, B3, ...
    string getAddress() const;

    // Напълно виртуална функция за извеждане в поток
    virtual ostream& Output(ostream& toStream) const = 0;
};

const int CCellDescrBase::MAX_ROW;
const int CCellDescrBase::MAX_COL;

CCellDescrBase::CCellDescrBase()
    : m_str_val(""), m_row(0), m_col(0) {}

CCellDescrBase::CCellDescrBase(const string& strVal, int nRow, int nCol)
    : m_str_val(strVal), m_row(0), m_col(0)
{
    setRow(nRow);       // през мутаторите, за да мине проверката
    setCol(nCol);
}

CCellDescrBase::CCellDescrBase(const CCellDescrBase& cell)
    : m_str_val(cell.m_str_val), m_row(cell.m_row), m_col(cell.m_col) {}

CCellDescrBase::~CCellDescrBase() {}

int CCellDescrBase::getRow() const { return m_row; }
int CCellDescrBase::getCol() const { return m_col; }

void CCellDescrBase::setRow(int nRow)
{
    if (nRow < 0 || nRow >= MAX_ROW)
        throw CCellException("Невалиден ред: " + to_string(nRow) +
                             " (допустимо 0.." + to_string(MAX_ROW - 1) + ")");
    m_row = nRow;
}

void CCellDescrBase::setCol(int nCol)
{
    if (nCol < 0 || nCol >= MAX_COL)
        throw CCellException("Невалидна колона: " + to_string(nCol) +
                             " (допустимо 0.." + to_string(MAX_COL - 1) + ")");
    m_col = nCol;
}

const string& CCellDescrBase::getValue() const      { return m_str_val; }
void CCellDescrBase::setValue(const string& strVal) { m_str_val = strVal; }

string CCellDescrBase::getAddress() const
{
    string strAddr;
    strAddr += static_cast<char>('A' + m_col);
    strAddr += to_string(m_row + 1);
    return strAddr;
}

// ---------- Един оператор за цялата йерархия ----------
// Делегира на виртуалната Output - затова извежда според ТИПА НА ОБЕКТА.
ostream& operator<<(ostream& toStream, const CCellDescrBase& cell)
{
    return cell.Output(toStream);
}

// ============================================================
//  II. CCellDescrInteger - клетка с целочислени данни
// ============================================================
class CCellDescrInteger : public CCellDescrBase
{
public:
    CCellDescrInteger();
    CCellDescrInteger(const string& strVal, int nRow, int nCol);
    CCellDescrInteger(int nVal, int nRow, int nCol);          // удобен вариант
    CCellDescrInteger(const CCellDescrInteger& cell);
    virtual ~CCellDescrInteger();

    // Четене на данните като цяло число; хвърля при невалидна стойност
    int getInt() const;

    // Оператор за присвояване
    CCellDescrInteger& operator=(const CCellDescrInteger& cell);

    // Сравнения по ЦЕЛОЧИСЛЕНАТА стойност
    bool operator==(const CCellDescrInteger& cell) const;
    bool operator< (const CCellDescrInteger& cell) const;

    // Извеждане: числата се подравняват ВДЯСНО
    virtual ostream& Output(ostream& toStream) const;
};

CCellDescrInteger::CCellDescrInteger() : CCellDescrBase("0", 0, 0) {}

CCellDescrInteger::CCellDescrInteger(const string& strVal, int nRow, int nCol)
    : CCellDescrBase(strVal, nRow, nCol) {}

CCellDescrInteger::CCellDescrInteger(int nVal, int nRow, int nCol)
    : CCellDescrBase(to_string(nVal), nRow, nCol) {}

CCellDescrInteger::CCellDescrInteger(const CCellDescrInteger& cell)
    : CCellDescrBase(cell) {}

CCellDescrInteger::~CCellDescrInteger() {}

// ---------- Типовото преобразуване низ -> цяло число ----------
int CCellDescrInteger::getInt() const
{
    try
    {
        size_t nPos = 0;
        int nVal = stoi(m_str_val, &nPos);

        // stoi спира при първия негоден символ. Ако е останало нещо
        // освен празни знаци, стойността не е цяло число.
        while (nPos < m_str_val.length() && isspace(static_cast<unsigned char>(m_str_val[nPos])))
            nPos++;
        if (nPos != m_str_val.length())
            throw CCellException("Клетка " + getAddress() + ": \"" + m_str_val +
                                 "\" не е цяло число");
        return nVal;
    }
    catch (const invalid_argument&)
    {
        throw CCellException("Клетка " + getAddress() + ": \"" + m_str_val +
                             "\" не е цяло число");
    }
    catch (const out_of_range&)
    {
        throw CCellException("Клетка " + getAddress() + ": \"" + m_str_val +
                             "\" е извън обхвата на int");
    }
}

CCellDescrInteger& CCellDescrInteger::operator=(const CCellDescrInteger& cell)
{
    if (this == &cell) return *this;
    m_str_val = cell.m_str_val;
    m_row     = cell.m_row;
    m_col     = cell.m_col;
    return *this;
}

bool CCellDescrInteger::operator==(const CCellDescrInteger& cell) const
{
    return getInt() == cell.getInt();         // сравнява ЧИСЛАТА
}

bool CCellDescrInteger::operator<(const CCellDescrInteger& cell) const
{
    return getInt() < cell.getInt();
}

ostream& CCellDescrInteger::Output(ostream& toStream) const
{
    toStream << "[" << setw(3) << right << getAddress() << "] "
             << setw(10) << right;
    try
    {
        toStream << getInt();                 // числото, подравнено вдясно
    }
    catch (const CCellException&)
    {
        toStream << "#ЧИСЛО!";                // невалидна стойност в числова клетка
    }
    return toStream;
}

// ============================================================
//  III. CCellDescrString - клетка със стрингови данни
// ============================================================
class CCellDescrString : public CCellDescrBase
{
public:
    CCellDescrString();
    CCellDescrString(const string& strVal, int nRow, int nCol);
    CCellDescrString(const CCellDescrString& cell);
    virtual ~CCellDescrString();

    // Четене на данните като низ
    string getString() const;

    CCellDescrString& operator=(const CCellDescrString& cell);

    // Сравнения по СТРИНГОВАТА стойност
    bool operator==(const CCellDescrString& cell) const;
    bool operator< (const CCellDescrString& cell) const;

    // Извеждане: текстът се подравнява ВЛЯВО и се огражда с кавички
    virtual ostream& Output(ostream& toStream) const;
};

CCellDescrString::CCellDescrString() : CCellDescrBase("", 0, 0) {}

CCellDescrString::CCellDescrString(const string& strVal, int nRow, int nCol)
    : CCellDescrBase(strVal, nRow, nCol) {}

CCellDescrString::CCellDescrString(const CCellDescrString& cell)
    : CCellDescrBase(cell) {}

CCellDescrString::~CCellDescrString() {}

string CCellDescrString::getString() const { return m_str_val; }

CCellDescrString& CCellDescrString::operator=(const CCellDescrString& cell)
{
    if (this == &cell) return *this;
    m_str_val = cell.m_str_val;
    m_row     = cell.m_row;
    m_col     = cell.m_col;
    return *this;
}

bool CCellDescrString::operator==(const CCellDescrString& cell) const
{
    return m_str_val == cell.m_str_val;       // сравнява НИЗОВЕТЕ
}

bool CCellDescrString::operator<(const CCellDescrString& cell) const
{
    return m_str_val < cell.m_str_val;        // лексикографски
}

ostream& CCellDescrString::Output(ostream& toStream) const
{
    // padRight вместо setw: брои букви, не байтове (виж utf8Length)
    toStream << "[" << setw(3) << right << getAddress() << "] "
             << padRight("\"" + m_str_val + "\"", 10);
    return toStream;
}

// ============================================================
//  Шаблонен клас за сравнение CLess
// ============================================================
template <class T>
class CLess
{
public:
    // Оператор-функция: прави обекта извикваем като функция.
    bool operator()(const T& left, const T& right) const
    {
        return left < right;
    }
};

// Помощна шаблонна функция - връща по-малкия от двата, чрез CLess
template <class T>
const T& Smaller(const T& left, const T& right)
{
    CLess<T> less;
    return less(left, right) ? left : right;
}

// ============================================================
//  IV. Главна функция
// ============================================================
int main()
{
    // ===== 1-2. Масив от указатели към БАЗОВИЯ клас =====
    cout << "===== 1-2. Масив от указатели към CCellDescrBase =====" << endl;

    const int nCells = 6;
    CCellDescrBase* arrCells[nCells];         // равен брой от двата вида

    arrCells[0] = new CCellDescrInteger(42, 0, 0);          // A1
    arrCells[1] = new CCellDescrString("Иван", 0, 1);       // B1
    arrCells[2] = new CCellDescrInteger(-7, 1, 0);          // A2
    arrCells[3] = new CCellDescrString("Варна", 1, 1);      // B2
    arrCells[4] = new CCellDescrInteger(1000, 2, 0);        // A3
    arrCells[5] = new CCellDescrString("ООП", 2, 1);        // B3

    // ===== 3. Извеждане чрез операторите - всеки вид по своя начин =====
    cout << "--- Числата вдясно, текстът вляво ---" << endl;
    for (int i = 0; i < nCells; i++)
        cout << "  " << *arrCells[i] << "   <- " 
             << (i % 2 == 0 ? "целочислена" : "стрингова") << endl;

    // ===== 4. Обработка на изключения =====
    cout << endl << "===== 4. Изключения =====" << endl;

    // а) невалидна стойност в целочислена клетка
    cout << "--- Невалидна стойност ---" << endl;
    CCellDescrInteger cellBad("не-число", 3, 0);
    try
    {
        int nVal = cellBad.getInt();
        cout << "  стойност: " << nVal << endl;
    }
    catch (const CCellException& ex)
    {
        cout << "  хванато изключение: " << ex.what() << endl;
    }

    // б) стойност извън обхвата на int
    cout << "--- Извън обхвата на int ---" << endl;
    CCellDescrInteger cellHuge("99999999999999999999", 3, 1);
    try
    {
        // Извикваме ПРЕДИ извеждането: иначе "стойност: " вече е на екрана,
        // когато изключението бъде хвърлено, и редът излиза разкъсан.
        int nHuge = cellHuge.getInt();
        cout << "  стойност: " << nHuge << endl;
    }
    catch (const CCellException& ex)
    {
        cout << "  хванато изключение: " << ex.what() << endl;
    }

    // в) невалиден ред
    cout << "--- Невалиден ред ---" << endl;
    try
    {
        CCellDescrInteger cellErr(1, -5, 0);
        cout << "  създадена клетка " << cellErr.getAddress() << endl;
    }
    catch (const CCellException& ex)
    {
        cout << "  хванато изключение: " << ex.what() << endl;
    }

    // г) невалидна колона
    cout << "--- Невалидна колона ---" << endl;
    try
    {
        CCellDescrString cellErr("текст", 0, 99);
        cout << "  създадена клетка " << cellErr.getAddress() << endl;
    }
    catch (const CCellException& ex)
    {
        cout << "  хванато изключение: " << ex.what() << endl;
    }

    // д) невалиден индекс в масива
    cout << "--- Невалиден индекс в масива ---" << endl;
    const int arrIdx[] = { 2, 99, 4 };
    for (int k = 0; k < 3; k++)
    {
        int i = arrIdx[k];
        try
        {
            if (i < 0 || i >= nCells)
                throw CCellException("Невалиден индекс " + to_string(i) +
                                     " (допустимо 0.." + to_string(nCells - 1) + ")");
            cout << "  arrCells[" << i << "] = " << *arrCells[i] << endl;
        }
        catch (const CCellException& ex)
        {
            cout << "  хванато изключение: " << ex.what() << endl;
        }
    }

    // е) клетката с невалидна стойност се извежда, без да събори програмата
    cout << "--- Извеждане на повредена клетка ---" << endl;
    cout << "  " << cellBad << "   <- Output хваща изключението вътре" << endl;

    // ===== Оператори за сравнение =====
    cout << endl << "===== Оператори == и < =====" << endl;
    CCellDescrInteger nA(42, 0, 0), nB(42, 5, 5), nC(7, 1, 1);
    cout << "  nA(42) == nB(42) [различни позиции]: " << (nA == nB ? "да" : "не") << endl;
    cout << "  nA(42) == nC(7):                     " << (nA == nC ? "да" : "не") << endl;
    cout << "  nC(7)  <  nA(42):                    " << (nC < nA ? "да" : "не") << endl;

    CCellDescrString sA("Ана", 0, 0), sB("Ана", 3, 3), sC("Борис", 1, 1);
    cout << "  sA(Ана) == sB(Ана):                  " << (sA == sB ? "да" : "не") << endl;
    cout << "  sA(Ана) <  sC(Борис) [лексикограф.]: " << (sA < sC ? "да" : "не") << endl;

    // Разликата между двата вида сравнение
    cout << "--- Едни и същи данни, различно сравнение ---" << endl;
    CCellDescrInteger n9("9", 0, 0), n10("10", 0, 1);
    CCellDescrString  s9("9", 0, 0), s10("10", 0, 1);
    cout << "  като ЧИСЛА:  9 < 10 = " << (n9 < n10 ? "да" : "не") << endl;
    cout << "  като НИЗОВЕ: \"9\" < \"10\" = " << (s9 < s10 ? "да" : "не")
         << "   <- '9' е след '1' в таблицата" << endl;

    // ===== Оператор за присвояване =====
    cout << endl << "===== Оператор = =====" << endl;
    CCellDescrInteger nDst(0, 4, 4);
    cout << "  преди: " << nDst << endl;
    nDst = nA;
    cout << "  след nDst = nA: " << nDst << endl;

    // ===== Шаблонният клас CLess =====
    cout << endl << "===== Шаблонен клас CLess =====" << endl;
    CLess<int>    lessInt;
    CLess<string> lessStr;
    CLess<double> lessDbl;

    cout << "  CLess<int>(3, 8)          = " << (lessInt(3, 8) ? "да" : "не") << endl;
    cout << "  CLess<int>(8, 3)          = " << (lessInt(8, 3) ? "да" : "не") << endl;
    cout << "  CLess<string>(Ана, Борис) = " << (lessStr("Ана", "Борис") ? "да" : "не") << endl;
    cout << "  CLess<double>(2.5, 2.4)   = " << (lessDbl(2.5, 2.4) ? "да" : "не") << endl;

    // Същият шаблон работи и върху нашите класове - те имат operator<
    CLess<CCellDescrInteger> lessCellInt;
    CLess<CCellDescrString>  lessCellStr;
    cout << "  CLess<CCellDescrInteger>(nC(7), nA(42)) = "
         << (lessCellInt(nC, nA) ? "да" : "не") << endl;
    cout << "  CLess<CCellDescrString>(sA, sC)         = "
         << (lessCellStr(sA, sC) ? "да" : "не") << endl;

    cout << "--- Шаблонна функция, която ползва CLess ---" << endl;
    cout << "  Smaller(15, 4)           = " << Smaller(15, 4) << endl;
    cout << "  Smaller(string) на 2 низа= " << Smaller(string("круша"), string("ябълка")) << endl;
    cout << "  Smaller(nC, nA)          = " << Smaller(nC, nA) << endl;

    // Сортиране на масив от клетки чрез CLess
    cout << "--- Сортиране на числови клетки чрез CLess ---" << endl;
    CCellDescrInteger arrSort[5] = {
        CCellDescrInteger(50, 0, 0), CCellDescrInteger(-3, 1, 0),
        CCellDescrInteger(17, 2, 0), CCellDescrInteger(0, 3, 0),
        CCellDescrInteger(99, 4, 0)
    };
    CLess<CCellDescrInteger> comp;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4 - i; j++)
            if (comp(arrSort[j + 1], arrSort[j]))       // дясната е по-малка
            {
                CCellDescrInteger tmp = arrSort[j];
                arrSort[j] = arrSort[j + 1];
                arrSort[j + 1] = tmp;
            }
    for (int i = 0; i < 5; i++)
        cout << "  " << arrSort[i] << endl;

    // ===== Освобождаване на паметта =====
    cout << endl << "===== Освобождаване =====" << endl;
    for (int i = 0; i < nCells; i++)
    {
        delete arrCells[i];       // вика правилния деструктор - той е virtual
        arrCells[i] = nullptr;
    }
    cout << "  всички " << nCells << " клетки са освободени" << endl;

    // Задържа конзолния прозорец отворен при стартиране с F5 (debug)
    getchar();
    return 0;
}
