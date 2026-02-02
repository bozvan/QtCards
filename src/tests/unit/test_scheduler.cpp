#include "TestScheduler.h"
#include "Scheduler.h"
#include <QDateTime>
#include <QtTest>
#include <QDebug>

// =============== ИНИЦИАЛИЗАЦИЯ И ОЧИСТКА ===============

void TestScheduler::initTestCase()
{
    qDebug() << "Запуск тестов для класса Scheduler";
    qDebug() << "Время начала:" << QDateTime::currentDateTime().toString();
}

void TestScheduler::cleanupTestCase()
{
    qDebug() << "Завершение тестов для класса Scheduler";
    qDebug() << "Время завершения:" << QDateTime::currentDateTime().toString();
}

void TestScheduler::init() {}

void TestScheduler::cleanup() {}

// =============== ВСПОМОГАТЕЛЬНЫЕ МЕТОДЫ ===============

Card TestScheduler::createTestCard(int id, int daysFromNow) const
{
    QDateTime nextReview = QDateTime::currentDateTime().addDays(daysFromNow);
    QDateTime lastReview = QDateTime::currentDateTime().addDays(-1);

    return Card(id,
                QString("Вопрос %1").arg(id),
                QString("Ответ %1").arg(id),
                ContentType::Text,
                TestMode::DirectAnswer,
                2.0f,  // easyFactor
                qAbs(daysFromNow),  // intervalDays
                1,     // repetitions
                nextReview,
                lastReview,
                1);    // deckId
}

Deck TestScheduler::createTestDeck(int cardCount, int duePercentage) const
{
    Deck deck;
    deck.setId(1);
    deck.setName("Тестовая колода");

    QList<Card> cards;

    for (int i = 0; i < cardCount; i++) {
        // Простой детерминированный подход: каждая N-ая карточка готова
        bool isDue = (i * 100 / cardCount) < duePercentage;
        int daysFromNow = isDue ? -1 : 1;

        cards.append(createTestCard(i + 1, daysFromNow));
    }

    deck.setCards(cards);
    return deck;
}

void TestScheduler::verifyTimestampUpdates(const Card& card, const QDateTime& beforeUpdate) const
{
    QVERIFY(card.getLastReview().isValid());
    QVERIFY(card.getNextReview().isValid());

    // lastReview должен быть после времени до обновления
    QVERIFY(card.getLastReview() >= beforeUpdate);
    QVERIFY(card.getLastReview() <= QDateTime::currentDateTime().addSecs(1));

    // nextReview должен быть после lastReview
    QVERIFY(card.getNextReview() >= card.getLastReview());
}

// =============== ТЕСТЫ ДЛЯ updateCard() ===============

void TestScheduler::testUpdateCardGradeBoundaries()
{
    Card card = createTestCard(1, 1);
    QDateTime beforeUpdate = QDateTime::currentDateTime().addSecs(-1);

    // Тест нижней границы
    card.setEasyFactor(2.0f);
    card.setIntervalDays(10);
    card.setRepetitions(3);

    Scheduler::updateCard(card, 0); // Минимальная оценка
    QVERIFY(card.getEasyFactor() >= 1.3f);
    QVERIFY(card.getEasyFactor() <= 2.5f);
    QCOMPARE(card.getIntervalDays(), 1);
    QCOMPARE(card.getRepetitions(), 0);
    verifyTimestampUpdates(card, beforeUpdate);

    // Тест верхней границы
    Card card2 = createTestCard(2, 1);
    card2.setEasyFactor(2.0f);
    card2.setIntervalDays(10);
    card2.setRepetitions(3);

    Scheduler::updateCard(card2, 5); // Максимальная оценка
    QVERIFY(card2.getEasyFactor() >= 1.3f);
    QVERIFY(card2.getEasyFactor() <= 2.5f);
    QVERIFY(card2.getIntervalDays() > 10);
    QCOMPARE(card2.getRepetitions(), 4);
}

