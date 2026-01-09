#include <QtTest>
#include <QSignalSpy>
#include <QDateTime>
#include <climits>
#include "Card.h"

class TestCard : public QObject {
    Q_OBJECT
private slots:
    // Constructor tests
    void testDefaultConstructor();
    void testParameterizedConstructor();
    void testCopyConstructor();
    void testCopyAssignmentOperator();
    void testMoveConstructor();
    void testMoveAssignmentOperator();
    void testMoveFromTemporary();
    void testMoveSelfAssignment();
    void testMoveEmptyObject();
    void testMoveAfterCopy();
    void testMovePerformance();
    void testMoveThenUseSource();

    // Setter tests
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

    // Getter tests
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

    // Integration tests
    void testSetAndGetConsistency();
    void testEdgeCases();
};


// ==================== CONSTRUCTORS ====================
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


void TestCard::testCopyConstructor() {
    QDateTime nextReview = QDateTime::currentDateTime().addDays(2);
    QDateTime lastReview = QDateTime::currentDateTime().addDays(-2);

    Card original(100, "Original question?", "Original answer.",
                  ContentType::Audio, TestMode::Matching,
                  2.0f, 14, 5, nextReview, lastReview, 50);

    Card copy(original);

    QCOMPARE(copy.getId(), original.getId());
    QCOMPARE(copy.getQuestion(), original.getQuestion());
    QCOMPARE(copy.getAnswer(), original.getAnswer());
    QCOMPARE(copy.getContentType(), original.getContentType());
    QCOMPARE(copy.getTestMode(), original.getTestMode());
    QVERIFY(qFuzzyCompare(copy.getEasyFactor(), original.getEasyFactor()));
    QCOMPARE(copy.getIntervalDays(), original.getIntervalDays());
    QCOMPARE(copy.getRepetitions(), original.getRepetitions());
    QCOMPARE(copy.getNextReview(), original.getNextReview());
    QCOMPARE(copy.getLastReview(), original.getLastReview());
    QCOMPARE(copy.getDeckId(), original.getDeckId());

    // Проверяем глубокое копирование (изменение копии не влияет на оригинал)
    copy.setQuestion("Modified question");
    QVERIFY(copy.getQuestion() != original.getQuestion());
    QCOMPARE(original.getQuestion(), QString("Original question?"));

    copy.setId(999);
    QVERIFY(copy.getId() != original.getId());
    QCOMPARE(original.getId(), 100);

    // Проверяем с пустыми значениями
    Card emptyOriginal;
    Card emptyCopy(emptyOriginal);

    QCOMPARE(emptyCopy.getId(), 0);
    QCOMPARE(emptyCopy.getQuestion(), QString());
    QCOMPARE(emptyCopy.getAnswer(), QString());

    // Проверяем с граничными значениями
    Card edgeCase(INT_MAX, "", "", ContentType::Text, TestMode::DirectAnswer,
                  -100.0f, INT_MIN, INT_MAX, QDateTime(), QDateTime(), INT_MIN);
    Card edgeCopy(edgeCase);

    QCOMPARE(edgeCopy.getId(), INT_MAX);
    QCOMPARE(edgeCopy.getIntervalDays(), INT_MIN);
    QCOMPARE(edgeCopy.getRepetitions(), INT_MAX);
    QCOMPARE(edgeCopy.getDeckId(), INT_MIN);
    QVERIFY(qFuzzyCompare(edgeCopy.getEasyFactor(), -100.0f));
}

