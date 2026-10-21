#include <iostream>
#include <windows.h>

using namespace std;

/*
==========================================================================================================================================
                                    ДОМАШНЄ ЗАВДАННЯ 25. Перевантаження операторів методами-членами. Клас Date
==========================================================================================================================================
*/

class Date
{
private:
    int day;
    int month;
    int year;

    // Рівень 3: високосний рік — ділиться на 4, але не на 100, або ділиться на 400
    bool isLeap() const
    {
        return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
    }

    int daysInMonth() const
    {
        if (month == 2)
        {
            if (isLeap())           // Рівень 1: просто return 28;
            {
                return 29;
            }
            return 28;
        }
        if (month == 4 || month == 6 || month == 9 || month == 11)
        {
            return 30;
        }
        return 31;
    }

public:
    Date(int day, int month, int year) : day(day), month(month), year(year)
    {
        // Спочатку місяць і рік: від них залежить кількість днів у місяці
        if (this->month < 1 || this->month > 12)
        {
            this->month = 1;
        }
        if (this->year < 1)
        {
            this->year = 1;
        }
        if (this->day < 1 || this->day > daysInMonth())
        {
            this->day = 1;
        }
    }

    // ===== ІНКРЕМЕНТ: наступний день =====
    //  1. Чому префіксний ++ повертає Date&, а постфіксний — Date ? Що буде, якщо постфіксний повертатиме Date& ?
    Date& operator++()      
    {
        day++;
        if (day > daysInMonth())
        {
            day = 1;
            month++;
            if (month > 12)
            {
                month = 1;
                year++;
            }
        }
        return *this;
    }

    //  1. Чому префіксний ++ повертає Date&, а постфіксний — Date ? Що буде, якщо постфіксний повертатиме Date& ?
    Date operator++(int)    //  2. Навіщо параметр int у постфіксному operator++(int)?
    {                       
        Date old = *this;
        ++(*this);
        return old;
    }

    // ===== ДЕКРЕМЕНТ: попередній день =====
    Date& operator--()
    {
        day--;
        if (day < 1)
        {
            month--;
            if (month < 1)
            {
                month = 12;
                year--;
            }
            day = daysInMonth();    // останній день нового (попереднього) місяця
        }
        return *this;
    }

    Date operator--(int)
    {
        Date old = *this;
        --(*this);
        return old;
    }

    // ===== += і + : зсув на кілька днів =====
    Date& operator+=(int days)              //  3. Чому operator+ константний, а operator+= — ні?
    {
        if (days > 0)
        {
            for (int i = 0; i < days; i++)
            {
                ++(*this);
            }
        }
        return *this;
    }

    Date operator+(int days) const          //  3. Чому operator+ константний, а operator+= — ні?
    {
        Date result = *this;
        result += days;
        return result;
    }

    // ===== Рівень 2: -= і - =====
    Date& operator-=(int days)
    {
        if (days > 0)
        {
            for (int i = 0; i < days; i++)
            {
                --(*this);
            }
        }
        return *this;
    }

    //  5. (Рівень 2) У класі два оператори з іменем operator-. Як компілятор розуміє, який викликати в d - 5 і в d - other?
    Date operator-(int days) const
    {
        Date result = *this;
        result -= days;
        return result;
    }

    //  5. (Рівень 2) У класі два оператори з іменем operator-. Як компілятор розуміє, який викликати в d - 5 і в d - other?
    int operator-(const Date& other) const
    {
        if (*this < other)
        {
            return -(other - *this);
        }
        int days = 0;
        Date current = other;
        while (current < *this)
        {
            ++current;
            days++;
        }
        return days;
    }

    // ===== ПОРІВНЯННЯ: == і <, решта через них =====
    bool operator==(const Date& other) const    //  4. Яка користь від того, що !=, >, <=, >= реалізовано через == та <?
    {
        return day == other.day && month == other.month && year == other.year;
    }

    bool operator<(const Date& other) const     //  4. Яка користь від того, що !=, >, <=, >= реалізовано через == та <?
    {
        if (year != other.year)
        {
            return year < other.year;
        }
        if (month != other.month)
        {
            return month < other.month;
        }
        return day < other.day;
    }

    bool operator!=(const Date& other) const    //  4. Яка користь від того, що !=, >, <=, >= реалізовано через == та <?
    {
        return !(*this == other);
    }

    bool operator>(const Date& other) const     //  4. Яка користь від того, що !=, >, <=, >= реалізовано через == та <?
    {
        return other < *this;
    }