void TestScheduler::testUpdateCardExcellentGrade()
{
    // Оценка 5 (отлично)
    Card card = createTestCard(1, 1);
    float oldEF = card.getEasyFactor();
    int oldInterval = card.getIntervalDays();
    int oldRepetitions = card.getRepetitions();
    QDateTime beforeUpdate = QDateTime::currentDateTime().addSecs(-1);

    Scheduler::updateCard(card, 5);

    // EF должен увеличиться (но не превысить 2.5)
    QVERIFY(card.getEasyFactor() >= oldEF);
    QVERIFY(card.getEasyFactor() <= 2.5f);

    // Интервал должен увеличиться
    QVERIFY(card.getIntervalDays() > oldInterval);

    // Повторения увеличиваются на 1
    QCOMPARE(card.getRepetitions(), oldRepetitions + 1);

    verifyTimestampUpdates(card, beforeUpdate);
}

void TestScheduler::testUpdateCardGoodGrade()
{
    // Оценка 4 (хорошо)
    Card card = createTestCard(1, 1);
    float oldEF = card.getEasyFactor();
    QDateTime beforeUpdate = QDateTime::currentDateTime().addSecs(-1);

    Scheduler::updateCard(card, 4);

    // EF может немного измениться
    QVERIFY(std::abs(card.getEasyFactor() - oldEF) < 0.5f);
    QVERIFY(card.getEasyFactor() >= 1.3f);
    QVERIFY(card.getEasyFactor() <= 2.5f);

    verifyTimestampUpdates(card, beforeUpdate);
}

void TestScheduler::testUpdateCardMediumGrade()
{
    // Оценка 3 (удовлетворительно)
    Card card = createTestCard(1, 1);
    float oldEF = card.getEasyFactor();
    QDateTime beforeUpdate = QDateTime::currentDateTime().addSecs(-1);

    Scheduler::updateCard(card, 3);

    // EF должен уменьшиться
    QVERIFY(card.getEasyFactor() <= oldEF);
    QVERIFY(card.getEasyFactor() >= 1.3f);

    verifyTimestampUpdates(card, beforeUpdate);
}

void TestScheduler::testUpdateCardPoorGrade()
{
    // Оценка 2 (плохо) - меньше 3
    Card card = createTestCard(1, 1);
    card.setEasyFactor(2.5f);
    card.setIntervalDays(30);
    card.setRepetitions(5);
    QDateTime beforeUpdate = QDateTime::currentDateTime().addSecs(-1);

    Scheduler::updateCard(card, 2);

    // При grade < 3: сброс повторений, интервал = 1
    QCOMPARE(card.getRepetitions(), 0);
    QCOMPARE(card.getIntervalDays(), 1);

    // EF сохраняется (или уменьшается, но не сбрасывается)
    QVERIFY(card.getEasyFactor() >= 1.3f);
    QVERIFY(card.getEasyFactor() <= 2.5f);

    verifyTimestampUpdates(card, beforeUpdate);
}

void TestScheduler::testUpdateCardFailGrade()
{
    // Оценка 0-1 (полный провал)
    Card card = createTestCard(1, 1);
    card.setEasyFactor(2.5f);
    card.setIntervalDays(100);
    card.setRepetitions(10);
    QDateTime beforeUpdate = QDateTime::currentDateTime().addSecs(-1);

    Scheduler::updateCard(card, 0);

    QCOMPARE(card.getRepetitions(), 0);
    QCOMPARE(card.getIntervalDays(), 1);

    // EF должен быть в допустимых пределах
    QVERIFY(card.getEasyFactor() >= 1.3f);
    QVERIFY(card.getEasyFactor() <= 2.5f);

    verifyTimestampUpdates(card, beforeUpdate);
}

