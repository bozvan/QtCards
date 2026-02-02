#pragma once
#include "ContentType.h"
#include "TestMode.h"
#include <QDateTime>
#include <QString>


class Card
{
public:
    Card();
    Card(int id, const QString &question, const QString &answer,
         ContentType contentType, TestMode testMode,
         float easyFactor, int intervalDays, int repetitions,
         const QDateTime &nextReview, const QDateTime &lastReview, int deckId);
    Card(const Card& other);
    Card(Card&& other) noexcept;
    ~Card() = default;

    // =============== GETTERS ===============

    int getId() const;
    QString getQuestion() const;
    QString getAnswer() const;
    ContentType getContentType() const;
    TestMode getTestMode() const;
    float getEasyFactor() const;
    int getIntervalDays() const;
    int getRepetitions() const;
    QDateTime getNextReview() const;
    QDateTime getLastReview() const;
    int getDeckId() const;

    // =============== SETTERS ===============

    void setId(int id);
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

    // =============== OTHERS ===============

    bool isDefault() const;
    void swap(Card& other) noexcept;
    void updateSM2(int grade);

    Card& operator=(const Card& other);
    Card& operator=(Card&& other) noexcept;
    bool operator==(const Card& other) const;
    bool operator!=(const Card& other) const;

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
    bool isDefaultCard;
};
