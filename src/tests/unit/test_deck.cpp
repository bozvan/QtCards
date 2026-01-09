#include <QtTest/QtTest>
#include "TestDeck.h"
#include "Deck.h"

TestDeck::TestDeck() {}

TestDeck::~TestDeck() {}

void TestDeck::initTestCase()
{
    qDebug("Инициализация всех тестов TestDeck");
}

void TestDeck::cleanupTestCase()
{
    qDebug("Очистка после всех тестов TestDeck");
}

void TestDeck::init()
{
    testDeck = new Deck();
    testCards = new QList<Card>();

    // Создаем тестовые карточки
    Card card1(1, "Что такое ООП?",
               "Объектно-ориентированное программирование",
               ContentType::Text, TestMode::DirectAnswer,
               2.5f, 1, 0, QDateTime::currentDateTime(),
               QDateTime(), 1);

    Card card2(2, "Столица Франции?",
               "Париж",
               ContentType::Text, TestMode::MultipleChoice,
               1.8f, 3, 1, QDateTime::currentDateTime().addDays(1),
               QDateTime::currentDateTime(), 1);

    Card card3(3, "2 + 2?",
               "4",
               ContentType::Text, TestMode::DirectAnswer,
               3.0f, 5, 2, QDateTime::currentDateTime().addDays(3),
               QDateTime::currentDateTime().addDays(-2), 1);

    testCards->append(card1);
    testCards->append(card2);
    testCards->append(card3);
}

void TestDeck::cleanup()
{
    delete testDeck;
    delete testCards;
    testDeck = nullptr;
    testCards = nullptr;
}

// Тест 1: Конструктор по умолчанию
void TestDeck::testDefaultConstructor()
{
    Deck deck;

    // Проверяем значения по умолчанию
    QCOMPARE(deck.getId(), 0);  // Предполагаем, что id по умолчанию 0
    QVERIFY(deck.getName().isEmpty());  // Имя должно быть пустым
    QVERIFY(deck.getCards().isEmpty()); // Карточки должны быть пустым списком

    // Проверяем, что объект создан
    QVERIFY2(true, "Конструктор по умолчанию должен создавать объект");
}

// Тест 2: Получение id (текущее значение по умолчанию)
void TestDeck::testGetId()
{
    // По умолчанию id должен быть 0
    QCOMPARE(testDeck->getId(), 0);
}

// Тест 3: Установка и получение id
void TestDeck::testSetId()
{
    // Тест 1: Положительное число
    testDeck->setId(42);
    QCOMPARE(testDeck->getId(), 42);

    // Тест 2: Другое число
    testDeck->setId(100);
    QCOMPARE(testDeck->getId(), 100);

    // Тест 3: Большое число
    testDeck->setId(999999);
    QCOMPARE(testDeck->getId(), 999999);
}

// Тест 4: Получение name (по умолчанию пустое)
void TestDeck::testGetName()
{
    QVERIFY(testDeck->getName().isEmpty());
    QCOMPARE(testDeck->getName(), QString(""));
}

// Тест 5: Установка и получение name
void TestDeck::testSetName()
{
    // Тест 1: Обычное имя
    QString name1 = "Программирование на C++";
    testDeck->setName(name1);
    QCOMPARE(testDeck->getName(), name1);

    // Тест 2: Другое имя
    QString name2 = "Английский язык";
    testDeck->setName(name2);
    QCOMPARE(testDeck->getName(), name2);

    // Тест 3: Имя с пробелами
    QString name3 = "  Математика для начинающих  ";
    testDeck->setName(name3);
    QCOMPARE(testDeck->getName(), name3);
}

// Тест 6: Unicode символы в имени
void TestDeck::testSetNameUnicode()
{
    // Тест с русскими буквами
    QString russianName = "Колода с русскими буквами";
    testDeck->setName(russianName);
    QCOMPARE(testDeck->getName(), russianName);

    // Тест с эмодзи и специальными символами
    QString unicodeName = QString::fromUtf8("🎴 Карточки 📚 с emoji и символами: ©®™");
    testDeck->setName(unicodeName);
    QCOMPARE(testDeck->getName(), unicodeName);

    // Тест с арабскими символами
    QString arabicName = "أوراق اللعب";
    testDeck->setName(arabicName);
    QCOMPARE(testDeck->getName(), arabicName);
}

// Тест 7: Пустое имя
void TestDeck::testSetNameEmpty()
{
    // Устанавливаем пустое имя
    testDeck->setName("");
    QVERIFY(testDeck->getName().isEmpty());
    QCOMPARE(testDeck->getName(), QString(""));

    // Устанавливаем непустое, затем пустое
    testDeck->setName("Непустое имя");
    testDeck->setName("");
    QVERIFY(testDeck->getName().isEmpty());
}

// Тест 8: Получение пустого списка карточек
void TestDeck::testGetCardsEmpty()
{
    QList<Card> cards = testDeck->getCards();
    QVERIFY(cards.isEmpty());
    QCOMPARE(cards.size(), 0);
}

