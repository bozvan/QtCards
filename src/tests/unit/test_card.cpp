#include <QtTest>
#include <QSignalSpy>
#include <QDateTime>
#include <climits>
#include "Card.h"

class TestCard : public QObject {
    Q_OBJECT
private slots:
    // Конструкторы
    void testDefaultConstructor();
    void testParameterizedConstructor();

    // Тесты для сеттеров
    void testSetId();
    void testSetQuestion();
    void testSetAnswer();
    void testSetContentType();
    void testSetTestMode();
    void testSetEasyFactor();
    void testSetIntervalDays();
    void testSetRepetitions();
    void testSetNextReview();
    void testSetLastReview();
    void testSetDeckId();

    // Тесты для геттеров
    void testGetId();
    void testGetQuestion();
    void testGetAnswer();
    void testGetContentType();
    void testGetTestMode();
    void testGetEasyFactor();
    void testGetIntervalDays();
    void testGetRepetitions();
    void testGetNextReview();
    void testGetLastReview();
    void testGetDeckId();

    // Интеграционные тесты
    void testSetAndGetConsistency();
    void testEdgeCases();
};


// ==================== КОНСТРУКТОРЫ ====================
void TestCard::testDefaultConstructor() {
    Card card;
    QCOMPARE(card.getId(), 0);
    QCOMPARE(card.getQuestion(), QString());
    QCOMPARE(card.getAnswer(), QString());
    QCOMPARE(card.getEasyFactor(), 0.0f);
    QCOMPARE(card.getIntervalDays(), 0);
    QCOMPARE(card.getRepetitions(), 0);
    QCOMPARE(card.getDeckId(), 0);
}

void TestCard::testParameterizedConstructor() {
    QDateTime nextReview = QDateTime::currentDateTime().addDays(1);
    QDateTime lastReview = QDateTime::currentDateTime().addDays(-1);

    Card card(1, "Q?", "A.", ContentType::Text, TestMode::DirectAnswer,
              2.5f, 7, 3, nextReview, lastReview, 10);

    QCOMPARE(card.getId(), 1);
    QCOMPARE(card.getQuestion(), "Q?");
    QCOMPARE(card.getAnswer(), "A.");
    QCOMPARE(card.getContentType(), ContentType::Text);
    QCOMPARE(card.getTestMode(), TestMode::DirectAnswer);
    QCOMPARE(card.getEasyFactor(), 2.5f);
    QCOMPARE(card.getIntervalDays(), 7);
    QCOMPARE(card.getRepetitions(), 3);
    QCOMPARE(card.getNextReview(), nextReview);
    QCOMPARE(card.getLastReview(), lastReview);
    QCOMPARE(card.getDeckId(), 10);
}

// ==================== СЕТТЕРЫ ====================
void TestCard::testSetId() {
    Card card;
    card.setId(42);
    QVERIFY(card.getId() == 42);
}

void TestCard::testSetQuestion() {
    Card card;
    card.setQuestion("New question?");
    QVERIFY(card.getQuestion() == "New question?");
}

void TestCard::testSetAnswer() {
    Card card;
    card.setAnswer("New answer.");
    QVERIFY(card.getAnswer() == "New answer.");
}

void TestCard::testSetContentType() {
    Card card;
    card.setContentType(ContentType::Image);
    QVERIFY(card.getContentType() == ContentType::Image);
}

void TestCard::testSetTestMode() {
    Card card;
    card.setTestMode(TestMode::MultipleChoice);
    QVERIFY(card.getTestMode() == TestMode::MultipleChoice);
}

void TestCard::testSetEasyFactor() {
    Card card;
    card.setEasyFactor(1.7f);
    QVERIFY(qFuzzyCompare(card.getEasyFactor(), 1.7f));
}

void TestCard::testSetIntervalDays() {
    Card card;
    card.setIntervalDays(14);
    QVERIFY(card.getIntervalDays() == 14);
}

void TestCard::testSetRepetitions() {
    Card card;
    card.setRepetitions(5);
    QVERIFY(card.getRepetitions() == 5);
}

void TestCard::testSetNextReview() {
    Card card;
    QDateTime newTime = QDateTime::fromMSecsSinceEpoch(123456789);
    card.setNextReview(newTime);
    QVERIFY(card.getNextReview() == newTime);
}

void TestCard::testSetLastReview() {
    Card card;
    QDateTime newTime = QDateTime::currentDateTime();
    card.setLastReview(newTime);
    QVERIFY(card.getLastReview() == newTime);
}

void TestCard::testSetDeckId() {
    Card card;
    card.setDeckId(99);
    QVERIFY(card.getDeckId() == 99);
}

// ==================== ГЕТТЕРЫ ====================
void TestCard::testGetId() {
    Card card(42, "", "", ContentType::Text, TestMode::DirectAnswer,
              0.0f, 0, 0, QDateTime(), QDateTime(), 0);
    QCOMPARE(card.getId(), 42);
}

void TestCard::testGetQuestion() {
    Card card(0, "Test question?", "", ContentType::Text, TestMode::DirectAnswer,
              0.0f, 0, 0, QDateTime(), QDateTime(), 0);
    QCOMPARE(card.getQuestion(), "Test question?");
}

