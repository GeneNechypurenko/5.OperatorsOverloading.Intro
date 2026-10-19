#include <iostream>
#include <cstdlib>
#include <windows.h>

using namespace std;

// Розв'язок: Модуль 17, Вступ до перевантаження операторів, практика 1 — operator/ для Fraction

class Fraction
{
private:
    int numerator;          // чисельник
    int denominator;        // знаменник, завжди > 0

    // Скорочення дробу: ділимо чисельник і знаменник на найбільший спільний дільник (НСД).
    // НСД шукаємо алгоритмом Евкліда: замінюємо пару (a, b) на (b, залишок від a / b),
    // поки b не стане 0. Тоді в a залишиться НСД.
    //   Приклад для 6/8:  (6, 8) → (8, 6) → (6, 2) → (2, 0)   →  НСД = 2,  6/8 = 3/4
    //   Приклад для 0/5:  (0, 5) → (5, 0)                     →  НСД = 5,  0/5 = 0/1
    void reduce()
    {   // Функція abs(int) із бібліотеки <cstdlib> обчислює та повертає абсолютне значення (модуль) цілого числа
        int a = abs(numerator);     // abs: знак не впливає на НСД (знак дробу залишається в numerator)
        int b = denominator;        // знаменник завжди > 0 — це гарантує конструктор

        while (b != 0)
        {
            int rest = a % b;       // залишок від ділення a на b
            a = b;                  // крок алгоритму: (a, b) → (b, rest)
            b = rest;
        }
        // тут a — НСД чисельника і знаменника

        if (a > 1)                  // НСД = 1 → дріб уже нескоротний, ділити немає на що
        {
            numerator /= a;
            denominator /= a;
        }
    }

public:
    // Конструктор приводить дріб до «стандартного» вигляду:
    //   1) знаменник ніколи не дорівнює 0;
    //   2) знаменник завжди додатний — знак дробу зберігається лише в чисельнику:  1/-2 → -1/2;
    //   3) дріб скорочений:  6/8 → 3/4.
    // Завдяки цьому всі інші методи можуть покладатися на те, що denominator > 0.
    Fraction(int numerator, int denominator) : numerator(numerator), denominator(denominator)
    {
        if (this->denominator == 0)
        {
            this->denominator = 1;                  // на 0 ділити не можна — некоректний знаменник замінюємо на 1
        }
        else if (this->denominator < 0)
        {
            this->numerator = -this->numerator;     // знак переносимо в чисельник:
            this->denominator = -this->denominator; // 1/-2 → -1/2,   -3/-4 → 3/4
        }
        reduce();                                   // скорочуємо лише ПІСЛЯ виправлення знаменника:
                                                    // reduce розраховує на denominator > 0
    }

    // a / b  →  a.operator/(b):  ділення = множення на «перевернутий» дріб
    // a/b : c/d = (a * d) / (b * c)
    Fraction operator/(const Fraction& other) const
    {
        if (other.numerator == 0)
        {
            cout << "  Ділення на нуль" << endl;
            return *this;           // копія поточного дробу: сам дріб не змінюється
        }
        // Якщо other від'ємний, знаменник вийде від'ємним — конструктор сам перенесе знак у чисельник
        return Fraction(numerator * other.denominator, denominator * other.numerator);
    }

    void print() const
    {
        cout << numerator << "/" << denominator;
    }
};

int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    Fraction a(1, 2);
    Fraction b(3, 4);

    cout << "1. Звичайне ділення" << endl;
    Fraction quotient = a / b;              // a.operator/(b) = 4/6 → скорочується до 2/3
    cout << "  1/2 : 3/4 = ";
    quotient.print();
    cout << endl;

    cout << "2. Ділення на від'ємний дріб" << endl;
    Fraction negative = a / Fraction(-3, 4);    // 4/-6 → конструктор: -4/6 → -2/3
    cout << "  1/2 : -3/4 = ";
    negative.print();
    cout << endl;

    cout << "3. Ділення на нуль" << endl;
    Fraction zero(0, 5);
    Fraction same = a / zero;               // спершу виклик (тут виводиться повідомлення), потім вивід результату
    cout << "  1/2 : 0/1 = ";
    same.print();
    cout << endl;

    return 0;
}

/*
  ПРИМІТКА ДЛЯ РОЗБОРУ
  - Перевіряємо саме other.numerator: дріб дорівнює нулю, коли нулю дорівнює чисельник.
    Знаменник тут перевіряти не потрібно — конструктор гарантує, що він > 0.
  - return *this — повертаємо КОПІЮ поточного дробу (тип результату Fraction, не Fraction&).
    Метод const, тому сам дріб a не змінюється.
  - Повідомлення «Ділення на нуль» виводиться ВСЕРЕДИНІ оператора. Тому спершу ділимо,
    а вже потім виводимо рядок з результатом — інакше повідомлення опиниться посеред рядка.
*/
