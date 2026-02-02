#pragma once
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QString>
#include <QStandardPaths>
#include "Card.h"


class DatabaseManager
{
    enum TableName
    {
        Decks,
        Cards
    };

public:
    DatabaseManager();
    ~DatabaseManager() = default;
    void setUp();
    bool open();
    QString getDbPath() const;
    QStringList getTableList() const;
    uint32_t getDeckRowCount();
    uint32_t getCardRowCount();

    bool insertDeck(QString name);
    bool insertCard(Card &card);
private:
    QString DB_PATH;
    QSqlDatabase dbase;
    QSqlQuery dbQuery;
    uint32_t deckRowCount;
    uint32_t cardRowCount;
};
