#include "Deck.h"

Deck::Deck() : id(0), name(""), cards() {}

int Deck::getId()
{
    return id;
}

QString Deck::getName()
{
    return name;
}

QList<Card> Deck::getCards()
{
    return cards;
}

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

// QList<Card> Deck::getDueCards() const
// {

// }

// int Deck::getDueCount() const
// {

// }
