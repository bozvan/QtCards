#include "Card.h"
#include <QDateTime>
#include <cmath>
#include <algorithm>


Card::Card()
{
    id = 0;
    question = QString();
    answer = QString();
    easyFactor = 0.0f;
    intervalDays = 0;
    repetitions = 0;
    deckId = 0;
    isDefaultCard = true;
}

Card::Card(int id, const QString &question, const QString &answer,
           ContentType contentType, TestMode testMode,
           float easyFactor, int intervalDays, int repetitions,
           const QDateTime &nextReview, const QDateTime &lastReview, int deckId) :
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
    deckId(deckId),
    isDefaultCard(false)
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
    deckId(other.deckId),
    isDefaultCard(false)
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
    deckId(std::exchange(other.deckId, 0)),
    isDefaultCard(false)
{}

// =============== GETTERS IMPLEMENTATION ===============

int Card::getId() const { return id; }
QString Card::getQuestion() const { return question; }
QString Card::getAnswer() const { return answer; }
ContentType Card::getContentType() const { return contentType; }
TestMode Card::getTestMode() const { return testMode; }
float Card::getEasyFactor() const { return easyFactor; }
int Card::getIntervalDays() const { return intervalDays; }
int Card::getRepetitions() const { return repetitions; }
QDateTime Card::getNextReview() const { return nextReview; }
QDateTime Card::getLastReview() const { return lastReview; }
int Card::getDeckId() const { return deckId; }

// =============== SETTERS IMPLEMENTATION ===============

void Card::setId(int id) { this->id = id; }
void Card::setQuestion(const QString &question) { this->question = question; }
void Card::setAnswer(const QString &answer) { this->answer = answer; }
void Card::setContentType(ContentType contentType) { this->contentType = contentType; }
void Card::setTestMode(TestMode testMode) { this->testMode = testMode; }
void Card::setEasyFactor(float easyFactor) { this->easyFactor = easyFactor; }
void Card::setIntervalDays(int intervalDays) { this->intervalDays = intervalDays; }
void Card::setRepetitions(int repetitions) { this->repetitions = repetitions; }
void Card::setNextReview(QDateTime nextReview) { this->nextReview = nextReview; }
void Card::setLastReview(QDateTime lastReview) { this->lastReview = lastReview; }
void Card::setDeckId(int deckId) { this->deckId = deckId; }

// =============== OTHERS IMPLEMENTATION ===============

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

bool Card::isDefault() const
{
    return isDefaultCard;
}

void Card::updateSM2(int grade)
{
    grade = qBound(0, grade, 5);
    lastReview = QDateTime::currentDateTime();
    if (grade < 3) {
        repetitions = 0;
        intervalDays = 1;
    } else {
        if (repetitions == 0) {
            intervalDays = 1;
        } else if (repetitions == 1) {
            intervalDays = 6;
        } else {
            intervalDays = static_cast<int>(std::round(intervalDays * easyFactor));
        }
        repetitions++;
    }
    float quality = static_cast<float>(grade);
    float newEF = easyFactor + (0.1f - (5.0f - quality) * (0.08f + (5.0f - quality) * 0.02f));
    easyFactor = qBound(1.3f, newEF, 2.5f);
    nextReview = lastReview.addDays(intervalDays);
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

bool Card::operator==(const Card& other) const
{
    if (this->id == other.id && this->question == other.question && this->answer == other.answer &&
        this->contentType == other.contentType && this->testMode == other.testMode &&
        this->easyFactor == other.easyFactor && this->intervalDays == other.intervalDays &&
        this->repetitions == other.repetitions && this->nextReview == other.nextReview &&
        this->lastReview == other.lastReview && this->deckId == other.deckId)
    {
        return true;
    }
    return false;
}

bool Card::operator!=(const Card& other) const
{
    return !(*this==other);
}