void TestCard::testGetAnswer() {
    Card card(0, "", "Test answer", ContentType::Text, TestMode::DirectAnswer,
              0.0f, 0, 0, QDateTime(), QDateTime(), 0);
    QCOMPARE(card.getAnswer(), "Test answer");
}

void TestCard::testGetContentType() {
    Card card(0, "", "", ContentType::Audio, TestMode::DirectAnswer,
              0.0f, 0, 0, QDateTime(), QDateTime(), 0);
    QCOMPARE(card.getContentType(), ContentType::Audio);
}

void TestCard::testGetTestMode() {
    Card card(0, "", "", ContentType::Text, TestMode::Matching,
              0.0f, 0, 0, QDateTime(), QDateTime(), 0);
    QCOMPARE(card.getTestMode(), TestMode::Matching);
}

void TestCard::testGetEasyFactor() {
    Card card(0, "", "", ContentType::Text, TestMode::DirectAnswer,
              2.1f, 0, 0, QDateTime(), QDateTime(), 0);
    QVERIFY(qFuzzyCompare(card.getEasyFactor(), 2.1f));
}

void TestCard::testGetIntervalDays() {
    Card card(0, "", "", ContentType::Text, TestMode::DirectAnswer,
              0.0f, 30, 0, QDateTime(), QDateTime(), 0);
    QCOMPARE(card.getIntervalDays(), 30);
}

void TestCard::testGetRepetitions() {
    Card card(0, "", "", ContentType::Text, TestMode::DirectAnswer,
              0.0f, 0, 10, QDateTime(), QDateTime(), 0);
    QCOMPARE(card.getRepetitions(), 10);
}

void TestCard::testGetNextReview() {
    QDateTime testTime = QDateTime::fromString("2024-01-01T12:00:00", Qt::ISODate);
    Card card(0, "", "", ContentType::Text, TestMode::DirectAnswer,
              0.0f, 0, 0, testTime, QDateTime(), 0);
    QCOMPARE(card.getNextReview(), testTime);
}

void TestCard::testGetLastReview() {
    QDateTime testTime = QDateTime::fromString("2023-12-31T23:59:59", Qt::ISODate);
    Card card(0, "", "", ContentType::Text, TestMode::DirectAnswer,
              0.0f, 0, 0, QDateTime(), testTime, 0);
    QCOMPARE(card.getLastReview(), testTime);
}

void TestCard::testGetDeckId() {
    Card card(0, "", "", ContentType::Text, TestMode::DirectAnswer,
              0.0f, 0, 0, QDateTime(), QDateTime(), 25);
    QCOMPARE(card.getDeckId(), 25);
}

// ==================== ИНТЕГРАЦИОННЫЕ ТЕСТЫ ====================
void TestCard::testSetAndGetConsistency() {
    Card card;

    // Устанавливаем все значения
    card.setId(100);
    card.setQuestion("Consistency test?");
    card.setAnswer("Yes");
    card.setContentType(ContentType::Image);
    card.setTestMode(TestMode::MultipleChoice);
    card.setEasyFactor(1.5f);
    card.setIntervalDays(21);
    card.setRepetitions(7);

    QDateTime next = QDateTime::currentDateTime().addDays(5);
    QDateTime last = QDateTime::currentDateTime().addDays(-5);
    card.setNextReview(next);
    card.setLastReview(last);
    card.setDeckId(50);

    // Проверяем согласованность
    QCOMPARE(card.getId(), 100);
    QCOMPARE(card.getQuestion(), "Consistency test?");
    QCOMPARE(card.getAnswer(), "Yes");
    QCOMPARE(card.getContentType(), ContentType::Image);
    QCOMPARE(card.getTestMode(), TestMode::MultipleChoice);
    QVERIFY(qFuzzyCompare(card.getEasyFactor(), 1.5f));
    QCOMPARE(card.getIntervalDays(), 21);
    QCOMPARE(card.getRepetitions(), 7);
    QCOMPARE(card.getNextReview(), next);
    QCOMPARE(card.getLastReview(), last);
    QCOMPARE(card.getDeckId(), 50);
}

void TestCard::testEdgeCases() {
    Card card;

    // Граничные значения для ID
    card.setId(INT_MAX);
    QCOMPARE(card.getId(), INT_MAX);

    card.setId(INT_MIN);
    QCOMPARE(card.getId(), INT_MIN);

    // Очень длинная строка
    QString longString(5000, 'X');
    card.setQuestion(longString);
    QCOMPARE(card.getQuestion().length(), 5000);

    // Специальные символы
    card.setAnswer("Line1\nLine2\tTab\r\nUnicode: 测试 тест");
    QCOMPARE(card.getAnswer(), "Line1\nLine2\tTab\r\nUnicode: 测试 тест");

    // Нулевые даты
    card.setNextReview(QDateTime());
    QVERIFY(card.getNextReview().isNull());

    card.setLastReview(QDateTime());
    QVERIFY(card.getLastReview().isNull());

    // Отрицательные значения
    card.setEasyFactor(-10.5f);
    QVERIFY(qFuzzyCompare(card.getEasyFactor(), -10.5f));

    card.setIntervalDays(-100);
    QCOMPARE(card.getIntervalDays(), -100);

    card.setRepetitions(-5);
    QCOMPARE(card.getRepetitions(), -5);
}

QTEST_APPLESS_MAIN(TestCard)
#include "test_card.moc"