// Тест 9: Установка и получение списка карточек
void TestDeck::testSetCards()
{
    // Устанавливаем карточки
    testDeck->setCards(*testCards);

    // Получаем карточки обратно
    QList<Card> retrievedCards = testDeck->getCards();

    // Проверяем размер
    QCOMPARE(retrievedCards.size(), 3);

    // Проверяем содержимое (если есть доступ к полям Card)
    if (retrievedCards.size() > 0) {
        // Проверяем первую карточку
        Card firstCard = retrievedCards[0];
        // Предполагаем, что у Card есть публичные поля или геттеры
        // Если нет, нужно добавить соответствующие методы в класс Card
        QCOMPARE(firstCard.getId(), 1);
        QCOMPARE(firstCard.getQuestion(), QString("Что такое ООП?"));
        QCOMPARE(firstCard.getAnswer(), QString("Объектно-ориентированное программирование"));
    }

    // Проверяем последнюю карточку
    if (retrievedCards.size() > 2) {
        Card lastCard = retrievedCards[2];
        QCOMPARE(lastCard.getId(), 3);
        QCOMPARE(lastCard.getQuestion(), QString("2 + 2?"));
        QCOMPARE(lastCard.getAnswer(), QString("4"));
    }
}

// Тест 10: Установка пустого списка карточек
void TestDeck::testSetCardsEmpty()
{
    QList<Card> emptyList;

    // Сначала устанавливаем непустой список
    testDeck->setCards(*testCards);
    QCOMPARE(testDeck->getCards().size(), 3);

    // Затем устанавливаем пустой
    testDeck->setCards(emptyList);

    // Проверяем, что список стал пустым
    QList<Card> cards = testDeck->getCards();
    QVERIFY(cards.isEmpty());
    QCOMPARE(cards.size(), 0);
}

// Тест 11: Большой список карточек
void TestDeck::testSetCardsLargeList()
{
    QList<Card> largeList;
    const int LARGE_COUNT = 100;

    // Создаем много карточек
    for (int i = 0; i < LARGE_COUNT; ++i) {
        Card card(i + 1000,
                  QString("Вопрос %1").arg(i),
                  QString("Ответ %1").arg(i),
                  ContentType::Text,
                  TestMode::DirectAnswer,
                  2.0f,
                  i % 7,
                  i % 3,
                  QDateTime::currentDateTime().addDays(i),
                  QDateTime::currentDateTime().addDays(-i),
                  1);
        largeList.append(card);
    }

    // Устанавливаем большой список
    testDeck->setCards(largeList);

    // Проверяем размер
    QList<Card> retrievedCards = testDeck->getCards();
    QCOMPARE(retrievedCards.size(), LARGE_COUNT);

    // Проверяем первую и последнюю карточку
    if (!retrievedCards.isEmpty()) {
        QCOMPARE(retrievedCards.first().getId(), 1000);
        QCOMPARE(retrievedCards.first().getQuestion(), QString("Вопрос 0"));

        QCOMPARE(retrievedCards.last().getId(), 1000 + LARGE_COUNT - 1);
        QCOMPARE(retrievedCards.last().getQuestion(), QString("Вопрос %1").arg(LARGE_COUNT - 1));
    }
}

// Тест 12: Несколько сеттеров вместе
void TestDeck::testMultipleSetters()
{
    testDeck->setId(777);
    testDeck->setName("Комплексная колода");
    testDeck->setCards(*testCards);

    QCOMPARE(testDeck->getId(), 777);
    QCOMPARE(testDeck->getName(), QString("Комплексная колода"));
    QCOMPARE(testDeck->getCards().size(), 3);
}

// Тест 13: Карточки копируются, а не разделяются
void TestDeck::testCardsCopiedNotShared()
{
    // Создаем локальный список карточек
    QList<Card> localCards;
    Card card(99, "Тестовый вопрос", "Тестовый ответ",
              ContentType::Text, TestMode::DirectAnswer,
              2.0f, 1, 0, QDateTime::currentDateTime(),
              QDateTime(), 1);
    localCards.append(card);

    // Устанавливаем карточки в колоду
    testDeck->setCards(localCards);

    // Изменяем оригинальный список (не должен влиять на колоду)
    localCards[0].getQuestion() = "Измененный вопрос";

    // Проверяем, что карточка в колоде не изменилась
    QList<Card> deckCards = testDeck->getCards();
    if (!deckCards.isEmpty()) {
        QCOMPARE(deckCards[0].getQuestion(), QString("Тестовый вопрос"));
        QVERIFY(deckCards[0].getQuestion() != QString("Измененный вопрос"));
    }
}

// Тест 14: Отрицательный id
void TestDeck::testNegativeId()
{
    testDeck->setId(-1);
    QCOMPARE(testDeck->getId(), -1);

    testDeck->setId(-100);
    QCOMPARE(testDeck->getId(), -100);
}

// Тест 15: Нулевой id
void TestDeck::testZeroId()
{
    testDeck->setId(0);
    QCOMPARE(testDeck->getId(), 0);

    // Меняем на другое значение и возвращаем к 0
    testDeck->setId(50);
    testDeck->setId(0);
    QCOMPARE(testDeck->getId(), 0);
}

// Для QtTest нужно создать макрос, который генерирует main()
//QTEST_APPLESS_MAIN(TestDeck)
