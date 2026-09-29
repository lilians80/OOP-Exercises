// Тема 12: Асоциативни контейнери, итератори, алгоритми.
// CCar      - автомобил; статични карти код на град <-> град.
// CRegister - регистър на автомобили (list<CCar>).

#include <algorithm>  // find, find_if
#include <cctype>     // isdigit, isalpha
#include <cstdio>     // getchar
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
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

// ============================================================
//  I. Клас CCar
// ============================================================
class CCar
{
// Скрити членове
private:
    int    m_nPower;        // мощност
    string m_strModel;      // модел
    string m_strPlate;      // регистрационен номер
    string m_strOwner;      // собственик

public:
    CCar();                                                     // I.1
    CCar(const string& strModel, int nPower,                    // I.2
         const string& strPlate, const string& strOwner);
    CCar(const CCar& car);                                      // I.3

    // I.4 Акцесор на номер на автомобила
    const string& getPlate() const;

    // Допълнителни акцесори - нужни при извеждането и търсенето
    const string& getModel() const;
    const string& getOwner() const;
    int getPower() const;

    bool operator==(const CCar& car) const;    // I.5 по СОБСТВЕНИК
    bool operator< (const CCar& car) const;    // I.6 по РЕГ. НОМЕР

    friend ostream& operator<<(ostream& toStream, const CCar& car);   // I.7
    friend istream& operator>>(istream& fromStream, CCar& car);       // I.8

    // ---------- Статични променливи ----------
    static string                   m_strCityCode;    // I.9.1 текущ код на град
    static map<string, string>      m_mapCodeToCity;  // I.9.2 код -> град
    static multimap<string, string> m_mmapCityToCode; // I.9.3 град -> код(ове)

    // ---------- Статични функции ----------
    static string getCityCode(const string& strPlate);   // I.10
    static bool   isSameCity(const CCar& car);           // I.11
    static string getCodeByCity(const string& strCity);  // I.12
    static string getCityName(const string& strCode);    // помощна: код -> град
};

// ---------- I.1-I.3 Конструктори ----------
CCar::CCar()
    : m_nPower(0), m_strModel("(няма)"), m_strPlate(""), m_strOwner("(няма)") {}

CCar::CCar(const string& strModel, int nPower,
           const string& strPlate, const string& strOwner)
    : m_nPower(nPower), m_strModel(strModel),
      m_strPlate(strPlate), m_strOwner(strOwner) {}

CCar::CCar(const CCar& car)
    : m_nPower(car.m_nPower), m_strModel(car.m_strModel),
      m_strPlate(car.m_strPlate), m_strOwner(car.m_strOwner) {}

// ---------- I.4 и останалите акцесори ----------
const string& CCar::getPlate() const { return m_strPlate; }
const string& CCar::getModel() const { return m_strModel; }
const string& CCar::getOwner() const { return m_strOwner; }
int CCar::getPower() const           { return m_nPower; }

// ---------- I.5 Еквивалентност по СОБСТВЕНИК ----------
bool CCar::operator==(const CCar& car) const
{
    return m_strOwner == car.m_strOwner;
}

// ---------- I.6 По-малък по РЕГ. НОМЕР ----------
bool CCar::operator<(const CCar& car) const
{
    return m_strPlate < car.m_strPlate;
}

// ---------- I.7 Извеждане ----------
ostream& operator<<(ostream& toStream, const CCar& car)
{
    toStream << padRight(car.m_strModel, 9)
             << setw(5) << right << car.m_nPower << " к.с.  "
             << padRight(car.m_strPlate, 10)
             << padRight(car.m_strOwner, 8)
             << " (" << CCar::getCityName(CCar::getCityCode(car.m_strPlate)) << ")";
    return toStream;
}

// ---------- I.8 Четене ----------
// Формат: <марка> <мощност> <рег.номер> <собственик>
istream& operator>>(istream& fromStream, CCar& car)
{
    string strModel, strPlate, strOwner;
    int nPower = 0;
    if (fromStream >> strModel >> nPower >> strPlate >> strOwner)
    {
        car.m_strModel = strModel;
        car.m_nPower   = nPower;
        car.m_strPlate = strPlate;
        car.m_strOwner = strOwner;
    }
    return fromStream;
}

