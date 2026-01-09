#include "Deck.h"
#include <QDateTime>

/**
 * @brief Конструктор по умолчанию
 *
 * Инициализирует колоду с нулевым идентификатором, пустым названием
 * и пустым списком карточек.
 * Использует список инициализации членов для эффективности.
 */
Deck::Deck() : id(0), name(""), cards() {}

/**
 * @brief Получить идентификатор колоды
 * @return Уникальный идентификатор колоды
 */
int Deck::getId() const
{
    return id;
}

/**
 * @brief Получить название колоды
 * @return Название колоды
 */
QString Deck::getName() const
{
    return name;
}

/**
 * @brief Получить список всех карточек в колоде
 * @return QList<Card> Копия списка карточек
 */
QList<Card> Deck::getCards() const
{
    return cards;
}

/**
 * @brief Установить идентификатор колоды
 * @param id Новый идентификатор колоды
 */
void Deck::setId(int id)
{
    this->id = id;
}

/**
 * @brief Установить название колоды
 * @param name Новое название колоды
 */
void Deck::setName(QString name)
{
    this->name = name;
}

/**
 * @brief Установить список карточек
 * @param cards Новый список карточек для колоды
 */
void Deck::setCards(QList<Card> cards)
{
    this->cards = cards;
}

/**
 * @brief Получить карточки для повторения сегодня
 *
 * Алгоритм отбора карточек для повторения:
 * 1. Получает текущее системное время
 * 2. Для каждой карточки в колоде проверяет дату следующего повторения
 * 3. Если дата невалидна или наступила/прошла - добавляет карточку в результат
 *
 * Сложность алгоритма: O(n), где n - количество карточек в колоде
 *
 * @return QList<Card> Список карточек, требующих повторения
 *
 * @note Использует QDateTime::currentDateTime() для получения текущего времени
 * @note Карточки с невалидными датами (QDateTime()) считаются готовыми к повторению
 * @note Время сравнения включает дату и время, а не только дату
 *
 * @see QDateTime::isValid()
 * @see QDateTime::currentDateTime()
 */
QList<Card> Deck::getDueCards() const
{
    QList<Card> dueCards;
    QDateTime now = QDateTime::currentDateTime();

    // Проход по всем карточкам в колоде
    for (const Card& card : cards) {
        QDateTime nextReview = card.getNextReview();

        // Условие включения карточки в список повторения
        if (!nextReview.isValid() || nextReview <= now) {
            dueCards.append(card);
        }
    }

    return dueCards;
}

/**
 * @brief Получить количество карточек для повторения сегодня
 *
 * Оптимизированная версия getDueCards(), которая только подсчитывает
 * карточки без создания их копий в новом списке.
 *
 * Используется, когда нужен только счетчик, а не сами карточки.
 * Полезно для отображения статистики и уведомлений.
 *
 * Сложность алгоритма: O(n), где n - количество карточек в колоде
 *
 * @return int Количество карточек, готовых к повторению
 *
 * @note Всегда возвращает значение, равное getDueCards().size()
 * @note Гарантирует корректную работу с карточками, у которых nextReview не установлен
 */
int Deck::getDueCount() const
{
    int count = 0;
    QDateTime now = QDateTime::currentDateTime();

    // Подсчет карточек, готовых к повторению
    for (const Card& card : cards) {
        QDateTime nextReview = card.getNextReview();
        if (!nextReview.isValid() || nextReview <= now) {
            count++;
        }
    }

    return count;
}
