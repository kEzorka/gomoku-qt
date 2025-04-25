#pragma once
/*
 * gameboardwidget.h
 *
 * Заголовочный файл для класса GameBoardWidget,
 * который реализует игровое поле: отрисовку доски, обработку ходов игрока и ИИ,
 * сохранение/отмену ходов, работу подсказок и переход в меню.
 *
 * В этой версии введена переменная currentTurn, которая определяет, кто делает следующий ход.
 * При режиме "Бот против Бота" мы используем два разных кода игроков (GameLogic::Human и GameLogic::AI),
 * чтобы различать цвета фигур.
 */

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <vector>
#include "../../backend/include/game-logic.h"
#include "../../backend/include/alpha-beta-ai.h"

/**
 * @brief Класс BoardView наследуется от QGraphicsView и обрабатывает клики по игровому полю.
 */
class BoardView : public QGraphicsView {
    Q_OBJECT
public:
    explicit BoardView(QWidget* parent = nullptr);
protected:
    void mousePressEvent(QMouseEvent* event) override;
signals:
    void cellClicked(int row, int col);
};

/**
 * @brief Класс GameBoardWidget реализует логику игрового поля.
 */
class GameBoardWidget : public QWidget {
    Q_OBJECT
public:
    /**
     * @brief Конструктор игрового поля.
     * @param playerVsBot true, если режим "Игрок против Бота", иначе "Бот против Бота".
     * @param difficulty Глубина поиска для алгоритма alpha-beta.
     * @param parent Родительский виджет.
     */
    explicit GameBoardWidget(bool playerVsBot, int difficulty, QWidget* parent = nullptr);

signals:
    /// Сигнал возврата в меню (сброс текущей игры).
    void returnToMenu();

private slots:
    void onCellClicked(int row, int col);
    void onReturnToMenu();
    void onUndo();
    void onHint();
    void onSaveGame();
    void onLoadGame();
    void onBotMove();

private:
    void setupUI();
    void drawBoard();
    void updateBoard();
    void addMoveToHistory();
    bool checkGameOver();

    BoardView* boardView;         // Виджет для отображения игрового поля.
    QGraphicsScene* scene;        // Сцена для отрисовки элементов (сетка, фишки).
    QPushButton* btnReturn;       // Кнопка возврата в меню.
    QPushButton* btnUndo;         // Кнопка отмены последнего хода.
    QPushButton* btnHint;         // Кнопка подсказки от ИИ.
    QPushButton* btnSave;         // Кнопка сохранения игры.
    QPushButton* btnLoad;         // Кнопка загрузки сохраненной игры.
    QLabel* statusLabel;          // Метка для отображения текущего хода.
    QTimer* botTimer;             // Таймер для автоматизированного хода ИИ.

    GameLogic game;               // Логика игры: хранит состояние доски и методы для ходов.
    AlphaBetaAI ai;               // Объект для работы алгоритма alpha-beta.
    int botDepth;                 // Глубина поиска, определяющая уровень сложности.
    bool playerVsBot;             // Режим игры: true, если "Игрок против Бота".

    int currentTurn;              // Текущий игрок: будет равен GameLogic::Human или GameLogic::AI.
                                // В режиме Bot vs Bot – это два разных бота с различными цветами.

    // Структура для сохранения состояния игры (доска и чей ход следующий).
    struct SavedState {
        std::vector<std::vector<int>> board; // Текущее состояние доски.
        int currentPlayer;                   // Тот, кто должен сделать следующий ход.
    };
    std::vector<SavedState> moveHistory; // История ходов для отмены.
    SavedState lastSavedState;           // Сохранённое состояние игры.
    bool hasSavedState = false;          // Флаг наличия сохранённого состояния.

    const int cellSize = 30; // Размер клетки доски в пикселях.
};