// ============================================================
//  Дефиниране на статичните променливи (ЗАДЪЛЖИТЕЛНО, извън класа)
// ============================================================

// I.9.1 Текущият град по подразбиране
string CCar::m_strCityCode = "B";           // Варна

// I.9.2 Код на град -> име на град
// Източник: bg.wikipedia.org/wiki/Регистрационен_номер_на_МПС_(България)
map<string, string> CCar::m_mapCodeToCity = {
    { "A",  "Бургас"          },
    { "B",  "Варна"           },
    { "BT", "Велико Търново"  },
    { "C",  "София"           },
    { "CA", "София"           },
    { "CB", "София"           },
    { "CC", "Сливен"          },
    { "E",  "Благоевград"     },
    { "EB", "Габрово"         },
    { "K",  "Кюстендил"       },
    { "M",  "Монтана"         },
    { "H",  "Шумен"           },
    { "P",  "Русе"            },
    { "PA", "Пазарджик"       },
    { "PB", "Пловдив"         },
    { "PK", "Перник"          },
    { "T",  "Хасково"         },
    { "TX", "Търговище"       },
    { "X",  "Плевен"          },
    { "Y",  "Добрич"          }
};

// I.9.3 Град -> код(ове). МУЛТИкарта, защото София има три кода.
// Строи се от картата по-горе, за да няма два независими списъка.
static multimap<string, string> buildCityToCode()
{
    multimap<string, string> mmap;
    for (map<string, string>::const_iterator it = CCar::m_mapCodeToCity.begin();
         it != CCar::m_mapCodeToCity.end(); ++it)
        mmap.insert(make_pair(it->second, it->first));   // разменени местата
    return mmap;
}

multimap<string, string> CCar::m_mmapCityToCode = buildCityToCode();

// ---------- I.10 Код на града по регистрационен номер ----------
// Кодът са водещите БУКВИ преди първата цифра: CA1234AB -> "CA".
string CCar::getCityCode(const string& strPlate)
{
    string strCode;
    for (size_t i = 0; i < strPlate.length(); i++)
    {
        if (isdigit(static_cast<unsigned char>(strPlate[i])))
            break;                                   // започнаха цифрите
        strCode += strPlate[i];
    }
    return strCode;
}

// ---------- помощна: име на града по код ----------
string CCar::getCityName(const string& strCode)
{
    map<string, string>::const_iterator it = m_mapCodeToCity.find(strCode);
    if (it == m_mapCodeToCity.end())
        return "непознат код";
    return it->second;
}

// ---------- I.11 Еквивалентност по код на град със статичната I.9.1 ----------
bool CCar::isSameCity(const CCar& car)
{
    return getCityCode(car.m_strPlate) == m_strCityCode;
}

// ---------- I.12 Код на града по име на града (от мултикартата) ----------
// При няколко кода връща първия. Всички се вземат с equal_range.
string CCar::getCodeByCity(const string& strCity)
{
    multimap<string, string>::const_iterator it = m_mmapCityToCode.find(strCity);
    if (it == m_mmapCityToCode.end())
        return "";
    return it->second;
}

// ============================================================
//  II. Клас CRegister
// ============================================================
class CRegister
{
private:
    list<CCar> m_lstCars;

public:
    CRegister();
    explicit CRegister(const string& strFileName);      // II.1

    const list<CCar>& getCars() const;
    size_t size() const;

    string     findPlateByOwner(const string& strOwner) const;   // II.2
    list<CCar> getCarsByCode(const string& strCode) const;       // II.3

    friend ostream& operator<<(ostream& toStream, const CRegister& reg);   // II.4
    friend istream& operator>>(istream& fromStream, CRegister& reg);       // II.5
};

CRegister::CRegister() : m_lstCars() {}

// ---------- II.1 Конструктор по име на файл ----------
CRegister::CRegister(const string& strFileName) : m_lstCars()
{
    ifstream iFile(strFileName.c_str());
    if (!iFile)
        throw runtime_error("Не мога да отворя файла: " + strFileName);

    iFile >> *this;

    if (m_lstCars.empty())
        throw runtime_error("Файлът " + strFileName + " не съдържа автомобили");
}

const list<CCar>& CRegister::getCars() const { return m_lstCars; }
size_t CRegister::size() const               { return m_lstCars.size(); }

