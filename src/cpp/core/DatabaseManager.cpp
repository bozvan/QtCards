#include "DatabaseManager.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDir>
#include <QString>
#include <QStandardPaths>


DatabaseManager::DatabaseManager()
{
    QDir dir;
    dir.mkdir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    DB_PATH = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/QtCards.sqlite";
    dbase = QSqlDatabase::addDatabase("QSQLITE");
    dbase.setDatabaseName(DB_PATH);
    QSqlQuery _dbQuery(dbase);
    dbQuery = std::move(_dbQuery);

    qDebug() << "DatabaseManager constructor!";
}

void DatabaseManager::setUp()
{
    if (!dbQuery.exec("CREATE TABLE IF NOT EXISTS decks "
                      "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "name TEXT NOT NULL, "
                      "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP);"))
    {
        qDebug() << "Table decks is not created!" << dbQuery.lastError().text();
    }
    if (!dbQuery.exec("CREATE TABLE IF NOT EXISTS cards "
                      "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "deck_id INTEGER, "
                      "question TEXT NOT NULL, "
                      "answer TEXT NOT NULL, "
                      "easy_factor REAL DEFAULT 2.5, "
                      "interval_days INTEGER DEFAULT 0, "
                      "repetitions INTEGER DEFAULT 0, "
                      "next_review TIMESTAMP, "
                      "last_review TIMESTAMP, "
                      "content_type INTEGER DEFAULT 0, "
                      "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, "
                      "FOREIGN KEY (deck_id) REFERENCES decks(id));"))
    {
        qDebug() << "Table cards is not created!" << dbQuery.lastError().text();
    }

    deckRowCount = getDeckRowCount();
    cardRowCount = getCardRowCount();
}

bool DatabaseManager::open()
{
    if (!dbase.open())
    {
        qDebug() << "Что-то не так!" << dbase.lastError().text();
        return false;
    }
    qDebug() << "БД открыта!";
    return true;
}

QString DatabaseManager::getDbPath() const { return DB_PATH; }
QStringList DatabaseManager::getTableList() const { return dbase.tables(QSql::Tables); }
uint32_t DatabaseManager::getDeckRowCount()
{
    dbQuery.prepare("SELECT * FROM decks;");
    dbQuery.exec();
    return dbQuery.size();
}
uint32_t DatabaseManager::getCardRowCount()
{
    dbQuery.prepare("SELECT * FROM cards;");
    dbQuery.exec();
    return dbQuery.size();
}

bool DatabaseManager::insertDeck(QString name="")
{
    if (name == "") return false;
    QString query = QString("INSERT INTO decks (name) VALUES (?)");
    dbQuery.prepare(query);
    dbQuery.addBindValue(name);

    if (!dbQuery.exec())
    {
        qDebug() << "Insert error:" << dbQuery.lastError();
        qDebug() << "SQL QUERY:" << query;
        return false;
    }
    deckRowCount++;
    return true;
}

bool DatabaseManager::insertCard(Card &card)
{
    if (card.isDefault()) return false;
    QString query = QString("INSERT INTO cards "
                            "(deck_id, question, answer, easy_factor, interval_days, repetitions, next_review, last_review) "
                            "VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
    dbQuery.prepare(query);
    dbQuery.addBindValue(card.getDeckId());
    dbQuery.addBindValue(card.getQuestion());
    dbQuery.addBindValue(card.getAnswer());
    dbQuery.addBindValue(card.getEasyFactor());
    dbQuery.addBindValue(card.getIntervalDays());
    dbQuery.addBindValue(card.getRepetitions());
    dbQuery.addBindValue(card.getNextReview());
    dbQuery.addBindValue(card.getLastReview());

    if (!dbQuery.exec())
    {
        qDebug() << "Insert error:" << dbQuery.lastError();
        qDebug() << "SQL QUERY:" << query;
        return false;
    }
    cardRowCount++;
    return true;
}


