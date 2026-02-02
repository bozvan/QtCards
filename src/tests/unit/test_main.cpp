#include <QtTest>
#include "TestDeck.h"
#include "TestCard.h"
#include "TestScheduler.h"

// Объявляем все тестовые классы
class TestCard;
class TestDeck;
class TestScheduler;

// Регистрируем все тесты
int main(int argc, char *argv[]) {
    int status = 0;

    {
        TestCard tc;
        status |= QTest::qExec(&tc, argc, argv);
    }

    // {
    //     TestDeck td;
    //     status |= QTest::qExec(&td, argc, argv);
    // }

    // {
    //     TestScheduler ts;
    //     status |= QTest::qExec(&ts, argc, argv);
    // }
    return status;
}