// ---------- II.5 Четене ----------
istream& operator>>(istream& fromStream, CRegister& reg)
{
    reg.m_lstCars.clear();
    CCar car;
    while (fromStream >> car)
        reg.m_lstCars.push_back(car);
    return fromStream;
}

// ---------- II.4 Извеждане ----------
ostream& operator<<(ostream& toStream, const CRegister& reg)
{
    toStream << "Регистър (" << reg.m_lstCars.size() << " автомобила)" << endl;
    for (list<CCar>::const_iterator it = reg.m_lstCars.begin();
         it != reg.m_lstCars.end(); ++it)
        toStream << "    " << *it << endl;
    return toStream;
}

// ---------- II.2 Рег. номер по собственик ----------
// operator== сравнява по собственик, затова find работи направо.
string CRegister::findPlateByOwner(const string& strOwner) const
{
    CCar target("", 0, "", strOwner);        // само собственикът има значение
    list<CCar>::const_iterator it = find(m_lstCars.begin(), m_lstCars.end(), target);

    if (it == m_lstCars.end())
        return "";
    return it->getPlate();
}

// ---------- II.3 Списък от автомобили по код на град ----------
list<CCar> CRegister::getCarsByCode(const string& strCode) const
{
    list<CCar> lstResult;
    for (list<CCar>::const_iterator it = m_lstCars.begin();
         it != m_lstCars.end(); ++it)
        if (CCar::getCityCode(it->getPlate()) == strCode)
            lstResult.push_back(*it);
    return lstResult;
}

// ============================================================
//  Помощна: извеждане на списък автомобили
// ============================================================
static void printCars(const string& strTitle, const list<CCar>& lstCars)
{
    cout << "  " << strTitle << " (" << lstCars.size() << ")" << endl;
    if (lstCars.empty())
        cout << "      (празен списък)" << endl;
    for (list<CCar>::const_iterator it = lstCars.begin(); it != lstCars.end(); ++it)
        cout << "      " << *it << endl;
}

