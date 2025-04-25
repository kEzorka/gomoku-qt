#pragma once
/*
 * mainwindow.h
 *
 * Заголовочный файл для класса MainWindow, основного окна приложения.
 * Он использует QStackedWidget для переключения между стартовым меню и игровым полем.
 */

#include <QMainWindow>
#include <QStackedWidget>
#include "menu-widget.h"
#include "game-board-widget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    // Конструктор основного окна
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    /**
     * @brief startGame Запускает новую игру с заданными параметрами.
     * @param playerVsBot true, если выбран режим "Игрок против Бота", иначе "Бот против Бота".
     * @param difficulty Глубина поиска, определяющая уровень сложности.
     */
    void startGame(bool playerVsBot, int difficulty);

    /**
     * @brief returnToMenu Переход к главному меню (сброс текущей игры).
     */
    void returnToMenu();

private:
    QStackedWidget* centralStack; // Контейнер для переключения между экранами меню и игры.
    MenuWidget* menuWidget;       // Стартовый экран (меню).
    GameBoardWidget* gameBoardWidget; // Игровое поле (создаётся динамически при запуске игры).
};
