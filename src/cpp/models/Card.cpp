#include "Card.h"
#include <QDateTime>
#include <cmath>
#include <algorithm>

/**
 * @brief Конструктор по умолчанию
 *
 * Инициализирует все поля нулевыми значениями или значениями по умолчанию.
 * easyFactor устанавливается в 0.0f (невалидное значение для SM2).
 */
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

/**
 * @brief Параметризированный конструктор
 *
 * Инициализирует все поля переданными значениями.
 * Использует список инициализации членов для эффективности.
 */
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
    deckId(deckId)
{}

/**
 * @brief Конструктор копирования
 *
 * Создает глубокую копию всех полей объекта other.
 * Для строк используется конструктор копирования QString.
 */
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

/**
 * @brief Конструктор перемещения
 *
 * Перемещает ресурсы из объекта other. Для строк используется семантика перемещения,
 * для примитивных типов - std::exchange с обнулением исходных значений.
 *
 * @note После перемещения объект other остается в допустимом состоянии
 *       с нулевыми значениями числовых полей и пустыми строками.
 */
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

/**
 * @brief Обменять содержимое двух карточек
 *
 * Использует std::swap для эффективного обмена значениями всех полей.
 * Гарантирует строгую гарантию безопасности исключений.
 */
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

/**
 * @brief Оператор присваивания копированием
 *
 * Реализует copy-and-swap идиому для гарантии строгой безопасности исключений.
 * Создает временную копию, обменивает с текущим объектом и возвращает ссылку.
 */
Card& Card::operator=(const Card& other) {
    Card temp(other);
    swap(temp);
    return *this;
}

/**
 * @brief Оператор присваивания перемещением
 *
 * Перемещает ресурсы из объекта other, обнуляя его числовые поля.
 * Проверяет на самоприсваивание для корректной работы.
 */
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

/**
 * @brief Обновить состояние карточки по алгоритму SM2
 *
 * Детальная реализация алгоритма SuperMemo 2:
 *
 * 1. **Ограничение оценки**: grade ограничивается диапазоном [0, 5]
 * 2. **Обновление времени**: lastReview устанавливается в текущее время
 * 3. **Обработка неудачного ответа (grade < 3)**:
 *    - repetitions сбрасывается в 0
 *    - intervalDays устанавливается в 1 день
 * 4. **Обработка успешного ответа (grade ≥ 3)**:
 *    - Если repetitions == 0: intervalDays = 1 день
 *    - Если repetitions == 1: intervalDays = 6 дней
 *    - Иначе: intervalDays = round(intervalDays × easyFactor)
 *    - repetitions увеличивается на 1
 * 5. **Обновление easyFactor**:
 *    - Формула: EF' = EF + (0.1 - (5 - quality) × (0.08 + (5 - quality) × 0.02))
 *    - где quality = grade
 *    - Ограничение: EF ∈ [1.3, 2.5]
 * 6. **Установка nextReview**: lastReview + intervalDays
 *
 * @param grade Оценка ответа пользователя (0-5)
 *
 * @note Использует qBound для ограничения grade
 * @note Использует std::round для корректного округления интервала
 * @note Формула easyFactor соответствует оригинальному алгоритму SM2
 */
void Card::updateSM2(int grade)
{
    // Шаг 1: Ограничение оценки в допустимом диапазоне
    grade = qBound(0, grade, 5);

    // Шаг 2: Обновление времени последнего повторения
    lastReview = QDateTime::currentDateTime();

    // Шаг 3-4: Определение нового интервала в зависимости от оценки
    if (grade < 3) {
        // Неудачный ответ - сброс прогресса
        repetitions = 0;
        intervalDays = 1;
    } else {
        // Успешный ответ - увеличение интервала
        if (repetitions == 0) {
            intervalDays = 1;           // Первое успешное повторение
        } else if (repetitions == 1) {
            intervalDays = 6;           // Второе успешное повторение
        } else {
            // Последующие повторения: умножение на easyFactor
            intervalDays = static_cast<int>(std::round(intervalDays * easyFactor));
        }
        repetitions++;  // Увеличение счетчика успешных повторений
    }

    // Шаг 5: Обновление фактора легкости по формуле SM2
    float quality = static_cast<float>(grade);
    float newEF = easyFactor + (0.1f - (5.0f - quality) * (0.08f + (5.0f - quality) * 0.02f));

    // Ограничение easyFactor в диапазоне [1.3, 2.5]
    if (newEF < 1.3f) {
        newEF = 1.3f;   // Нижняя граница
    } else if (newEF > 2.5f) {
        newEF = 2.5f;   // Верхняя граница
    }
    easyFactor = newEF;

    // Шаг 6: Установка даты следующего повторения
    nextReview = lastReview.addDays(intervalDays);
}
