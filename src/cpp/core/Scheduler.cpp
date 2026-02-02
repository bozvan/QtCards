#include "Scheduler.h"
#include <QDateTime>
#include <algorithm>
#include <cmath>

void Scheduler::updateCard(Card& card, int grade) noexcept
{
    grade = std::clamp(grade, 0, 5); // ограничение

    card.setLastReview(QDateTime::currentDateTime());
    int currentInterval = card.getIntervalDays();
    int currentRepetitions = card.getRepetitions();
    float currentEasyFactor = card.getEasyFactor();

    int newInterval = 1;
    int newRepetitions = currentRepetitions;

    if (grade < 3) {
        // Неудачный ответ - сброс прогресса
        newRepetitions = 0;
        newInterval = 1;
    } else {
        // Успешный ответ
        if (currentRepetitions == 0) {
            newInterval = 1;           // Первое успешное повторение
        } else if (currentRepetitions == 1) {
            newInterval = 6;           // Второе успешное повторение
        } else {
            newInterval = static_cast<int>(std::round(currentInterval * currentEasyFactor));
        }
        newRepetitions = currentRepetitions + 1;
    }

    float newEasyFactor = calculateNextEasyFactor(currentEasyFactor, grade);

    card.setIntervalDays(newInterval);
    card.setRepetitions(newRepetitions);
    card.setEasyFactor(newEasyFactor);
    card.setNextReview(card.getLastReview().addDays(newInterval));
}

QList<Card> Scheduler::getDueCards(const Deck& deck) noexcept
{
    QList<Card> dueCards;
    QList<Card> allCards = deck.getCards();

    // Резервируем память для оптимизации (предполагаем, что большинство карточек не готово)
    dueCards.reserve(allCards.size() / 4);
    std::copy_if(allCards.begin(), allCards.end(), std::back_inserter(dueCards),
                 [](const Card& card) {
                     return isCardDue(card);
                 });

    return dueCards;
}

int Scheduler::getDueCount(const Deck& deck) noexcept
{
    QList<Card> allCards = deck.getCards();

    return std::count_if(allCards.begin(), allCards.end(),
                         [](const Card& card) {
                             return isCardDue(card);
                         });
}

bool Scheduler::isCardDue(const Card& card) noexcept
{
    QDateTime nextReview = card.getNextReview();
    QDateTime now = QDateTime::currentDateTime();
    // Невалидная дата считается "готовой к повторению"
    if (!nextReview.isValid()) {
        return true;
    }
    return nextReview <= now;
}

int Scheduler::calculateNextInterval(int currentInterval, int repetitions,
                                     float easyFactor, int grade) noexcept
{
    grade = std::clamp(grade, 0, 5);
    if (grade < 3) {
        return 1;
    }
    if (repetitions == 0) {
        return 1;
    } else if (repetitions == 1) {
        return 6;
    } else {
        return static_cast<int>(std::round(currentInterval * easyFactor));
    }
}

float Scheduler::calculateNextEasyFactor(float currentEasyFactor, int grade) noexcept
{
    grade = std::clamp(grade, 0, 5);
    float quality = static_cast<float>(grade);
    float newEF = currentEasyFactor + (0.1f - (5.0f - quality) *
                                                  (0.08f + (5.0f - quality) * 0.02f));

    return std::clamp(newEF, 1.3f, 2.5f);
}