// ============================================================
//  III. Главна функция
// ============================================================
int main()
{
    // ===== III.1 Създаване на регистър по файл =====
    cout << "===== III.1 Създаване по файл =====" << endl;

    try
    {
        CRegister regBad("няма-такъв-файл.txt");
        cout << regBad;
    }
    catch (const exception& ex)
    {
        cout << "  хванато изключение: " << ex.what() << endl;
    }

    CRegister reg;
    try
    {
        reg = CRegister("Tema12.txt");
        cout << "  прочетени " << reg.size() << " автомобила" << endl;
    }
    catch (const exception& ex)
    {
        cout << "  хванато изключение: " << ex.what() << endl;
        getchar();
        return 1;
    }

    // ===== III.2 Извеждане с II.4 =====
    cout << endl << "===== III.2 Извеждане (operator<<) =====" << endl;
    cout << reg;

    // ===== I.10 Кодът на града от регистрационния номер =====
    cout << endl << "===== I.10 getCityCode =====" << endl;
    const char* arrPlates[] = { "CA1234AB", "B1235AB", "TX1234AB",
                                "PB5555AA", "ZZ0000ZZ" };
    for (size_t i = 0; i < sizeof(arrPlates) / sizeof(arrPlates[0]); i++)
    {
        string strCode = CCar::getCityCode(arrPlates[i]);
        cout << "    " << padRight(arrPlates[i], 10) << "-> код \""
             << strCode << "\" -> " << CCar::getCityName(strCode) << endl;
    }

    // ===== III.5 / I.12 Кодът на град Варна =====
    cout << endl << "===== III.5 Код на град Варна (I.12) =====" << endl;
    string strVarnaCode = CCar::getCodeByCity("Варна");
    cout << "  getCodeByCity(\"Варна\") = \"" << strVarnaCode << "\"" << endl;

    // Мултикартата може да даде НЯКОЛКО кода за един град
    cout << "--- Всички кодове на София (equal_range) ---" << endl;
    pair<multimap<string, string>::const_iterator,
         multimap<string, string>::const_iterator> range =
        CCar::m_mmapCityToCode.equal_range("София");

    cout << "    София ->";
    for (multimap<string, string>::const_iterator it = range.first;
         it != range.second; ++it)
        cout << " \"" << it->second << "\"";
    cout << "   (" << CCar::m_mmapCityToCode.count("София") << " кода)" << endl;

    cout << "    Варна ->";
    range = CCar::m_mmapCityToCode.equal_range("Варна");
    for (multimap<string, string>::const_iterator it = range.first;
         it != range.second; ++it)
        cout << " \"" << it->second << "\"";
    cout << "   (" << CCar::m_mmapCityToCode.count("Варна") << " код)" << endl;

    // ===== III.3 Списък по код на град =====
    cout << endl << "===== III.3 Списък по код на град (II.3) =====" << endl;
    printCars("код \"CA\" (София):", reg.getCarsByCode("CA"));
    printCars("код \"TX\" (Търговище):", reg.getCarsByCode("TX"));
    printCars("код \"Y\" (Добрич):", reg.getCarsByCode("Y"));

    // ===== III.4 Търсене по собственик =====
    cout << endl << "===== III.4 Търсене по собственик (II.2) =====" << endl;
    const char* arrOwners[] = { "own1", "own2", "own11", "own99" };
    for (size_t i = 0; i < sizeof(arrOwners) / sizeof(arrOwners[0]); i++)
    {
        string strPlate = reg.findPlateByOwner(arrOwners[i]);
        cout << "    " << padRight(arrOwners[i], 8) << "-> "
             << (strPlate.empty() ? string("НЕ е намерен") : strPlate) << endl;
    }

    cout << "--- own2 има повече от един автомобил ---" << endl;
    const list<CCar>& lstAll = reg.getCars();
    for (list<CCar>::const_iterator it = lstAll.begin(); it != lstAll.end(); ++it)
        if (it->getOwner() == "own2")
            cout << "      " << *it << endl;
    cout << "  findPlateByOwner връща само ПЪРВИЯ: "
         << reg.findPlateByOwner("own2") << endl;

    // ===== III.6-III.7 Колите във Варна =====
    cout << endl << "===== III.6-III.7 Автомобили във Варна =====" << endl;
    list<CCar> lstVarna = reg.getCarsByCode(strVarnaCode);
    printCars("Варна, код \"" + strVarnaCode + "\":", lstVarna);

    // ===== I.11 Сравнение със статичната променлива =====
    cout << endl << "===== I.11 isSameCity (статична променлива) =====" << endl;
    cout << "  CCar::m_strCityCode = \"" << CCar::m_strCityCode << "\" ("
         << CCar::getCityName(CCar::m_strCityCode) << ")" << endl;
    for (list<CCar>::const_iterator it = lstAll.begin(); it != lstAll.end(); ++it)
        cout << "    " << padRight(it->getPlate(), 10)
             << (CCar::isSameCity(*it) ? "да" : "не") << endl;

    // Статичната променлива е ЕДНА за целия клас - смяната важи веднага навсякъде
    cout << "--- след CCar::m_strCityCode = \"CA\" ---" << endl;
    CCar::m_strCityCode = "CA";
    cout << "  CCar::m_strCityCode = \"" << CCar::m_strCityCode << "\" ("
         << CCar::getCityName(CCar::m_strCityCode) << ")" << endl;
    for (list<CCar>::const_iterator it = lstAll.begin(); it != lstAll.end(); ++it)
        cout << "    " << padRight(it->getPlate(), 10)
             << (CCar::isSameCity(*it) ? "да" : "не") << endl;

    // ===== Картата е СОРТИРАНА по ключ =====
    cout << endl << "===== map е сортирана по ключ =====" << endl;
    cout << "  първите 6 кода по азбучен ред:" << endl;
    int nShown = 0;
    for (map<string, string>::const_iterator it = CCar::m_mapCodeToCity.begin();
         it != CCar::m_mapCodeToCity.end() && nShown < 6; ++it, ++nShown)
        cout << "      " << padRight(it->first, 4) << "-> " << it->second << endl;

    cout << "  find(\"PB\") -> " << CCar::m_mapCodeToCity.find("PB")->second << endl;
    cout << "  count(\"ZZ\") -> " << CCar::m_mapCodeToCity.count("ZZ")
         << "  (няма такъв код)" << endl;

    // Задържа конзолния прозорец отворен при стартиране с F5 (debug)
    getchar();
    return 0;
}
