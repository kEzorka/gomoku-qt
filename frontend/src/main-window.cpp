#include "../include/main-window.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    // Создаём контейнер для переключения между меню и игровым полем
    centralStack = new QStackedWidget(this);
    setCentralWidget(centralStack);

    // Создаём стартовое меню
    menuWidget = new MenuWidget(this);
    gameBoardWidget = nullptr; // Игровое поле будет создано при запуске игры

    centralStack->addWidget(menuWidget);
    // Подписываемся на сигнал запуска игры из меню
    connect(menuWidget, &MenuWidget::startGameRequested, this, &MainWindow::startGame);
}

void MainWindow::startGame(bool playerVsBot, int difficulty)
{
    // Если уже существует игровое поле — удаляем его и создаём новое
    if (gameBoardWidget) {
        centralStack->removeWidget(gameBoardWidget);
        gameBoardWidget->deleteLater();
    }
    gameBoardWidget = new GameBoardWidget(playerVsBot, difficulty, this);
    centralStack->addWidget(gameBoardWidget);
    centralStack->setCurrentWidget(gameBoardWidget);
    // Подписываемся на сигнал возврата в меню
    connect(gameBoardWidget, &GameBoardWidget::returnToMenu, this, &MainWindow::returnToMenu);
}

void MainWindow::returnToMenu()
{
    // Сбрасываем игровое поле и возвращаемся в меню
    if (gameBoardWidget) {
        centralStack->removeWidget(gameBoardWidget);
        gameBoardWidget->deleteLater();
        gameBoardWidget = nullptr;
    }
    centralStack->setCurrentWidget(menuWidget);
}