void TestScheduler::testUpdateCardSequence()
{
    // Симуляция последовательности повторений
    Card card = createTestCard(1, 0);
    card.setEasyFactor(2.5f);
    card.setIntervalDays(0);
    card.setRepetitions(0);

    // День 1: первый просмотр, хороший ответ
    Scheduler::updateCard(card, 4);
    QCOMPARE(card.getIntervalDays(), 1);
    QCOMPARE(card.getRepetitions(), 1);

    // День 2: второй просмотр, отличный ответ
    Scheduler::updateCard(card, 5);
    QCOMPARE(card.getIntervalDays(), 6); // Второй повтор: всегда 6 дней
    QCOMPARE(card.getRepetitions(), 2);

    // День 8: третий просмотр, хороший ответ
    Scheduler::updateCard(card, 4);
    int expectedInterval = static_cast<int>(std::round(6 * card.getEasyFactor()));
    QCOMPARE(card.getIntervalDays(), expectedInterval);
    QCOMPARE(card.getRepetitions(), 3);
}

void TestScheduler::testUpdateCardAfterFailure()
{
    Card card = createTestCard(1, 1);
    card.setEasyFactor(2.5f);
    card.setIntervalDays(30);
    card.setRepetitions(5);

    // Провал
    Scheduler::updateCard(card, 1);
    QCOMPARE(card.getRepetitions(), 0);
    QCOMPARE(card.getIntervalDays(), 1);

    // Затем успешный ответ
    Scheduler::updateCard(card, 4);
    QCOMPARE(card.getRepetitions(), 1);
    QCOMPARE(card.getIntervalDays(), 1); // Первый успех после сброса = 1 день

    // Еще один успешный ответ
    Scheduler::updateCard(card, 4);
    QCOMPARE(card.getRepetitions(), 2);
    QCOMPARE(card.getIntervalDays(), 6); // Второй успех = 6 дней
}

void TestScheduler::testUpdateCardEasyFactorBounds()
{
    // Тест, что EF всегда в пределах [1.3, 2.5]
    QList<float> initialEFs = {1.0f, 1.3f, 1.5f, 2.0f, 2.5f, 3.0f};
    QList<int> grades = {0, 1, 2, 3, 4, 5};

    for (float initialEF : initialEFs) {
        for (int grade : grades) {
            Card card = createTestCard(1, 1);
            card.setEasyFactor(initialEF);

            Scheduler::updateCard(card, grade);

            QVERIFY2(card.getEasyFactor() >= 1.3f,
                     QString("EF ниже 1.3: начальный=%1, grade=%2, полученный=%3")
                         .arg(initialEF).arg(grade).arg(card.getEasyFactor()).toUtf8());

            QVERIFY2(card.getEasyFactor() <= 2.5f,
                     QString("EF выше 2.5: начальный=%1, grade=%2, полученный=%3")
                         .arg(initialEF).arg(grade).arg(card.getEasyFactor()).toUtf8());
        }
    }
}

void TestScheduler::testUpdateCardTimestampUpdates()
{
    Card card = createTestCard(1, 1);
    QDateTime originalNextReview = card.getNextReview();
    QDateTime beforeUpdate = QDateTime::currentDateTime().addSecs(-1);

    Scheduler::updateCard(card, 4);

    // Проверка временных меток
    QVERIFY(card.getLastReview().isValid());
    QVERIFY(card.getNextReview().isValid());

    // lastReview должен быть обновлен
    QVERIFY(card.getLastReview() >= beforeUpdate);

    // nextReview должен измениться
    QVERIFY(card.getNextReview() != originalNextReview);

    // nextReview должен быть после lastReview
    QVERIFY(card.getNextReview() > card.getLastReview());
}

// =============== ТЕСТЫ ДЛЯ getDueCards() и getDueCount() ===============

void TestScheduler::testGetDueCardsEmptyDeck()
{
    Deck deck;
    deck.setName("Пустая колода");

    QList<Card> dueCards = Scheduler::getDueCards(deck);
    int dueCount = Scheduler::getDueCount(deck);

    QVERIFY(dueCards.isEmpty());
    QCOMPARE(dueCount, 0);
}

