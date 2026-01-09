#include <QtTest>
#include "TestDeck.h"
#include "TestCard.h"

// Объявляем все тестовые классы
class TestCard;
class TestDeck;

// Регистрируем все тесты
int main(int argc, char *argv[]) {
    int status = 0;

    {
        TestCard tc;
        status |= QTest::qExec(&tc, argc, argv);
    }

    {
        TestDeck td;
        status |= QTest::qExec(&td, argc, argv);
    }

    return status;
}
