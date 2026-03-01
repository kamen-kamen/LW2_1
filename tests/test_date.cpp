#include <gtest/gtest.h>
#include "../date/back/Date.h"

// 1. Проверка високосного года
TEST(DateTest, LeapYearTrue) {
    Date d(1, 1, 2024);
    EXPECT_TRUE(d.isLeap());
}

// 2. Проверка невисокосного года
TEST(DateTest, LeapYearFalse) {
    Date d(1, 1, 2023);
    EXPECT_FALSE(d.isLeap());
}

// 3. Нельзя создать некорректную дату
TEST(DateTest, InvalidDateThrows) {
    EXPECT_THROW(Date(31, 2, 2023), std::invalid_argument);
}

// 4. Переход на следующий день внутри месяца
TEST(DateTest, NextDaySimple) {
    Date d(10, 5, 2023);
    Date next = d.nextDay();
    EXPECT_EQ(next.toString(), "11.05.2023");
}

// 5. Переход на следующий месяц
TEST(DateTest, NextDayMonthChange) {
    Date d(31, 1, 2023);
    Date next = d.nextDay();
    EXPECT_EQ(next.toString(), "01.02.2023");
}

// 6. Переход на следующий год
TEST(DateTest, NextDayYearChange) {
    Date d(31, 12, 2023);
    Date next = d.nextDay();
    EXPECT_EQ(next.toString(), "01.01.2024");
}

// 7. Предыдущий день внутри месяца
TEST(DateTest, PreviousDaySimple) {
    Date d(15, 6, 2023);
    Date prev = d.previousDay();
    EXPECT_EQ(prev.toString(), "14.06.2023");
}

// 8. Разница между датами
TEST(DateTest, DurationBetweenDates) {
    Date d1(1, 1, 2023);
    Date d2(11, 1, 2023);
    EXPECT_EQ(d1.duration(d2), 10);
}

// 9. День рождения уже впереди в этом году
TEST(DateTest, DaysTillBirthdaySameYear) {
    Date today(1, 6, 2023);
    Date birthday(10, 6, 2000);
    EXPECT_EQ(today.daysTillYourBirthday(birthday), 9);
}

// 10. Формат строки
TEST(DateTest, ToStringFormat) {
    Date d(5, 3, 2023);
    EXPECT_EQ(d.toString(), "05.03.2023");
}