void TestScheduler::testGetDueCardsAllDue()
{
    Deck deck = createTestDeck(5, 100); // 100% карточек готовы

    QList<Card> dueCards = Scheduler::getDueCards(deck);
    int dueCount = Scheduler::getDueCount(deck);

    QCOMPARE(dueCards.size(), 5);
    QCOMPARE(dueCount, 5);
    QCOMPARE(dueCount, dueCards.size());
}

void TestScheduler::testGetDueCardsSomeDue()
{
    Deck deck = createTestDeck(10, 30); // 30% карточек готовы

    QList<Card> dueCards = Scheduler::getDueCards(deck);
    int dueCount = Scheduler::getDueCount(deck);

    QCOMPARE(dueCount, 3); // 30% от 10 = 3
    QCOMPARE(dueCount, dueCards.size());
}

void TestScheduler::testGetDueCardsNoneDue()
{
    Deck deck = createTestDeck(7, 0); // 0% карточек готовы

    QList<Card> dueCards = Scheduler::getDueCards(deck);
    int dueCount = Scheduler::getDueCount(deck);

    QVERIFY(dueCards.isEmpty());
    QCOMPARE(dueCount, 0);
}

void TestScheduler::testGetDueCardsInvalidDates()
{
    Deck deck;
    QList<Card> cards;

    // Карточка с невалидной датой
    Card card1(1, "Q1", "A1", ContentType::Text, TestMode::DirectAnswer,
               2.0f, 10, 3, QDateTime(), QDateTime::currentDateTime(), 1);

    // Карточка с корректной будущей датой
    Card card2(2, "Q2", "A2", ContentType::Text, TestMode::DirectAnswer,
               2.0f, 10, 3, QDateTime::currentDateTime().addDays(1),
               QDateTime::currentDateTime(), 1);

    cards.append(card1);
    cards.append(card2);
    deck.setCards(cards);

    QList<Card> dueCards = Scheduler::getDueCards(deck);
    int dueCount = Scheduler::getDueCount(deck);

    QCOMPARE(dueCards.size(), 1); // Только карточка с невалидной датой
    QCOMPARE(dueCount, 1);
    QCOMPARE(dueCards[0].getId(), 1);
}

void TestScheduler::testGetDueCardsConsistency()
{
    Deck deck = createTestDeck(20, 40); // 40% готовы

    int dueCount = Scheduler::getDueCount(deck);
    QList<Card> dueCards = Scheduler::getDueCards(deck);

    // Проверка согласованности методов
    QCOMPARE(dueCount, dueCards.size());

    // Проверка, что все возвращенные карточки действительно готовы
    for (const Card& card : dueCards) {
        QVERIFY(Scheduler::isCardDue(card));
    }
}

void TestScheduler::testGetDueCardsLargeDeck()
{
    // Тест производительности для большой колоды
    Deck deck = createTestDeck(10000, 25); // 10k карточек, 25% готовы

    QBENCHMARK {
        QList<Card> dueCards = Scheduler::getDueCards(deck);
    }

    // Проверка правильности
    int expectedDue = 2500; // 25% от 10000
    QCOMPARE(Scheduler::getDueCount(deck), expectedDue);
}

// =============== ТЕСТЫ ДЛЯ isCardDue() ===============

void TestScheduler::testIsCardDuePastDate()
{
    Card card = createTestCard(1, -1); // Дата в прошлом

    QVERIFY(Scheduler::isCardDue(card));
}

void TestScheduler::testIsCardDueCurrentDate()
{
    // Карточка с датой "прямо сейчас"
    Card card = createTestCard(1, 0);
    card.setNextReview(QDateTime::currentDateTime());

    QVERIFY(Scheduler::isCardDue(card));
}

void TestScheduler::testIsCardDueFutureDate()
{
    Card card = createTestCard(1, 1); // Дата в будущем

    QVERIFY(!Scheduler::isCardDue(card));
}

void TestScheduler::testIsCardDueInvalidDate()
{
    Card card = createTestCard(1, 1);
    card.setNextReview(QDateTime()); // Невалидная дата

    QVERIFY(Scheduler::isCardDue(card));
}

