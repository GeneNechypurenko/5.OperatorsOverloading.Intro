#include <iostream>
#include <windows.h>

using namespace std;

// Розв'язок: Модуль 17, Вступ до перевантаження операторів, практика 2 — клас Stats (характеристики RPG-персонажа)

class Stats
{
private:
    int strength;       // сила
    int agility;        // спритність
    int health;         // здоров'я

public:
    Stats(int strength, int agility, int health) : strength(strength), agility(agility), health(health)
    {
    }

    // ===== БІНАРНІ ОПЕРАТОРИ: один параметр — правий операнд =====

    // hero + sword  →  hero.operator+(sword):  герой надягає предмет
    Stats operator+(const Stats& other) const
    {
        return Stats(strength + other.strength, agility + other.agility, health + other.health);
    }

    // equipped - shield  →  equipped.operator-(shield):  герой знімає предмет
    Stats operator-(const Stats& other) const
    {
        return Stats(strength - other.strength, agility - other.agility, health - other.health);
    }

    // sword * 2  →  sword.operator*(2):  параметр — int, а не Stats
    Stats operator*(int multiplier) const
    {
        return Stats(strength * multiplier, agility * multiplier, health * multiplier);
    }

    // ===== УНАРНИЙ ОПЕРАТОР: без параметрів =====

    // -blessing  →  blessing.operator-():  прокляття — усе з протилежним знаком
    Stats operator-() const
    {
        return Stats(-strength, -agility, -health);
    }

    void print() const
    {
        cout << "Сила: " << strength << ", Спритність: " << agility << ", Здоров'я: " << health << endl;
    }
};

int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    Stats hero(10, 5, 100);         // герой
    Stats sword(5, 0, 0);           // меч: +5 сили
    Stats shield(0, -1, 20);        // щит: +20 здоров'я, але -1 спритності (важкий)
    Stats blessing(2, 2, 10);       // благословення

    cout << "1. Повне спорядження" << endl << "  ";
    Stats equipped = hero + sword + shield;     // (hero + sword) + shield — зліва направо
    equipped.print();                           // 15, 4, 120

    cout << "2. Зняв щит" << endl << "  ";
    Stats withoutShield = equipped - shield;
    withoutShield.print();                      // 15, 5, 100

    cout << "3. Прокляття замість благословення" << endl << "  ";
    Stats cursed = hero + -blessing;            // спершу унарний -, потім +
    cursed.print();                             // 8, 3, 90

    cout << "4. Зілля подвоїло меч" << endl << "  ";
    Stats boosted = hero + sword * 2;           // hero + (sword * 2): * виконується раніше за +
    boosted.print();                            // 20, 5, 100

    cout << "5. Оригінали не змінилися" << endl << "  ";
    hero.print();                               // 10, 5, 100 — оператори const, повертають НОВИЙ об'єкт

    return 0;
}

/*
  ПРИМІТКА ДЛЯ РОЗБОРУ
  - Типова помилка: забути одне з полів в операторі (наприклад, health). Програма компілюється,
    але здоров'я «губиться». Тому в завданні кожна перевірка має очікуваний результат.
  - hero * 2 скомпілюється, а 2 * hero — НІ: для оператора-методу лівий операнд завжди об'єкт класу (*this).
    Як дозволити  2 * hero  — тема перевантаження глобальними та дружніми функціями.
  - hero + -blessing працює, бо унарний мінус має вищий пріоритет за бінарний +.
*/
