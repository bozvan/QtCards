#include "Deck.h"
#include <QDateTime>

Deck::Deck() : id(0), name(""), cards() {}

// =============== GETTERS IMPLEMENTATION ===============

int Deck::getId() const
{
    return id;
}

QString Deck::getName() const
{
    return name;
}

QList<Card> Deck::getCards() const
{
    return cards;
}

// =============== SETTERS IMPLEMENTATION ===============

void Deck::setId(int id)
{
    this->id = id;
}

void Deck::setName(QString name)
{
    this->name = name;
}


void Deck::setCards(QList<Card> cards)
{
    this->cards = cards;
}

// =============== OTHERS IMPLEMENTATION ===============

QList<Card> Deck::getDueCards() const
{
    QList<Card> dueCards;
    QDateTime now = QDateTime::currentDateTime();

    // Проход по всем карточкам в колоде
    for (const Card& card : cards) {
        QDateTime nextReview = card.getNextReview();

        // Условие включения карточки в список повторения
        if (!nextReview.isValid() || nextReview <= now) {
            dueCards.append(card);
        }
    }

    return dueCards;
}

int Deck::getDueCount() const
{
    int count = 0;
    QDateTime now = QDateTime::currentDateTime();

    // Подсчет карточек, готовых к повторению
    for (const Card& card : cards) {
        QDateTime nextReview = card.getNextReview();
        if (!nextReview.isValid() || nextReview <= now) {
            count++;
        }
    }

    return count;
}