void TestScheduler::testIsCardDueExactTime()
{
    QDateTime exactTime = QDateTime::currentDateTime().addSecs(30);
    Card card = createTestCard(1, 0);
    card.setNextReview(exactTime);

    // Если время еще не наступило
    if (QDateTime::currentDateTime() < exactTime) {
        QVERIFY(!Scheduler::isCardDue(card));
    }
}

// =============== ТЕСТЫ ДЛЯ calculateNextInterval() ===============

void TestScheduler::testCalculateNextIntervalFirstSuccess()
{
    // Первый успешный ответ после сброса (repetitions=0)
    int interval = Scheduler::calculateNextInterval(10, 0, 2.0f, 4);
    QCOMPARE(interval, 1);
}

void TestScheduler::testCalculateNextIntervalSecondSuccess()
{
    // Второй успешный ответ (repetitions=1)
    int interval = Scheduler::calculateNextInterval(1, 1, 2.0f, 4);
    QCOMPARE(interval, 6);
}

void TestScheduler::testCalculateNextIntervalSubsequent()
{
    // Третий и последующие успешные ответы
    int oldInterval = 10;
    float easyFactor = 2.0f;
    int interval = Scheduler::calculateNextInterval(oldInterval, 3, easyFactor, 4);

    int expected = static_cast<int>(std::round(oldInterval * easyFactor));
    QCOMPARE(interval, expected);
}

void TestScheduler::testCalculateNextIntervalFailure()
{
    // Неудачный ответ (grade < 3)
    int interval = Scheduler::calculateNextInterval(100, 10, 2.5f, 2);
    QCOMPARE(interval, 1);
}

void TestScheduler::testCalculateNextIntervalEdgeCases()
{
    // Граничные значения
    QCOMPARE(Scheduler::calculateNextInterval(1, 0, 1.3f, 5), 1);
    QCOMPARE(Scheduler::calculateNextInterval(1000, 1, 2.5f, 5), 6);
    QCOMPARE(Scheduler::calculateNextInterval(0, 2, 2.0f, 5), 0); // 0 * 2.0 = 0
}

// =============== ТЕСТЫ ДЛЯ calculateNextEasyFactor() ===============

void TestScheduler::testCalculateNextEasyFactorFormula()
{
    // Проверка точности формулы

    // grade=5: EF' = EF + (0.1 - 0) = EF + 0.1
    float ef1 = Scheduler::calculateNextEasyFactor(2.0f, 5);
    QCOMPARE(qRound(ef1 * 10.0f), 21); // 2.1

    // grade=4: EF' = EF + (0.1 - 1*(0.08 + 1*0.02)) = EF + (0.1 - 0.1) = EF
    float ef2 = Scheduler::calculateNextEasyFactor(2.0f, 4);
    QCOMPARE(qRound(ef2 * 10.0f), 20); // 2.0

    // grade=3: EF' = EF + (0.1 - 2*(0.08 + 2*0.02)) = EF + (0.1 - 2*0.12) = EF - 0.14
    float ef3 = Scheduler::calculateNextEasyFactor(2.0f, 3);
    QCOMPARE(qRound(ef3 * 100.0f), 186); // 1.86
}

void TestScheduler::testCalculateNextEasyFactorBounds()
{
    // Тест границ [1.3, 2.5]

    // Нижняя граница
    float ef1 = Scheduler::calculateNextEasyFactor(1.3f, 0);
    QCOMPARE(ef1, 1.3f);

    // Верхняя граница
    float ef2 = Scheduler::calculateNextEasyFactor(2.5f, 5);
    QCOMPARE(ef2, 2.5f);

    // За пределами границ
    float ef3 = Scheduler::calculateNextEasyFactor(1.0f, 0); // Ниже минимума
    QVERIFY(ef3 >= 1.3f);

    float ef4 = Scheduler::calculateNextEasyFactor(3.0f, 5); // Выше максимума
    QVERIFY(ef4 <= 2.5f);
}