void TestCard::testCopyAssignmentOperator() {
    // Arrange
    QDateTime next1 = QDateTime::currentDateTime().addDays(1);
    QDateTime last1 = QDateTime::currentDateTime().addDays(-1);
    Card card1(1, "Original Question", "Original Answer",
               ContentType::Text, TestMode::DirectAnswer,
               1.5f, 7, 3, next1, last1, 10);

    QDateTime next2 = QDateTime::currentDateTime().addDays(2);
    QDateTime last2 = QDateTime::currentDateTime().addDays(-2);
    Card card2(2, "New Question", "New Answer",
               ContentType::Image, TestMode::MultipleChoice,
               2.0f, 14, 5, next2, last2, 20);

    // Act
    card1 = card2; // Copy assignment

    // Assert
    QCOMPARE(card1.getId(), 2);
    QCOMPARE(card1.getQuestion(), "New Question");
    QCOMPARE(card1.getAnswer(), "New Answer");
    QCOMPARE(card1.getContentType(), ContentType::Image);
    QCOMPARE(card1.getTestMode(), TestMode::MultipleChoice);
    QVERIFY(qFuzzyCompare(card1.getEasyFactor(), 2.0f));
    QCOMPARE(card1.getIntervalDays(), 14);
    QCOMPARE(card1.getRepetitions(), 5);
    QCOMPARE(card1.getNextReview(), next2);
    QCOMPARE(card1.getLastReview(), last2);
    QCOMPARE(card1.getDeckId(), 20);

    // Source should remain unchanged
    QCOMPARE(card2.getId(), 2);
    QCOMPARE(card2.getQuestion(), "New Question");
}

void TestCard::testMoveConstructor() {
    // Arrange
    QDateTime next = QDateTime::currentDateTime().addDays(1);
    QDateTime last = QDateTime::currentDateTime().addDays(-1);

    // Act - создаем временный объект и перемещаем его
    Card source(1, "Move Source", "Move Answer",
                ContentType::Audio, TestMode::Matching,
                2.5f, 30, 10, next, last, 50);

    Card destination(std::move(source));

    // Assert - destination должен получить значения
    QCOMPARE(destination.getId(), 1);
    QCOMPARE(destination.getQuestion(), "Move Source");
    QCOMPARE(destination.getAnswer(), "Move Answer");
    QCOMPARE(destination.getContentType(), ContentType::Audio);
    QCOMPARE(destination.getTestMode(), TestMode::Matching);
    QVERIFY(qFuzzyCompare(destination.getEasyFactor(), 2.5f));
    QCOMPARE(destination.getIntervalDays(), 30);
    QCOMPARE(destination.getRepetitions(), 10);
    QCOMPARE(destination.getNextReview(), next);
    QCOMPARE(destination.getLastReview(), last);
    QCOMPARE(destination.getDeckId(), 50);

    // Source должен быть в валидном, но перемещенном состоянии
    QCOMPARE(source.getId(), 0);
    QCOMPARE(source.getQuestion(), QString());
    QCOMPARE(source.getAnswer(), QString());
    QCOMPARE(source.getIntervalDays(), 0);
    QCOMPARE(source.getRepetitions(), 0);
    QCOMPARE(source.getDeckId(), 0);
    // Enum значения остаются как есть (не обнуляются)
}

void TestCard::testMoveAssignmentOperator() {
    // Arrange
    QDateTime next1 = QDateTime::currentDateTime().addDays(1);
    QDateTime last1 = QDateTime::currentDateTime().addDays(-1);
    Card card1(1, "Card 1", "Answer 1",
               ContentType::Text, TestMode::DirectAnswer,
               1.3f, 5, 2, next1, last1, 10);

    QDateTime next2 = QDateTime::currentDateTime().addDays(2);
    QDateTime last2 = QDateTime::currentDateTime().addDays(-2);
    Card card2(2, "Card 2", "Answer 2",
               ContentType::Image, TestMode::MultipleChoice,
               2.1f, 10, 4, next2, last2, 20);

    // Act
    card1 = std::move(card2); // Move assignment

    // Assert - card1 получает значения card2
    QCOMPARE(card1.getId(), 2);
    QCOMPARE(card1.getQuestion(), "Card 2");
    QCOMPARE(card1.getAnswer(), "Answer 2");
    QCOMPARE(card1.getContentType(), ContentType::Image);
    QCOMPARE(card1.getTestMode(), TestMode::MultipleChoice);
    QVERIFY(qFuzzyCompare(card1.getEasyFactor(), 2.1f));
    QCOMPARE(card1.getIntervalDays(), 10);
    QCOMPARE(card1.getRepetitions(), 4);
    QCOMPARE(card1.getNextReview(), next2);
    QCOMPARE(card1.getLastReview(), last2);
    QCOMPARE(card1.getDeckId(), 20);

    // card2 должен быть в перемещенном состоянии
    QCOMPARE(card2.getId(), 0);
    QCOMPARE(card2.getIntervalDays(), 0);
    QCOMPARE(card2.getRepetitions(), 0);
    QCOMPARE(card2.getDeckId(), 0);
}

