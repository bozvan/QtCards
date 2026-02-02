#include "mainwindow.h"
#include "DatabaseManager.h"
#include <QDateTime>
#include <QApplication>
#include <QString>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DatabaseManager db;
    if (!db.open()) return -1;
    db.setUp();

    MainWindow w(db);
    w.show();

    //dbQuery.exec("INSERT INTO decks (id, name) VALUES (1, 'first_deck')");

    // QStringList tables = db.getTableList();
    // qDebug() << "Таблицы:";
    // for (int var = 0; var < tables.size(); ++var) {
    //     qDebug() << tables.at(var);
    // }
    QDateTime nextReview = QDateTime::currentDateTime().addDays(1);
    QDateTime lastReview = QDateTime::currentDateTime().addDays(-1);
    Card card1(1, "Q?", "A.", ContentType::Text, TestMode::DirectAnswer,
              2.5f, 7, 3, nextReview, lastReview, 10);
    db.insertCard(card1);
    qDebug() << "added";
    Card def;
    db.insertCard(def);
    qDebug() << "added";
    Card card2(1, "Qdddd?", "Afffff.", ContentType::Text, TestMode::DirectAnswer,
               2.0f, 5, 2, nextReview, lastReview, 7);
    db.insertCard(card2);
    qDebug() << "added";




    return a.exec();
}
