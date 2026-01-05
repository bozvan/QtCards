#include "Card.h"


Card::Card()
{
    id = 0;
    question = QString();
    answer = QString();
    easyFactor = 0.0f;
    intervalDays = 0;
    repetitions = 0;
    deckId = 0;
}

Card::Card(int id, const QString &question, const QString &answer, ContentType contentType, TestMode testMode, float easyFactor, int intervalDays, int repetitions, const QDateTime &nextReview, const QDateTime &lastReview, int deckId) :
    id(id),
    question(question),
    answer(answer),
    contentType(contentType),
    testMode(testMode),
    easyFactor(easyFactor),
    intervalDays(intervalDays),
    repetitions(repetitions),
    nextReview(nextReview),
    lastReview(lastReview),
    deckId(deckId)
{}

Card::Card(const Card& other) :
    id(other.id),
    question(other.question),
    answer(other.answer),
    contentType(other.contentType),
    testMode(other.testMode),
    easyFactor(other.easyFactor),
    intervalDays(other.intervalDays),
    repetitions(other.repetitions),
    nextReview(other.nextReview),
    lastReview(other.lastReview),
    deckId(other.deckId)
{}

int Card::getId()
{
    return id;
}

QString Card::getQuestion()
{
    return question;
}

QString Card::getAnswer()
{
    return answer;
}

ContentType Card::getContentType()
{
    return contentType;
}

TestMode Card::getTestMode()
{
    return testMode;
}

float Card::getEasyFactor()
{
    return easyFactor;
}

int Card::getIntervalDays()
{
    return intervalDays;
}

int Card::getRepetitions()
{
    return repetitions;
}

QDateTime Card::getNextReview()
{
    return nextReview;
}

QDateTime Card::getLastReview()
{
    return lastReview;
}

int Card::getDeckId()
{
    return deckId;
}

void Card::setId(int id)
{
    this->id = id;
}

void Card::setQuestion(const QString &question)
{
    this->question = question;
}

void Card::setAnswer(const QString &answer)
{
    this->answer = answer;
}

void Card::setContentType(ContentType contentType)
{
    this->contentType = contentType;
}

void Card::setTestMode(TestMode testMode)
{
    this->testMode = testMode;
}

void Card::setEasyFactor(float easyFactor)
{
    this->easyFactor = easyFactor;
}

void Card::setIntervalDays(int intervalDays)
{
    this->intervalDays = intervalDays;
}

void Card::setRepetitions(int repetitions)
{
    this->repetitions = repetitions;
}

void Card::setNextReview(QDateTime nextReview)
{
    this->nextReview = nextReview;
}

void Card::setLastReview(QDateTime lastReview)
{
    this->lastReview = lastReview;
}

void Card::setDeckId(int deckId)
{
    this->deckId = deckId;
}