void TestCard::testMoveFromTemporary() {
    // Arrange & Act - создаем и сразу перемещаем временный объект
    Card card = Card(3, "Temporary", "Temp Answer",
                     ContentType::Audio, TestMode::Matching,
                     3.0f, 21, 7,
                     QDateTime::currentDateTime(),
                     QDateTime::currentDateTime().addDays(-1),
                     30);

    // Assert
    QCOMPARE(card.getId(), 3);
    QCOMPARE(card.getQuestion(), "Temporary");
    QCOMPARE(card.getAnswer(), "Temp Answer");
    QCOMPARE(card.getContentType(), ContentType::Audio);
    QCOMPARE(card.getTestMode(), TestMode::Matching);
    QVERIFY(qFuzzyCompare(card.getEasyFactor(), 3.0f));
    QCOMPARE(card.getIntervalDays(), 21);
    QCOMPARE(card.getRepetitions(), 7);
    QCOMPARE(card.getDeckId(), 30);
}

void TestCard::testMoveSelfAssignment() {
    // Arrange
    QDateTime next = QDateTime::currentDateTime().addDays(1);
    QDateTime last = QDateTime::currentDateTime().addDays(-1);
    Card card(1, "Self Assignment", "Test",
              ContentType::Text, TestMode::DirectAnswer,
              1.5f, 7, 3, next, last, 10);

    // Act - self move assignment
    card = std::move(card);

    // Assert - объект должен остаться в валидном состоянии
    QCOMPARE(card.getId(), 1);
    QCOMPARE(card.getQuestion(), "Self Assignment");
    QCOMPARE(card.getAnswer(), "Test");
    QCOMPARE(card.getContentType(), ContentType::Text);
    QCOMPARE(card.getTestMode(), TestMode::DirectAnswer);
    QVERIFY(qFuzzyCompare(card.getEasyFactor(), 1.5f));
    QCOMPARE(card.getIntervalDays(), 7);
    QCOMPARE(card.getRepetitions(), 3);
    QCOMPARE(card.getNextReview(), next);
    QCOMPARE(card.getLastReview(), last);
    QCOMPARE(card.getDeckId(), 10);
}

void TestCard::testMoveEmptyObject() {
    // Arrange
    Card emptySource; // Default constructor
    Card destination(1, "Not Empty", "Has Data",
                     ContentType::Text, TestMode::DirectAnswer,
                     1.0f, 1, 1, QDateTime(), QDateTime(), 1);

    // Act - перемещаем пустой объект
    destination = std::move(emptySource);

    // Assert - destination становится пустым
    QCOMPARE(destination.getId(), 0);
    QCOMPARE(destination.getQuestion(), QString());
    QCOMPARE(destination.getAnswer(), QString());
    QCOMPARE(destination.getEasyFactor(), 0.0f);
    QCOMPARE(destination.getIntervalDays(), 0);
    QCOMPARE(destination.getRepetitions(), 0);
    QCOMPARE(destination.getDeckId(), 0);

    // Source становится еще более пустым (хотя уже был пустым)
    QCOMPARE(emptySource.getId(), 0);
}

