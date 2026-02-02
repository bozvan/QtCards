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

    // =============== GETTERS ===============

    int getId() const;
    QString getName() const;
    QList<Card> getCards() const;

    // =============== SETTERS ===============

    void setId(int id);
    void setName(QString name);
    void setCards(QList<Card> cards);

    // =============== OTHERS ===============

    QList<Card> getDueCards() const;
    int getDueCount() const;
};
