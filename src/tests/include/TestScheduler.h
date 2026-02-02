#pragma once

#include <QtTest/QtTest>
#include "Card.h"
#include "Deck.h"

/**
 * @brief Тестовый класс для проверки Scheduler
 *
 * Класс тестирует все статические методы класса Scheduler:
 * - Алгоритм SM2 (updateCard)
 * - Фильтрацию карточек (getDueCards, getDueCount, isCardDue)
 * - Вспомогательные расчеты (calculateNextInterval, calculateNextEasyFactor)
 *
 * @ingroup test_module
 */
class TestScheduler : public QObject
{
    Q_OBJECT

public:
    TestScheduler() = default;
    ~TestScheduler() = default;

private slots:
    // =============== ИНИЦИАЛИЗАЦИЯ И ОЧИСТКА ===============
    void initTestCase();      ///< Выполняется один раз перед всеми тестами
    void cleanupTestCase();   ///< Выполняется один раз после всех тестов
    void init();              ///< Выполняется перед каждым тестом
    void cleanup();           ///< Выполняется после каждого теста

    // =============== ТЕСТЫ ДЛЯ updateCard() ===============
    void testUpdateCardGradeBoundaries();     ///< Граничные значения оценки
    void testUpdateCardExcellentGrade();      ///< Оценка 5 (отлично)
    void testUpdateCardGoodGrade();           ///< Оценка 4 (хорошо)
    void testUpdateCardMediumGrade();         ///< Оценка 3 (удовлетворительно)
    void testUpdateCardPoorGrade();           ///< Оценка 2 (плохо)
    void testUpdateCardFailGrade();           ///< Оценка 1-0 (неудача)
    void testUpdateCardSequence();            ///< Последовательность повторений
    void testUpdateCardAfterFailure();        ///< Поведение после неудачи
    void testUpdateCardEasyFactorBounds();    ///< Границы Easy Factor
    void testUpdateCardTimestampUpdates();    ///< Обновление временных меток

    // =============== ТЕСТЫ ДЛЯ getDueCards() и getDueCount() ===============
    void testGetDueCardsEmptyDeck();          ///< Пустая колода
    void testGetDueCardsAllDue();             ///< Все карточки готовы
    void testGetDueCardsSomeDue();            ///< Часть карточек готова
    void testGetDueCardsNoneDue();            ///< Ни одна не готова
    void testGetDueCardsInvalidDates();       ///< Невалидные даты
    void testGetDueCardsConsistency();        ///< Согласованность методов
    void testGetDueCardsLargeDeck();          ///< Большая колода (производительность)

    // =============== ТЕСТЫ ДЛЯ isCardDue() ===============
    void testIsCardDuePastDate();             ///< Дата в прошлом
    void testIsCardDueCurrentDate();          ///< Текущая дата
    void testIsCardDueFutureDate();           ///< Дата в будущем
    void testIsCardDueInvalidDate();          ///< Невалидная дата
    void testIsCardDueExactTime();            ///< Точное время

    // =============== ТЕСТЫ ДЛЯ calculateNextInterval() ===============
    void testCalculateNextIntervalFirstSuccess();     ///< Первый успех
    void testCalculateNextIntervalSecondSuccess();    ///< Второй успех
    void testCalculateNextIntervalSubsequent();       ///< Последующие успехи
    void testCalculateNextIntervalFailure();          ///< Неудача
    void testCalculateNextIntervalEdgeCases();        ///< Крайние случаи

    // =============== ТЕСТЫ ДЛЯ calculateNextEasyFactor() ===============
    void testCalculateNextEasyFactorFormula();        ///< Проверка формулы
    void testCalculateNextEasyFactorBounds();         ///< Границы значений
    void testCalculateNextEasyFactorAllGrades();      ///< Все возможные оценки

    // =============== ИНТЕГРАЦИОННЫЕ ТЕСТЫ ===============
    void testIntegrationCompleteWorkflow();           ///< Полный рабочий процесс
    void testIntegrationMultipleCards();              ///< Несколько карточек
    void testIntegrationRealisticScenario();          ///< Реалистичный сценарий

    // =============== ТЕСТЫ ГРАНИЧНЫХ СЛУЧАЕВ ===============
    void testEdgeCasesMinimalValues();                ///< Минимальные значения
    void testEdgeCasesMaximalValues();                ///< Максимальные значения
    void testEdgeCasesInvalidInputs();                ///< Невалидные входные данные

    // =============== ТЕСТЫ ПРОИЗВОДИТЕЛЬНОСТИ ===============
    void testPerformanceUpdateCard();                 ///< Производительность updateCard
    void testPerformanceGetDueCards();                ///< Производительность getDueCards

private:
    /// Создать тестовую карточку с заданными параметрами
    Card createTestCard(int id, int daysFromNow) const;

    /// Создать тестовую колоду с заданным количеством карточек
    Deck createTestDeck(int cardCount, int duePercentage = 50) const;

    /// Проверить, что временные метки обновлены корректно
    void verifyTimestampUpdates(const Card& card, const QDateTime& beforeUpdate) const;
};
