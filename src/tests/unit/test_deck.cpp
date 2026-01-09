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
    localCards[0].setQuestion("Измененный вопрос");

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


// ==================== DUE CARDS TESTS ====================

void TestDeck::testGetDueCardsEmptyDeck()
{
    Deck deck;
    deck.setName("Empty Deck");

    QList<Card> dueCards = deck.getDueCards();
    QVERIFY(dueCards.isEmpty());
    QCOMPARE(deck.getDueCount(), 0);
}

void TestDeck::testGetDueCardsAllDue()
{
    Deck deck;
    QList<Card> cards;

    // Создаем карточки, которые уже просрочены
    QDateTime pastDate = QDateTime::currentDateTime().addDays(-1);

    for (int i = 0; i < 5; i++) {
        Card card(i + 1,
                  QString("Question %1").arg(i),
                  QString("Answer %1").arg(i),
                  ContentType::Text,
                  TestMode::DirectAnswer,
                  2.0f,
                  10,
                  3,
                  pastDate.addDays(-i), // Все в прошлом
                  QDateTime::currentDateTime().addDays(-2),
                  1);
        cards.append(card);
    }

    deck.setCards(cards);

    QList<Card> dueCards = deck.getDueCards();
    QCOMPARE(dueCards.size(), 5);
    QCOMPARE(deck.getDueCount(), 5);

    // Проверяем, что все карточки те же
    for (int i = 0; i < 5; i++) {
        QCOMPARE(dueCards[i].getId(), cards[i].getId());
    }
}

void TestDeck::testGetDueCardsSomeDue()
{
    Deck deck;
    QList<Card> cards;
    QDateTime now = QDateTime::currentDateTime();

    // 1. Просроченная (вчера)
    Card card1(1, "Q1", "A1", ContentType::Text, TestMode::DirectAnswer,
               2.0f, 10, 3, now.addDays(-1), now.addDays(-2), 1);

    // 2. На сегодня (прямо сейчас)
    Card card2(2, "Q2", "A2", ContentType::Text, TestMode::DirectAnswer,
               2.0f, 10, 3, now, now.addDays(-1), 1);

    // 3. На завтра (не должна попасть)
    Card card3(3, "Q3", "A3", ContentType::Text, TestMode::DirectAnswer,
               2.0f, 10, 3, now.addDays(1), now, 1);

    // 4. На следующей неделе (не должна попасть)
    Card card4(4, "Q4", "A4", ContentType::Text, TestMode::DirectAnswer,
               2.0f, 10, 3, now.addDays(7), now, 1);

    // 5. С пустой датой (должна попасть)
    Card card5(5, "Q5", "A5", ContentType::Text, TestMode::DirectAnswer,
               2.0f, 10, 3, QDateTime(), now, 1);

    cards.append(card1);
    cards.append(card2);
    cards.append(card3);
    cards.append(card4);
    cards.append(card5);

    deck.setCards(cards);

    QList<Card> dueCards = deck.getDueCards();
    QCOMPARE(dueCards.size(), 3); // card1, card2, card5
    QCOMPARE(deck.getDueCount(), 3);

    // Проверяем ID карточек, которые попали
    QList<int> dueIds;
    for (const Card& card : dueCards) {
        dueIds.append(card.getId());
    }

    QVERIFY(dueIds.contains(1)); // Просроченная
    QVERIFY(dueIds.contains(2)); // На сегодня
    QVERIFY(dueIds.contains(5)); // С пустой датой
    QVERIFY(!dueIds.contains(3)); // На завтра - НЕ должна быть
    QVERIFY(!dueIds.contains(4)); // Через неделю - НЕ должна быть
}

void TestDeck::testGetDueCardsNoneDue()
{
    Deck deck;
    QList<Card> cards;
    QDateTime now = QDateTime::currentDateTime();

    // Все карточки в будущем
    for (int i = 0; i < 3; i++) {
        Card card(i + 1,
                  QString("Future Q%1").arg(i),
                  QString("Future A%1").arg(i),
                  ContentType::Text,
                  TestMode::DirectAnswer,
                  2.0f,
                  10,
                  3,
                  now.addDays(i + 1), // Все в будущем
                  now,
                  1);
        cards.append(card);
    }

    deck.setCards(cards);

    QList<Card> dueCards = deck.getDueCards();
    QVERIFY(dueCards.isEmpty());
    QCOMPARE(deck.getDueCount(), 0);
}

