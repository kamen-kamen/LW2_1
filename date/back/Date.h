#ifndef DATE_H
#define DATE_H

#include <QString>
#include <cmath>
#include <stdexcept>

class Date {
private:
    int day;
    int month;
    int year;

    // Вспомогательный метод для подсчета дней от 01.01.0001
    long long toTotalDays() const;
    int daysInMonth(int m, int y) const;

public:
    Date(); // Конструктор по умолчанию (текущая дата)
    Date(int d, int m, int y);

    // Сеттеры для изменения даты (нужны для редактирования)
    void setDate(int d, int m, int y);

    // --- Методы по заданию ---
    Date nextDay() const;
    Date previousDay() const;
    bool isLeap() const;
    short weekNumber() const;
    int daysTillYourBirthday(Date birthdayDate) const;
    int duration(Date otherDate) const;

    // Вывод
    QString toString() const;

    // Геттеры
    int getDay() const { return day; }
    int getMonth() const { return month; }
    int getYear() const { return year; }
};

#endif // DATE_H