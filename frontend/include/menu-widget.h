#pragma once
/*
 * menuwidget.h
 *
 * Заголовок класса MenuWidget – стартового меню игры.
 * Здесь пользователь выбирает режим (Игрок против Бота или Бот против Бота)
 * и уровень сложности, задаваемый глубиной поиска.
 */

#include <QWidget>
#include <QPushButton>
#include <QComboBox>
#include <QRadioButton>
#include <QLabel>
#include <QVBoxLayout>

class MenuWidget : public QWidget {
    Q_OBJECT
public:
    // Конструктор стартового меню
    explicit MenuWidget(QWidget* parent = nullptr);

signals:
    /**
     * @brief startGameRequested Сигнал, генерируемый при нажатии кнопки "Начать игру".
     * @param playerVsBot true, если выбран режим "Игрок против Бота", иначе "Бот против Бота".
     * @param difficulty Глубина поиска (2 – лёгкий, 3 – средний, 4 – трудный).
     */
    void startGameRequested(bool playerVsBot, int difficulty);

private slots:
    /// Обработчик нажатия кнопки "Начать игру"
    void onStartButtonClicked();

private:
    QComboBox* difficultyCombo; // Выпадающий список для выбора уровня сложности.
    QRadioButton* rbPlayerVsBot; // Радиокнопка для режима "Игрок против Бота".
    QRadioButton* rbBotVsBot;    // Радиокнопка для режима "Бот против Бота".
    QPushButton* btnStart;       // Кнопка для запуска игры.
};
