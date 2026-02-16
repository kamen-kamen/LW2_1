#include "date.h"
#include <QDate> // Используем только для получения системного времени в конструкторе

Date::Date() {
    QDate current = QDate::currentDate();
    day = current.day();
    month = current.month();
    year = current.year();
}

Date::Date(int d, int m, int y) {
    setDate(d, m, y);
}

bool Date::isLeap() const {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int Date::daysInMonth(int m, int y) const {
    int days[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (m == 2 && ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0)))
        return 29;
    return days[m];
}

void Date::setDate(int d, int m, int y) {
    if (y < 1 || m < 1 || m > 12 || d < 1 || d > daysInMonth(m, y)) {
        // Выбрасываем исключение для блока try-catch
        throw std::invalid_argument("Некорректная дата: " + std::to_string(d) + "." + std::to_string(m) + "." + std::to_string(y));
    }
    day = d; month = m; year = y;
}

// Перевод даты в количество дней от начала эры (для удобного вычитания)
long long Date::toTotalDays() const {
    long long total = year * 365 + year / 4 - year / 100 + year / 400;
    int daysInMonths[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    for (int i = 1; i < month; ++i) {
        total += daysInMonths[i];
    }
    if (month > 2 && isLeap()) total++;
    total += day;
    return total;
}

// 1. NextDay
Date Date::nextDay() const {
    int d = day + 1;
    int m = month;
    int y = year;
    if (d > daysInMonth(m, y)) {
        d = 1;
        m++;
        if (m > 12) {
            m = 1;
            y++;
        }
    }
    return Date(d, m, y);
}

// 2. PreviousDay
Date Date::previousDay() const {
    int d = day - 1;
    int m = month;
    int y = year;
    if (d < 1) {
        m--;
        if (m < 1) {
            m = 12;
            y--;
        }
        d = daysInMonth(m, y);
    }
    return Date(d, m, y);
}

// 3. WeekNumber (Упрощенный расчет)
short Date::weekNumber() const {
    int dayOfYear = 0;
    for (int i = 1; i < month; i++) dayOfYear += daysInMonth(i, year);
    dayOfYear += day;
    return (dayOfYear / 7) + 1;
}

// 4. Duration (разница модулей дней)
int Date::duration(Date otherDate) const {
    long long t1 = this->toTotalDays();
    long long t2 = otherDate.toTotalDays();
    return std::abs(t1 - t2);
}

// 5. DaysTillYourBirthday
int Date::daysTillYourBirthday(Date birthdayDate) const {
    // Создаем дату дня рождения в ТЕКУЩЕМ году
    Date bdayThisYear(birthdayDate.getDay(), birthdayDate.getMonth(), this->year);

    long long currentTotal = this->toTotalDays();
    long long bdayTotal = bdayThisYear.toTotalDays();

    if (bdayTotal >= currentTotal) {
        return bdayTotal - currentTotal;
    } else {
        // Если день рождения уже прошел, считаем до следующего года
        Date bdayNextYear(birthdayDate.getDay(), birthdayDate.getMonth(), this->year + 1);
        return bdayNextYear.toTotalDays() - currentTotal;
    }
}

QString Date::toString() const {
    return QString("%1.%2.%3")
            .arg(day, 2, 10, QChar('0'))
            .arg(month, 2, 10, QChar('0'))
            .arg(year, 4, 10, QChar('0'));
}