void TestScheduler::testCalculateNextEasyFactorAllGrades()
{
    // Проверка всех возможных оценок
    for (int grade = 0; grade <= 5; grade++) {
        float ef = Scheduler::calculateNextEasyFactor(2.0f, grade);

        QVERIFY(ef >= 1.3f);
        QVERIFY(ef <= 2.5f);

        // grade=5 увеличивает EF, grade=3 уменьшает
        if (grade == 5) {
            QVERIFY(ef >= 2.0f);
        } else if (grade == 3) {
            QVERIFY(ef <= 2.0f);
        }
    }
}

// =============== ИНТЕГРАЦИОННЫЕ ТЕСТЫ ===============

void TestScheduler::testIntegrationCompleteWorkflow()
{
    // Полный рабочий процесс
    Deck deck = createTestDeck(3, 100);

    // 1. Получить карточки для повторения
    QList<Card> dueCards = Scheduler::getDueCards(deck);
    QVERIFY(!dueCards.isEmpty());

    // 2. Обновить первую карточку с оценкой, которая точно изменит EF
    Card& card = dueCards[0];
    float oldEF = card.getEasyFactor();
    int oldInterval = card.getIntervalDays();

    // Используем оценку 5 или 3, чтобы EF точно изменился
    Scheduler::updateCard(card, 5); // Отличный ответ

    // 3. Проверить изменения
    QVERIFY(card.getIntervalDays() != oldInterval);
    // Для оценки 5 EF должен увеличиться
    QVERIFY(card.getEasyFactor() > oldEF);
    QVERIFY(card.getLastReview().isValid());
    QVERIFY(card.getNextReview().isValid());

    // 4. Проверить, что карточка больше не готова к повторению
    QVERIFY(!Scheduler::isCardDue(card));
}

void TestScheduler::testIntegrationMultipleCards()
{
    Deck deck = createTestDeck(10, 50); // 10 карточек, 50% готовы

    int initialDueCount = Scheduler::getDueCount(deck);
    QList<Card> initialDueCards = Scheduler::getDueCards(deck);

    // Проверяем, что действительно есть готовые карточки
    QVERIFY(initialDueCount > 0);
    QCOMPARE(initialDueCards.size(), initialDueCount);

    // Обновить все готовые карточки
    for (Card& card : initialDueCards) {
        Scheduler::updateCard(card, 3); // Средний ответ
    }

    // Получить колоду с обновленными карточками
    deck.setCards(initialDueCards + deck.getCards().mid(initialDueCount));

    // После обновления готовых карточек должно стать меньше
    int newDueCount = Scheduler::getDueCount(deck);
    QVERIFY(newDueCount < initialDueCount || newDueCount == 0);

    // Обновленные карточки не должны быть готовы (если nextReview установлен корректно)
    for (const Card& card : initialDueCards) {
        QVERIFY(card.getNextReview().isValid());
        QVERIFY(card.getNextReview() > QDateTime::currentDateTime());
    }
}

void TestScheduler::testIntegrationRealisticScenario()
{
    // Реалистичный сценарий изучения
    Deck deck;
    QList<Card> cards;

    // Создаем 3 карточки с разным прогрессом
    Card card1(1, "Hello", "Привет", ContentType::Text, TestMode::DirectAnswer,
               2.5f, 0, 0, QDateTime(), QDateTime(), 1);

    Card card2(2, "Goodbye", "До свидания", ContentType::Text, TestMode::DirectAnswer,
               2.0f, 1, 1, QDateTime::currentDateTime().addDays(-1),
               QDateTime::currentDateTime().addDays(-2), 1);

    Card card3(3, "Thank you", "Спасибо", ContentType::Text, TestMode::DirectAnswer,
               1.8f, 6, 2, QDateTime::currentDateTime().addDays(1),
               QDateTime::currentDateTime().addDays(-1), 1);

    cards.append(card1);
    cards.append(card2);
    cards.append(card3);
    deck.setCards(cards);

    // Проверить готовые карточки
    QList<Card> dueCards = Scheduler::getDueCards(deck);
    QCOMPARE(dueCards.size(), 2); // card1 и card2

    // Изучить card2 (отличный ответ)
    for (int i = 0; i < dueCards.size(); i++) {
        if (dueCards[i].getId() == 2) {
            Scheduler::updateCard(dueCards[i], 5);
            // Обновить карточку в основной коллекции
            cards[1] = dueCards[i];
            deck.setCards(cards);
            break;
        }
    }

    // Проверить изменения
    QList<Card> newDueCards = Scheduler::getDueCards(deck);
    QCOMPARE(newDueCards.size(), 1); // Теперь только card1
}