void TestDeck::testGetDueCardsWithInvalidDates()
{
    Deck deck;
    QList<Card> cards;

    // Карточка с невалидной датой nextReview
    Card card1(1, "Q1", "A1", ContentType::Text, TestMode::DirectAnswer,
               2.0f, 10, 3, QDateTime(), QDateTime::currentDateTime(), 1);

    // Карточка с "неправильной" датой (например, изначально неправильно создана)
    QDateTime invalidDate;
    Card card2(2, "Q2", "A2", ContentType::Text, TestMode::DirectAnswer,
               2.0f, 10, 3, invalidDate, invalidDate, 1);

    // Карточка с корректной будущей датой
    Card card3(3, "Q3", "A3", ContentType::Text, TestMode::DirectAnswer,
               2.0f, 10, 3, QDateTime::currentDateTime().addDays(1),
               QDateTime::currentDateTime(), 1);

    cards.append(card1);
    cards.append(card2);
    cards.append(card3);

    deck.setCards(cards);

    QList<Card> dueCards = deck.getDueCards();
    QCOMPARE(dueCards.size(), 2); // card1 и card2 с невалидными датами
    QCOMPARE(deck.getDueCount(), 2);
}

void TestDeck::testGetDueCardsMixedDates()
{
    Deck deck;
    QList<Card> cards;
    QDateTime now = QDateTime::currentDateTime();

    // Создаем 10 карточек с разными датами
    for (int i = 0; i < 10; i++) {
        Card card(i + 1,
                  QString("Mixed Q%1").arg(i),
                  QString("Mixed A%1").arg(i),
                  ContentType::Text,
                  TestMode::DirectAnswer,
                  2.0f,
                  (i % 3) + 1,
                  i % 4,
                  // Каждая третья карточка просрочена
                  (i % 3 == 0) ? now.addDays(-1) : now.addDays(i + 1),
                  now.addDays(-(i + 1)),
                  1);
        cards.append(card);
    }

    deck.setCards(cards);

    QList<Card> dueCards = deck.getDueCards();
    int expectedDue = 4; // i = 0, 3, 6, 9 (каждая третья, начиная с 0)
    QCOMPARE(dueCards.size(), expectedDue);
    QCOMPARE(deck.getDueCount(), expectedDue);

    // Проверяем, что только нужные карточки попали
    for (const Card& card : dueCards) {
        int id = card.getId();
        // ID должны быть: 1, 4, 7, 10
        QVERIFY(id == 1 || id == 4 || id == 7 || id == 10);
    }
}

void TestDeck::testGetDueCountEmptyDeck()
{
    Deck deck;
    QCOMPARE(deck.getDueCount(), 0);

    deck.setName("Test");
    QCOMPARE(deck.getDueCount(), 0); // Все еще 0
}

void TestDeck::testGetDueCountAllDue()
{
    Deck deck;
    QList<Card> cards;
    QDateTime past = QDateTime::currentDateTime().addSecs(-60); // 1 минуту назад

    for (int i = 0; i < 100; i++) {
        Card card(i,
                  QString("Mass Q%1").arg(i),
                  QString("Mass A%1").arg(i),
                  ContentType::Text,
                  TestMode::DirectAnswer,
                  2.0f,
                  1,
                  0,
                  past,
                  past.addDays(-1),
                  1);
        cards.append(card);
    }

    deck.setCards(cards);
    QCOMPARE(deck.getDueCount(), 100);
}

void TestDeck::testGetDueCountSomeDue()
{
    Deck deck;
    QList<Card> cards;
    QDateTime now = QDateTime::currentDateTime();

    // 10 карточек, половина просрочена
    for (int i = 0; i < 10; i++) {
        Card card(i,
                  QString("Half Q%1").arg(i),
                  QString("Half A%1").arg(i),
                  ContentType::Text,
                  TestMode::DirectAnswer,
                  2.0f,
                  1,
                  0,
                  (i < 5) ? now.addDays(-1) : now.addDays(1), // первые 5 просрочены
                  now.addDays(-2),
                  1);
        cards.append(card);
    }

    deck.setCards(cards);
    QCOMPARE(deck.getDueCount(), 5);
}

