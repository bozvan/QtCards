#pragma once
#include <QString>
#include <QList>
#include "Card.h"

class Deck {
private:
    int id;
    QString name;
    QList<Card> cards;

public:
    Deck();
    ~Deck() = default;

    int getId();
    QString getName();
    QList<Card> getCards();

    void setId(int id);
    void setName(QString name);
    void setCards(QList<Card> cards);

    QList<Card> getDueCards() const;  // Карточки для повторения сегодня
    int getDueCount() const;          // Сколько карточек к повторению
};
