#include <gtest/gtest.h>
#include "../date/back/Date.h"

// 1. Проверка високосного года (400 правило)
TEST(DateFunctionalTest, LeapYear400Rule) {
    Date d(1, 1, 2000);
    EXPECT_TRUE(d.isLeap());
}

// 2. 1900 не високосный
TEST(DateFunctionalTest, LeapYearCenturyRule) {
    Date d(1, 1, 1900);
    EXPECT_FALSE(d.isLeap());
}

// 3. Следующий день после 28 февраля в високосный год
TEST(DateFunctionalTest, LeapYearFebruaryTransition) {
    Date d(28, 2, 2024);
    Date next = d.nextDay();
    EXPECT_EQ(next.toString(), "29.02.2024");
}

// 4. Следующий день после 28 февраля в обычный год
TEST(DateFunctionalTest, NonLeapFebruaryTransition) {
    Date d(28, 2, 2023);
    Date next = d.nextDay();
    EXPECT_EQ(next.toString(), "01.03.2023");
}

// 5. Переход 29 февраля -> 1 марта
TEST(DateFunctionalTest, LeapDayToMarch) {
    Date d(29, 2, 2024);
    Date next = d.nextDay();
    EXPECT_EQ(next.toString(), "01.03.2024");
}

// 6. previousDay после nextDay возвращает исходную дату
TEST(DateFunctionalTest, NextThenPreviousSameDate) {
    Date d(15, 7, 2023);
    Date next = d.nextDay();
    Date prev = next.previousDay();
    EXPECT_EQ(prev.toString(), d.toString());
}

// 7. Разница между датами через месяц
TEST(DateFunctionalTest, DurationAcrossMonth) {
    Date d1(25, 1, 2023);
    Date d2(5, 2, 2023);
    EXPECT_EQ(d1.duration(d2), 11);
}

// 8. Разница между датами через год
TEST(DateFunctionalTest, DurationAcrossYear) {
    Date d1(31, 12, 2022);
    Date d2(1, 1, 2023);
    EXPECT_EQ(d1.duration(d2), 1);
}

// 9. Разница между датами через високосный год
TEST(DateFunctionalTest, DurationLeapYear) {
    Date d1(1, 1, 2023);
    Date d2(1, 1, 2024);
    EXPECT_EQ(d1.duration(d2), 365);
}

// 10. Проверка номера недели
TEST(DateFunctionalTest, WeekNumberBasic) {
    Date d(15, 1, 2023);
    EXPECT_EQ(d.weekNumber(), 3);
}

// 11. День рождения завтра
TEST(DateFunctionalTest, BirthdayTomorrow) {
    Date today(9, 6, 2023);
    Date birthday(10, 6, 2000);
    EXPECT_EQ(today.daysTillYourBirthday(birthday), 1);
}

// 12. День рождения сегодня
TEST(DateFunctionalTest, BirthdayToday) {
    Date today(10, 6, 2023);
    Date birthday(10, 6, 1990);
    EXPECT_EQ(today.daysTillYourBirthday(birthday), 0);
}

// 13. День рождения уже прошел в этом году
TEST(DateFunctionalTest, BirthdayNextYear) {
    Date today(20, 6, 2023);
    Date birthday(10, 6, 2000);
    EXPECT_GT(today.daysTillYourBirthday(birthday), 300);
}

// 14. День рождения 29 февраля в невисокосный год
TEST(DateFunctionalTest, LeapBirthdayHandling) {
    Date today(27, 2, 2023);
    Date birthday(29, 2, 2000);
    EXPECT_EQ(today.daysTillYourBirthday(birthday), 1);
}

// 15. Цепочка из нескольких nextDay
TEST(DateFunctionalTest, MultipleNextDayChain) {
    Date d(28, 12, 2023);

    d = d.nextDay();
    d = d.nextDay();
    d = d.nextDay();
    d = d.nextDay();

    EXPECT_EQ(d.toString(), "01.01.2024");
}