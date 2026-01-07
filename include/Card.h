#pragma once
#include "ContentType.h"
#include "TestMode.h"
#include <QDateTime>
#include <QString>

/**
 * @brief The Card class
 * @details
 * The class, that describe simple card object
 * @author bozvan
 */
class Card
{
public:
    Card();
    Card(int id, const QString &question, const QString &answer, ContentType contentType, TestMode testMode, float easyFactor, int intervalDays, int repetitions, const QDateTime &nextReview, const QDateTime &lastReview, int deckId);
    Card(const Card& other);
    Card(Card&& other) noexcept;
    ~Card() = default;

    // GETTERS
    int getId();
    QString getQuestion();
    QString getAnswer();
    ContentType getContentType();
    TestMode getTestMode();
    float getEasyFactor();
    int getIntervalDays();
    int getRepetitions();
    QDateTime getNextReview();
    QDateTime getLastReview();
    int getDeckId();

    // SETTERS
    void setId(int _id);
    void setQuestion(const QString &question);
    void setAnswer(const QString &answer);
    void setContentType(ContentType contentType);
    void setTestMode(TestMode testMode);
    void setEasyFactor(float easyFactor);
    void setIntervalDays(int intervalDays);
    void setRepetitions(int repetitions);
    void setNextReview(QDateTime nextReview);
    void setLastReview(QDateTime lastReview);
    void setDeckId(int deckId);

    void swap(Card& other) noexcept;
    Card& operator=(const Card& other);
    Card& operator=(Card&& other) noexcept;

private:
    int id;
    QString question;
    QString answer;
    ContentType contentType;
    TestMode testMode;
    float easyFactor;
    int intervalDays;
    int repetitions;
    QDateTime nextReview;
    QDateTime lastReview;
    int deckId;
};
