#include "Scheduler.h"
#include <QDateTime>
#include <algorithm>
#include <cmath>

/**
 * @brief Обновить карточку по алгоритму SM2 (SuperMemo 2)
 *
 * Детальная реализация алгоритма SuperMemo 2:
 *
 * 1. Валидация входных данных:
 *    - Grade ограничивается диапазоном [0, 5]
 *
 * 2. Обновление метаданных:
 *    - lastReview устанавливается в текущее время
 *
 * 3. Обработка неудачного ответа (grade < 3):
 *    - repetitions сбрасывается в 0
 *    - intervalDays устанавливается в 1 день
 *
 * 4. Обработка успешного ответа (grade ≥ 3):
 *    - Если repetitions == 0: intervalDays = 1 день
 *    - Если repetitions == 1: intervalDays = 6 дней
 *    - Иначе: intervalDays = round(intervalDays × easyFactor)
 *    - repetitions увеличивается на 1
 *
 * 5. Обновление Easy Factor:
 *    - Формула: EF' = EF + (0.1 - (5 - quality) × (0.08 + (5 - quality) × 0.02))
 *    - где quality = grade
 *    - Ограничение: EF ∈ [1.3, 2.5]
 *
 * 6. Установка nextReview:
 *    - nextReview = lastReview + intervalDays
 */
void Scheduler::updateCard(Card& card, int grade) noexcept
{
    // Шаг 1: Валидация и подготовка
    grade = std::clamp(grade, 0, 5);

    // Шаг 2: Обновление времени последнего повторения
    card.setLastReview(QDateTime::currentDateTime());

    // Шаг 3-4: Определение нового интервала
    int currentInterval = card.getIntervalDays();
    int currentRepetitions = card.getRepetitions();
    float currentEasyFactor = card.getEasyFactor();

    int newInterval = 1;
    int newRepetitions = currentRepetitions;

    if (grade < 3) {
        // Неудачный ответ - сброс прогресса
        newRepetitions = 0;
        newInterval = 1;
    } else {
        // Успешный ответ
        if (currentRepetitions == 0) {
            newInterval = 1;           // Первое успешное повторение
        } else if (currentRepetitions == 1) {
            newInterval = 6;           // Второе успешное повторение
        } else {
            // Последующие повторения: умножение на easyFactor
            newInterval = static_cast<int>(std::round(currentInterval * currentEasyFactor));
        }
        newRepetitions = currentRepetitions + 1;
    }

    // Шаг 5: Обновление фактора легкости
    float newEasyFactor = calculateNextEasyFactor(currentEasyFactor, grade);

    // Шаг 6: Применение изменений к карточке
    card.setIntervalDays(newInterval);
    card.setRepetitions(newRepetitions);
    card.setEasyFactor(newEasyFactor);
    card.setNextReview(card.getLastReview().addDays(newInterval));
}

/**
 * @brief Получить карточки, готовые к повторению
 *
 * Алгоритм фильтрации:
 * 1. Получает текущее системное время
 * 2. Проходит по всем карточкам в колоде
 * 3. Для каждой карточки проверяет isCardDue()
 * 4. Добавляет подходящие карточки в результирующий список
 */
QList<Card> Scheduler::getDueCards(const Deck& deck) noexcept
{
    QList<Card> dueCards;
    QList<Card> allCards = deck.getCards();

    // Резервируем память для оптимизации (предполагаем, что большинство карточек не готово)
    dueCards.reserve(allCards.size() / 4);

    // Фильтрация карточек
    std::copy_if(allCards.begin(), allCards.end(), std::back_inserter(dueCards),
                 [](const Card& card) {
                     return isCardDue(card);
                 });

    return dueCards;
}

/**
 * @brief Получить количество карточек для повторения
 *
 * Оптимизированная версия, которая только подсчитывает карточки
 * без создания их копий в памяти.
 */
int Scheduler::getDueCount(const Deck& deck) noexcept
{
    QList<Card> allCards = deck.getCards();

    return std::count_if(allCards.begin(), allCards.end(),
                         [](const Card& card) {
                             return isCardDue(card);
                         });
}

/**
 * @brief Проверить, готова ли карточка к повторению
 *
 * Детальная логика проверки:
 * 1. Получает дату следующего повторения карточки
 * 2. Если дата невалидна - карточка готова к повторению
 * 3. Если дата валидна и наступила/прошла - карточка готова
 * 4. Иначе - карточка не готова
 */
bool Scheduler::isCardDue(const Card& card) noexcept
{
    QDateTime nextReview = card.getNextReview();
    QDateTime now = QDateTime::currentDateTime();

    // Невалидная дата считается "готовой к повторению"
    if (!nextReview.isValid()) {
        return true;
    }

    // Сравнение дат (включая время)
    return nextReview <= now;
}

/**
 * @brief Получить следующий интервал повторения по алгоритму SM2
 *
 * Расчетная логика интервалов:
 * - grade < 3: всегда 1 день (неудачный ответ)
 * - repetitions == 0: 1 день (первый успешный ответ после сброса)
 * - repetitions == 1: 6 дней (стандартное второе повторение)
 * - repetitions > 1: currentInterval × easyFactor (с округлением)
 */
int Scheduler::calculateNextInterval(int currentInterval, int repetitions,
                                     float easyFactor, int grade) noexcept
{
    grade = std::clamp(grade, 0, 5);

    if (grade < 3) {
        return 1;  // Неудачный ответ
    }

    if (repetitions == 0) {
        return 1;  // Первый успешный ответ
    } else if (repetitions == 1) {
        return 6;  // Второй успешный ответ
    } else {
        // Последующие успешные ответы
        return static_cast<int>(std::round(currentInterval * easyFactor));
    }
}

/**
 * @brief Рассчитать новый Easy Factor по алгоритму SM2
 *
 * Формула SM2 для Easy Factor:
 * EF' = EF + (0.1 - (5 - quality) × (0.08 + (5 - quality) × 0.02))
 * где quality = grade (0-5)
 *
 * Ограничения:
 * - Минимальное значение: 1.3
 * - Максимальное значение: 2.5
 */
float Scheduler::calculateNextEasyFactor(float currentEasyFactor, int grade) noexcept
{
    grade = std::clamp(grade, 0, 5);

    // Формула SM2 для обновления Easy Factor
    float quality = static_cast<float>(grade);
    float newEF = currentEasyFactor + (0.1f - (5.0f - quality) *
                                                  (0.08f + (5.0f - quality) * 0.02f));

    // Ограничение результата в диапазоне [1.3, 2.5]
    return std::clamp(newEF, 1.3f, 2.5f);
}
