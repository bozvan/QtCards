#pragma once
#include "ContentType.h"
#include "TestMode.h"
#include <QDateTime>
#include <QString>

/**
 * @brief Класс, представляющий учебную карточку с системой интервального повторения
 *
 * Класс Card реализует карточку для системы интервального повторения по алгоритму SM2.
 * Каждая карточка содержит вопрос, ответ и метаданные для управления процессом повторения.
 *
 * @note Для корректной работы требует поддержки move semantics (правило пяти)
 * @see https://en.wikipedia.org/wiki/SuperMemo#Description_of_SM-2_algorithm
 *
 * @author bozvan
 * @version 1.0
 */
class Card
{
public:
    /**
     * @brief Конструктор по умолчанию
     *
     * Создает карточку с нулевыми значениями полей и пустыми строками.
     * easyFactor инициализируется значением 0.0f.
     */
    Card();

    /**
     * @brief Параметризированный конструктор
     *
     * Создает карточку с заданными значениями всех полей.
     *
     * @param id Уникальный идентификатор карточки
     * @param question Текст вопроса
     * @param answer Текст ответа
     * @param contentType Тип содержимого (текст/изображение)
     * @param testMode Режим тестирования
     * @param easyFactor Фактор легкости (Easy Factor) для алгоритма SM2
     * @param intervalDays Текущий интервал повторения в днях
     * @param repetitions Количество успешных повторений подряд
     * @param nextReview Дата следующего повторения
     * @param lastReview Дата последнего повторения
     * @param deckId Идентификатор колоды, к которой принадлежит карточка
     */
    Card(int id, const QString &question, const QString &answer,
         ContentType contentType, TestMode testMode,
         float easyFactor, int intervalDays, int repetitions,
         const QDateTime &nextReview, const QDateTime &lastReview, int deckId);

    /**
     * @brief Конструктор копирования
     * @param other Карточка для копирования
     */
    Card(const Card& other);

    /**
     * @brief Конструктор перемещения
     *
     * Перемещает ресурсы из другой карточки. Исходный объект остается
     * в допустимом, но неопределенном состоянии.
     *
     * @param other Карточка для перемещения (r-value reference)
     * @note noexcept гарантирует безопасность при использовании в контейнерах
     */
    Card(Card&& other) noexcept;

    /**
     * @brief Деструктор
     *
     * Деструктор по умолчанию, так как все члены имеют тривиальные деструкторы.
     */
    ~Card() = default;

    // =============== GETTERS ===============

    /**
     * @brief Получить идентификатор карточки
     * @return Уникальный идентификатор карточки
     */
    int getId();

    /**
     * @brief Получить текст вопроса
     * @return Текст вопроса
     */
    QString getQuestion();

    /**
     * @brief Получить текст ответа
     * @return Текст ответа
     */
    QString getAnswer();

    /**
     * @brief Получить тип содержимого
     * @return Тип содержимого (ContentType enum)
     */
    ContentType getContentType();

    /**
     * @brief Получить режим тестирования
     * @return Режим тестирования (TestMode enum)
     */
    TestMode getTestMode();

    /**
     * @brief Получить фактор легкости
     *
     * Easy Factor (EF) - множитель для расчета следующего интервала повторения
     * в алгоритме SM2. Диапазон значений: [1.3, 2.5].
     *
     * @return Текущее значение фактора легкости
     */
    float getEasyFactor();

    /**
     * @brief Получить текущий интервал повторения
     * @return Интервал повторения в днях
     */
    int getIntervalDays();

    /**
     * @brief Получить количество повторений
     *
     * Счетчик успешных повторений подряд. Сбрасывается в 0 при неудачном ответе.
     *
     * @return Количество успешных повторений подряд
     */
    int getRepetitions();

    /**
     * @brief Получить дату следующего повторения
     * @return Дата и время следующего запланированного повторения
     */
    QDateTime getNextReview();

    /**
     * @brief Получить дату последнего повторения
     * @return Дата и время последнего выполненного повторения
     */
    QDateTime getLastReview();

    /**
     * @brief Получить идентификатор колоды
     * @return Идентификатор колоды, к которой принадлежит карточка
     */
    int getDeckId();

