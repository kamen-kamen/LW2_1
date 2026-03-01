#include "date.h"
#include <QDate>
#include <stdexcept>
#include <cmath>

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
    static const int days[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (m == 2 && ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0))) {
        return 29;
    }
    return days[m];
}

void Date::setDate(int d, int m, int y) {
    if (y < 1 || m < 1 || m > 12 || d < 1 || d > daysInMonth(m, y)) {
        throw std::invalid_argument("Invalid date parameters");
    }
    day = d;
    month = m;
    year = y;
}

long long Date::toTotalDays() const {
    long long y = year - 1;
    long long total = y * 365 + y / 4 - y / 100 + y / 400;

    static const int daysInMonths[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    for (int i = 1; i < month; ++i) {
        total += daysInMonths[i];
    }

    if (month > 2 && isLeap()) {
        total++;
    }

    total += day;
    return total;
}

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

short Date::weekNumber() const {
    int dayOfYear = 0;
    for (int i = 1; i < month; i++) {
        dayOfYear += daysInMonth(i, year);
    }
    dayOfYear += day;
    return static_cast<short>((dayOfYear / 7) + 1);
}

int Date::duration(Date otherDate) const {
    return std::abs(static_cast<int>(this->toTotalDays() - otherDate.toTotalDays()));
}

int Date::daysTillYourBirthday(Date birthdayDate) const {
    int bDay = birthdayDate.day;
    int bMonth = birthdayDate.month;

    if (bDay == 29 && bMonth == 2 && !isLeap()) {
        bDay = 28;
    }

    Date bdayThisYear(bDay, bMonth, year);
    long long currentTotal = toTotalDays();
    long long bdayTotal = bdayThisYear.toTotalDays();

    if (bdayTotal >= currentTotal) {
        return static_cast<int>(bdayTotal - currentTotal);
    }

    if (bDay == 29 && bMonth == 2 && !Date(1, 1, year + 1).isLeap()) {
        bDay = 28;
    }

    Date bdayNextYear(bDay, bMonth, year + 1);
    return static_cast<int>(bdayNextYear.toTotalDays() - currentTotal);
}

QString Date::toString() const {
    return QString("%1.%2.%3")
            .arg(day, 2, 10, QChar('0'))
            .arg(month, 2, 10, QChar('0'))
            .arg(year, 4, 10, QChar('0'));
}
