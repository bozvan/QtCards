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

Card::Card(Card&& other) noexcept :
    id(std::exchange(other.id, 0)),
    question(std::move(other.question)),
    answer(std::move(other.answer)),
    contentType(other.contentType),
    testMode(other.testMode),
    easyFactor(other.easyFactor),
    intervalDays(std::exchange(other.intervalDays, 0)),
    repetitions(std::exchange(other.repetitions, 0)),
    nextReview(std::move(other.nextReview)),
    lastReview(std::move(other.lastReview)),
    deckId(std::exchange(other.deckId, 0))
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

void Card::swap(Card& other) noexcept {
    using std::swap;
    swap(id, other.id);
    swap(question, other.question);
    swap(answer, other.answer);
    swap(contentType, other.contentType);
    swap(testMode, other.testMode);
    swap(easyFactor, other.easyFactor);
    swap(intervalDays, other.intervalDays);
    swap(repetitions, other.repetitions);
    swap(nextReview, other.nextReview);
    swap(lastReview, other.lastReview);
    swap(deckId, other.deckId);
}


Card& Card::operator=(const Card& other) {
    Card temp(other);
    swap(temp);
    return *this;
}

Card& Card::operator=(Card&& other) noexcept
{
    if (this != &other) {
        id = std::exchange(other.id, 0);
        question = std::move(other.question);
        answer = std::move(other.answer);
        contentType = other.contentType;
        testMode = other.testMode;
        easyFactor = other.easyFactor;
        intervalDays = std::exchange(other.intervalDays, 0);
        repetitions = std::exchange(other.repetitions, 0);
        nextReview = std::move(other.nextReview);
        lastReview = std::move(other.lastReview);
        deckId = std::exchange(other.deckId, 0);
    }
    return *this;
}