// =============== ТЕСТЫ ГРАНИЧНЫХ СЛУЧАЕВ ===============

void TestScheduler::testEdgeCasesMinimalValues()
{
    // Минимальные значения
    Card card(1, "", "", ContentType::Text, TestMode::DirectAnswer,
              std::numeric_limits<float>::min(),
              std::numeric_limits<int>::min(),
              std::numeric_limits<int>::min(),
              QDateTime(),
              QDateTime(),
              std::numeric_limits<int>::min());

    Scheduler::updateCard(card, 0);

    // Проверка, что значения в допустимых пределах
    QVERIFY(card.getEasyFactor() >= 1.3f);
    QVERIFY(card.getEasyFactor() <= 2.5f);
    QVERIFY(card.getIntervalDays() >= 1);
    QVERIFY(card.getRepetitions() >= 0);
}

void TestScheduler::testEdgeCasesMaximalValues()
{
    // Используем разумные максимальные значения вместо абсолютных максимумов
    Card card(1,
              QString(1000, 'Q'), // Длинная строка
              QString(1000, 'A'),
              ContentType::Image, // Последний enum
              TestMode::MultipleChoice, // Последний enum
              2.5f, // Максимальное допустимое значение для easyFactor
              36500, // ~100 лет в днях - разумный максимум
              1000, // Максимальное разумное количество повторений
              QDateTime(QDate(2999, 12, 31), QTime(23, 59, 59)),
              QDateTime(QDate(2000, 1, 1), QTime(0, 0, 0)),
              1000000); // Большой, но разумный deckId

    Scheduler::updateCard(card, 5);

    QVERIFY(card.getEasyFactor() <= 2.5f);
    QVERIFY(card.getEasyFactor() >= 1.3f);

    // Интервал должен быть положительным
    QVERIFY2(card.getIntervalDays() >= 1,
             QString("Интервал должен быть >= 1, получен: %1").arg(card.getIntervalDays()).toUtf8());

    QVERIFY(card.getNextReview().isValid());
    QVERIFY(card.getLastReview().isValid());

    // Проверим, что nextReview корректно установлен
    QVERIFY(card.getNextReview() > card.getLastReview());
}

void TestScheduler::testEdgeCasesInvalidInputs()
{
    // Входные данные за пределами допустимых
    Card card = createTestCard(1, 1);

    // Оценка за пределами [0, 5]
    Scheduler::updateCard(card, -100);
    QVERIFY(card.getEasyFactor() >= 1.3f);
    QVERIFY(card.getEasyFactor() <= 2.5f);

    Scheduler::updateCard(card, 100);
    QVERIFY(card.getEasyFactor() >= 1.3f);
    QVERIFY(card.getEasyFactor() <= 2.5f);
}

// =============== ТЕСТЫ ПРОИЗВОДИТЕЛЬНОСТИ ===============

void TestScheduler::testPerformanceUpdateCard()
{
    Card card = createTestCard(1, 1);

    QBENCHMARK {
        for (int i = 0; i < 1000; i++) {
            Scheduler::updateCard(card, 3);
        }
    }
}

void TestScheduler::testPerformanceGetDueCards()
{
    Deck deck = createTestDeck(5000, 30); // 5000 карточек

    QBENCHMARK {
        QList<Card> dueCards = Scheduler::getDueCards(deck);
    }
}