    bool operator<=(const Date& other) const    //  4. Яка користь від того, що !=, >, <=, >= реалізовано через == та <?
    {
        return !(other < *this);
    }

    bool operator>=(const Date& other) const    //  4. Яка користь від того, що !=, >, <=, >= реалізовано через == та <?
    {
        return !(*this < other);
    }

    void print() const
    {
        if (day < 10)
        {
            cout << "0";
        }
        cout << day << ".";
        if (month < 10)
        {
            cout << "0";
        }
        cout << month << "." << year;
    }
};

int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    // ---------------- РІВЕНЬ 1 ----------------
    cout << "1. Наступний день після кінця місяця" << endl;
    Date d(28, 2, 2026);
    cout << "  ";
    d.print();
    cout << "  →  ";
    ++d;
    d.print();                              // 01.03.2026
    cout << endl;

    cout << "2. Новий рік: постфіксний ++" << endl;
    Date e(31, 12, 2026);
    Date old = e++;
    cout << "  old = ";
    old.print();                            // 31.12.2026
    cout << ",  e = ";
    e.print();                              // 01.01.2027
    cout << endl;

    cout << "3. Кінець курсу" << endl;
    Date today(17, 10, 2026);
    Date courseEnd = today + 41;
    cout << "  ";
    today.print();
    cout << " + 41 = ";
    courseEnd.print();                      // 27.11.2026
    cout << endl;

    cout << "4. Найраніша з трьох дат" << endl;
    Date first(15, 3, 2027);
    Date second(1, 12, 2026);
    Date third(20, 12, 2026);
    Date earliest = first;
    if (second < earliest)
    {
        earliest = second;
    }
    if (third < earliest)
    {
        earliest = third;
    }
    cout << "  ";
    earliest.print();                       // 01.12.2026
    cout << endl;

    cout << "5. Ланцюжок +=" << endl;
    Date f(25, 12, 2026);
    (f += 5) += 2;
    cout << "  25.12.2026 + 5 + 2 = ";
    f.print();                              // 01.01.2027
    cout << endl;

    cout << "6. Некоректна дата" << endl;
    Date wrong(31, 4, 2026);                // у квітні 30 днів → day = 1
    cout << "  Date(31, 4, 2026) = ";
    wrong.print();
    cout << endl;

    // ---------------- РІВЕНЬ 2 ----------------
    cout << "7. Рівень 2: віднімання днів" << endl;
    Date g(1, 1, 2027);
    Date h = g - 1;
    cout << "  01.01.2027 - 1 = ";
    h.print();                              // 31.12.2026
    cout << endl;
    Date march(1, 3, 2026);
    --march;
    cout << "  --01.03.2026 = ";
    march.print();                          // 28.02.2026
    cout << endl;

    cout << "8. Рівень 2: різниця дат" << endl;
    cout << "  До кінця курсу: " << courseEnd - today << " днів" << endl;                     // 41
    cout << "  До Нового року: " << Date(1, 1, 2027) - today << " днів" << endl;             // 76
    cout << "  today - courseEnd = " << today - courseEnd << endl;                           // -41

    // ---------------- РІВЕНЬ 3 ----------------
    cout << "9. Рівень 3: високосний рік" << endl;
    Date leap(28, 2, 2028);
    ++leap;
    cout << "  наступний день після 28.02.2028: ";
    leap.print();                           // 29.02.2028
    cout << endl;
    Date notLeap(29, 2, 2100);              // 2100 не високосний → day = 1
    cout << "  Date(29, 2, 2100) = ";
    notLeap.print();                        // 01.02.2100
    cout << endl;

    return 0;
}

/*
  1. Чому префіксний ++ повертає Date&, а постфіксний — Date? Що буде, якщо постфіксний повертатиме Date&?
  2. Навіщо параметр int у постфіксному operator++(int)?
  3. Чому operator+ константний, а operator+= — ні?
  4. Яка користь від того, що !=, >, <=, >= реалізовано через == та <?
  5. (Рівень 2) У класі два оператори з іменем operator-. Як компілятор розуміє, який викликати в d - 5 і в d - other?
  6. Чому для дат перевантажують Date + int і Date - Date, але не Date + Date?

  ВІДПОВІДІ НА ЧАСТИНУ «ПОЯСНИ»

  1. 

  2. 

  3. 

  4. 

  5. 

  6. 
*/
