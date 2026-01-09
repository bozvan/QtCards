#pragma once
#include "Deck.h"
#include "Card.h"

class TestDeck : public QObject
{
    Q_OBJECT

public:
    TestDeck();
    ~TestDeck();

private slots:
    // Инициализация и очистка
    void initTestCase();     // Выполняется один раз перед всеми тестами
    void cleanupTestCase();  // Выполняется один раз после всех тестов
    void init();             // Выполняется перед каждым тестом
    void cleanup();          // Выполняется после каждого теста

    // Тесты конструкторов и базовой функциональности
    void testDefaultConstructor();

    // Тесты для id
    void testGetId();
    void testSetId();

    // Тесты для name
    void testGetName();
    void testSetName();
    void testSetNameUnicode();
    void testSetNameEmpty();

    // Тесты для cards
    void testGetCardsEmpty();
    void testSetCards();
    void testSetCardsEmpty();
    void testSetCardsLargeList();

    // Интеграционные тесты
    void testMultipleSetters();
    void testCardsCopiedNotShared();

    // Тесты на граничные случаи
    void testNegativeId();
    void testZeroId();

private:
    Deck* testDeck = nullptr;
    QList<Card>* testCards = nullptr;
};
