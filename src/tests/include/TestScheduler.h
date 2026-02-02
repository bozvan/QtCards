#pragma once

#include <QtTest/QtTest>
#include "Card.h"
#include "Deck.h"


class TestScheduler : public QObject
{
    Q_OBJECT

public:
    TestScheduler() = default;
    ~TestScheduler() = default;

private slots:
    // =============== ИНИЦИАЛИЗАЦИЯ И ОЧИСТКА ===============
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // =============== ТЕСТЫ ДЛЯ updateCard() ===============
    void testUpdateCardGradeBoundaries();
    void testUpdateCardExcellentGrade();
    void testUpdateCardGoodGrade();
    void testUpdateCardMediumGrade();
    void testUpdateCardPoorGrade();
    void testUpdateCardFailGrade();
    void testUpdateCardSequence();
    void testUpdateCardAfterFailure();
    void testUpdateCardEasyFactorBounds();
    void testUpdateCardTimestampUpdates();

    // =============== ТЕСТЫ ДЛЯ getDueCards() и getDueCount() ===============
    void testGetDueCardsEmptyDeck();
    void testGetDueCardsAllDue();
    void testGetDueCardsSomeDue();
    void testGetDueCardsNoneDue();
    void testGetDueCardsInvalidDates();
    void testGetDueCardsConsistency();
    void testGetDueCardsLargeDeck();

    // =============== ТЕСТЫ ДЛЯ isCardDue() ===============
    void testIsCardDuePastDate();
    void testIsCardDueCurrentDate();
    void testIsCardDueFutureDate();
    void testIsCardDueInvalidDate();
    void testIsCardDueExactTime();

    // =============== ТЕСТЫ ДЛЯ calculateNextInterval() ===============
    void testCalculateNextIntervalFirstSuccess();
    void testCalculateNextIntervalSecondSuccess();
    void testCalculateNextIntervalSubsequent();
    void testCalculateNextIntervalFailure();
    void testCalculateNextIntervalEdgeCases();

    // =============== ТЕСТЫ ДЛЯ calculateNextEasyFactor() ===============
    void testCalculateNextEasyFactorFormula();
    void testCalculateNextEasyFactorBounds();
    void testCalculateNextEasyFactorAllGrades();

    // =============== ИНТЕГРАЦИОННЫЕ ТЕСТЫ ===============
    void testIntegrationCompleteWorkflow();
    void testIntegrationMultipleCards();
    void testIntegrationRealisticScenario();

    // =============== ТЕСТЫ ГРАНИЧНЫХ СЛУЧАЕВ ===============
    void testEdgeCasesMinimalValues();
    void testEdgeCasesMaximalValues();
    void testEdgeCasesInvalidInputs();

    // =============== ТЕСТЫ ПРОИЗВОДИТЕЛЬНОСТИ ===============
    void testPerformanceUpdateCard();
    void testPerformanceGetDueCards();

private:
    Card createTestCard(int id, int daysFromNow) const;
    Deck createTestDeck(int cardCount, int duePercentage = 50) const;
    void verifyTimestampUpdates(const Card& card, const QDateTime& beforeUpdate) const;
};