void TestDeck::testGetDueCountNoneDue()
{
    Deck deck;
    QList<Card> cards;
    QDateTime now = QDateTime::currentDateTime();

    // 7 карточек, все на будущее
    for (int i = 0; i < 7; i++) {
        Card card(i,
                  QString("Future Q%1").arg(i),
                  QString("Future A%1").arg(i),
                  ContentType::Text,
                  TestMode::DirectAnswer,
                  2.0f,
                  1,
                  0,
                  now.addDays(i + 100), // Далеко в будущем
                  now,
                  1);
        cards.append(card);
    }

    deck.setCards(cards);
    QCOMPARE(deck.getDueCount(), 0);
}

void TestDeck::testGetDueCountConsistency()
{
    // Важный тест: проверяем, что getDueCount() == getDueCards().size()
    Deck deck;
    QList<Card> cards;
    QDateTime now = QDateTime::currentDateTime();

    // Случайный набор дат
    QList<QDateTime> dates = {
        now.addDays(-2),    // просрочена
        now.addSecs(-3600), // просрочена (1 час назад)
        now,                // сейчас
        now.addDays(1),     // завтра
        QDateTime(),        // пустая
        now.addDays(7),     // через неделю
        now.addDays(-1)     // вчера
    };

    for (int i = 0; i < dates.size(); i++) {
        Card card(i,
                  QString("Consistency Q%1").arg(i),
                  QString("Consistency A%1").arg(i),
                  ContentType::Text,
                  TestMode::DirectAnswer,
                  2.0f,
                  1,
                  0,
                  dates[i],
                  now.addDays(-1),
                  1);
        cards.append(card);
    }

    deck.setCards(cards);

    int dueCount = deck.getDueCount();
    int dueCardsSize = deck.getDueCards().size();

    QCOMPARE(dueCount, dueCardsSize);

    // Логируем для отладки
    qDebug() << "Due count:" << dueCount << ", Due cards size:" << dueCardsSize;
}

void TestDeck::testGetDueCardsOrder()
{
    // Если нужна сортировка по приоритету (например, самые просроченные первые)
    Deck deck;
    QList<Card> cards;
    QDateTime now = QDateTime::currentDateTime();

    // Карточки с разной степенью просроченности
    Card card1(1, "Q1", "A1", ContentType::Text, TestMode::DirectAnswer,
               2.0f, 1, 0, now.addDays(-10), now.addDays(-11), 1); // Просрочена на 10 дней

    Card card2(2, "Q2", "A2", ContentType::Text, TestMode::DirectAnswer,
               2.0f, 1, 0, now.addDays(-2), now.addDays(-3), 1); // Просрочена на 2 дня

    Card card3(3, "Q3", "A3", ContentType::Text, TestMode::DirectAnswer,
               2.0f, 1, 0, now.addSecs(-60), now.addDays(-1), 1); // Просрочена на 1 минуту

    Card card4(4, "Q4", "A4", ContentType::Text, TestMode::DirectAnswer,
               2.0f, 1, 0, now.addDays(1), now, 1); // На завтра (НЕ должна попасть)

    cards.append(card1);
    cards.append(card2);
    cards.append(card3);
    cards.append(card4);

    deck.setCards(cards);

    QList<Card> dueCards = deck.getDueCards();
    QCOMPARE(dueCards.size(), 3);

    // По умолчанию порядок сохраняется как в исходном списке
    // Если нужна сортировка, её нужно добавить в getDueCards()
    QCOMPARE(dueCards[0].getId(), 1);
    QCOMPARE(dueCards[1].getId(), 2);
    QCOMPARE(dueCards[2].getId(), 3);
}

void TestDeck::testGetDueCardsPerformance()
{
    // Тест производительности для большой колоды
    Deck deck;
    QList<Card> cards;
    QDateTime now = QDateTime::currentDateTime();

    const int LARGE_COUNT = 10000;

    for (int i = 0; i < LARGE_COUNT; i++) {
        // Каждая 5-я карточка просрочена
        bool isDue = (i % 5 == 0);
        Card card(i,
                  QString("Perf Q%1").arg(i),
                  QString("Perf A%1").arg(i),
                  ContentType::Text,
                  TestMode::DirectAnswer,
                  2.0f,
                  1,
                  0,
                  isDue ? now.addDays(-1) : now.addDays(1),
                  now,
                  1);
        cards.append(card);
    }

    deck.setCards(cards);

    QBENCHMARK {
        QList<Card> dueCards = deck.getDueCards();
    }

    // Проверяем правильность
    int expectedDue = LARGE_COUNT / 5; // 2000
    QCOMPARE(deck.getDueCount(), expectedDue);
}