    // =============== SETTERS ===============

    /**
     * @brief Установить идентификатор карточки
     * @param id Новый идентификатор карточки
     */
    void setId(int id);

    /**
     * @brief Установить текст вопроса
     * @param question Новый текст вопроса
     */
    void setQuestion(const QString &question);

    /**
     * @brief Установить текст ответа
     * @param answer Новый текст ответа
     */
    void setAnswer(const QString &answer);

    /**
     * @brief Установить тип содержимого
     * @param contentType Новый тип содержимого
     */
    void setContentType(ContentType contentType);

    /**
     * @brief Установить режим тестирования
     * @param testMode Новый режим тестирования
     */
    void setTestMode(TestMode testMode);

    /**
     * @brief Установить фактор легкости
     * @param easyFactor Новое значение фактора легкости
     * @warning Значение должно быть в диапазоне [1.3, 2.5] для корректной работы алгоритма SM2
     */
    void setEasyFactor(float easyFactor);

    /**
     * @brief Установить интервал повторения
     * @param intervalDays Новый интервал повторения в днях
     */
    void setIntervalDays(int intervalDays);

    /**
     * @brief Установить количество повторений
     * @param repetitions Новое количество повторений
     */
    void setRepetitions(int repetitions);

    /**
     * @brief Установить дату следующего повторения
     * @param nextReview Новая дата следующего повторения
     */
    void setNextReview(QDateTime nextReview);

    /**
     * @brief Установить дату последнего повторения
     * @param lastReview Новая дата последнего повторения
     */
    void setLastReview(QDateTime lastReview);

    /**
     * @brief Установить идентификатор колоды
     * @param deckId Новый идентификатор колоды
     */
    void setDeckId(int deckId);

    /**
     * @brief Обменять содержимое двух карточек
     *
     * Эффективный обмен содержимым между двумя объектами Card.
     * Используется в операторе присваивания копированием.
     *
     * @param other Карточка для обмена
     * @note noexcept гарантирует безопасность операции
     */
    void swap(Card& other) noexcept;

    /**
     * @brief Оператор присваивания копированием
     * @param other Карточка для копирования
     * @return Ссылка на текущий объект
     */
    Card& operator=(const Card& other);

    /**
     * @brief Оператор присваивания перемещением
     * @param other Карточка для перемещения
     * @return Ссылка на текущий объект
     * @note noexcept гарантирует безопасность операции
     */
    Card& operator=(Card&& other) noexcept;

    /**
     * @brief Обновить состояние карточки по алгоритму SM2
     *
     * Алгоритм SuperMemo 2 (SM2) для интервального повторения:
     * 1. При grade < 3: сброс счетчика повторений, интервал = 1 день
     * 2. При grade ≥ 3: увеличение интервала в зависимости от easyFactor
     * 3. Обновление easyFactor по формуле EF' = EF + (0.1 - (5 - quality) * (0.08 + (5 - quality) * 0.02))
     * 4. Установка nextReview = lastReview + intervalDays
     *
     * @param grade Оценка ответа (0-5)
     *              0: Полный провал (повторить немедленно)
     *              1: Очень плохо
     *              2: Плохо
     *              3: Удовлетворительно
     *              4: Хорошо
     *              5: Отлично (очень легко)
     *
     * @note Grade автоматически ограничивается в диапазоне [0, 5]
     * @note Easy Factor ограничивается в диапазоне [1.3, 2.5]
     *
     * @see https://www.supermemo.com/en/archives1990-2015/english/ol/sm2
     */
    void updateSM2(int grade);

private:
    int id;                     ///< Уникальный идентификатор карточки
    QString question;           ///< Текст вопроса
    QString answer;             ///< Текст ответа
    ContentType contentType;    ///< Тип содержимого
    TestMode testMode;          ///< Режим тестирования
    float easyFactor;           ///< Фактор легкости (1.3 - 2.5)
    int intervalDays;           ///< Текущий интервал повторения в днях
    int repetitions;            ///< Количество успешных повторений подряд
    QDateTime nextReview;       ///< Дата следующего повторения
    QDateTime lastReview;       ///< Дата последнего повторения
    int deckId;                 ///< Идентификатор колоды
};
