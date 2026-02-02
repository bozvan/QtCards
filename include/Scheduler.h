#pragma once

#include <QList>
#include <QDateTime>
#include "Card.h"
#include "Deck.h"

class Scheduler
{
public:
    static void updateCard(Card& card, int grade) noexcept;
    static QList<Card> getDueCards(const Deck& deck) noexcept;
    static int getDueCount(const Deck& deck) noexcept;
    static bool isCardDue(const Card& card) noexcept;
    static int calculateNextInterval(int currentInterval, int repetitions,
                                     float easyFactor, int grade) noexcept;
    static float calculateNextEasyFactor(float currentEasyFactor, int grade) noexcept;
};