void TestCard::testMoveAfterCopy() {
    // Arrange
    QDateTime next = QDateTime::currentDateTime().addDays(1);
    QDateTime last = QDateTime::currentDateTime().addDays(-1);

    Card original(1, "Original", "Data",
                  ContentType::Image, TestMode::MultipleChoice,
                  2.0f, 14, 5, next, last, 20);

    // Act - сначала копируем, затем перемещаем
    Card copy = original; // Copy constructor
    Card moved = std::move(copy); // Move constructor

    // Assert - moved содержит данные оригинала
    QCOMPARE(moved.getId(), 1);
    QCOMPARE(moved.getQuestion(), "Original");
    QCOMPARE(moved.getContentType(), ContentType::Image);
    QCOMPARE(moved.getTestMode(), TestMode::MultipleChoice);
    QVERIFY(qFuzzyCompare(moved.getEasyFactor(), 2.0f));
    QCOMPARE(moved.getIntervalDays(), 14);
    QCOMPARE(moved.getRepetitions(), 5);
    QCOMPARE(moved.getNextReview(), next);
    QCOMPARE(moved.getLastReview(), last);
    QCOMPARE(moved.getDeckId(), 20);

    // copy теперь в перемещенном состоянии
    QCOMPARE(copy.getId(), 0);
    QCOMPARE(copy.getQuestion(), QString());
    QCOMPARE(copy.getIntervalDays(), 0);
    QCOMPARE(copy.getRepetitions(), 0);
    QCOMPARE(copy.getDeckId(), 0);

    // original остается неизменным
    QCOMPARE(original.getId(), 1);
    QCOMPARE(original.getQuestion(), "Original");
}

void TestCard::testMovePerformance() {
    // Arrange - создаем объект с большими строками
    QString longQuestion(10000, 'Q'); // 10К символов
    QString longAnswer(10000, 'A');   // 10К символов

    Card source(1, longQuestion, longAnswer,
                ContentType::Text, TestMode::DirectAnswer,
                1.5f, 7, 3, QDateTime(), QDateTime(), 10);

    // Act - перемещаем (должно быть быстро, без копирования строк)
    auto start = std::chrono::high_resolution_clock::now();
    Card destination = std::move(source);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    // Assert - данные переместились
    QCOMPARE(destination.getQuestion(), longQuestion);
    QCOMPARE(destination.getAnswer(), longAnswer);

    // Source должен быть пустым (или содержать пустые строки)
    QVERIFY(source.getQuestion().isEmpty());
    QVERIFY(source.getAnswer().isEmpty());

    // Можно залогировать время, но обычно не делают assertions на время
    qDebug() << "Move operation took:" << duration.count() << "microseconds";
}

void TestCard::testMoveThenUseSource() {
    // Arrange
    Card source(1, "Source Data", "Source Answer",
                ContentType::Audio, TestMode::Matching,
                2.5f, 30, 10, QDateTime(), QDateTime(), 50);

    // Act - перемещаем, затем используем источник
    Card destination = std::move(source);

    // Source теперь в перемещенном состоянии, но мы можем его использовать
    source.setId(100);
    source.setQuestion("New Question After Move");
    source.setAnswer("New Answer After Move");
    source.setEasyFactor(5.0f);
    source.setIntervalDays(100);
    source.setRepetitions(20);
    source.setDeckId(200);

    // Assert - destination не изменился
    QCOMPARE(destination.getId(), 1);
    QCOMPARE(destination.getQuestion(), "Source Data");
    QCOMPARE(destination.getAnswer(), "Source Answer");
    QVERIFY(qFuzzyCompare(destination.getEasyFactor(), 2.5f));
    QCOMPARE(destination.getIntervalDays(), 30);
    QCOMPARE(destination.getRepetitions(), 10);
    QCOMPARE(destination.getDeckId(), 50);

    // Source имеет новые значения
    QCOMPARE(source.getId(), 100);
    QCOMPARE(source.getQuestion(), "New Question After Move");
    QCOMPARE(source.getAnswer(), "New Answer After Move");
    QVERIFY(qFuzzyCompare(source.getEasyFactor(), 5.0f));
    QCOMPARE(source.getIntervalDays(), 100);
    QCOMPARE(source.getRepetitions(), 20);
    QCOMPARE(source.getDeckId(), 200);
}

// ==================== SETTERS ====================
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

// ==================== GETTERS ====================
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

// ==================== INTEGRATION TESTS ====================
void TestCard::testSetAndGetConsistency() {
    Card card;

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

    // Checking
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
