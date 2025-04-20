#include "../include/board-widget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include <QTimer>

BoardWidget::BoardWidget(QWidget *parent)
    : QWidget(parent),
      humanTurn(true),
      gameOver(false),
      cellSize(30)  // initial default; will be adjusted in paintEvent
{
    // Set minimum size for the board
    setMinimumSize(GameLogic::BOARD_SIZE * cellSize, GameLogic::BOARD_SIZE * cellSize);
}

void BoardWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);

    // Adjust cell size to fill widget
    int minDim = (width() < height() ? width() : height());
    cellSize = minDim / GameLogic::BOARD_SIZE;

    // Draw grid
    for (int i = 0; i <= GameLogic::BOARD_SIZE; i++) {
        painter.drawLine(i * cellSize, 0, i * cellSize, GameLogic::BOARD_SIZE * cellSize);
        painter.drawLine(0, i * cellSize, GameLogic::BOARD_SIZE * cellSize, i * cellSize);
    }

    // Draw game pieces
    for (int row = 0; row < GameLogic::BOARD_SIZE; row++) {
        for (int col = 0; col < GameLogic::BOARD_SIZE; col++) {
            int piece = game.board[row][col];
            if (piece == GameLogic::None)
                continue;

            if (piece == GameLogic::Human)
                painter.setBrush(Qt::black);
            else
                painter.setBrush(Qt::white);

            int x = col * cellSize;
            int y = row * cellSize;
            painter.drawEllipse(x + 2, y + 2, cellSize - 4, cellSize - 4);
        }
    }
}

void BoardWidget::mousePressEvent(QMouseEvent *event)
{
    // Do nothing if game over.
    if (gameOver)
        return;

    // Use the new Qt6 API: use event->position()
    QPoint pos = event->position().toPoint();
    int col = pos.x() / cellSize;
    int row = pos.y() / cellSize;

    if (row < 0 || row >= GameLogic::BOARD_SIZE || col < 0 || col >= GameLogic::BOARD_SIZE)
        return;
    if (!game.isMoveValid(row, col))
        return;

    // Place human's move.
    game.makeMove(row, col, GameLogic::Human);
    update();

    // Check for human win.
    if (game.checkWin(row, col, GameLogic::Human)) {
        QMessageBox::information(this, "Game Over", "Human wins!");
        gameOver = true;
        return;
    }

    humanTurn = false;
    // Schedule AI move after a short delay.
    QTimer::singleShot(100, this, SLOT(aiMove()));
}

void BoardWidget::aiMove()
{
    // Do nothing if game is over.
    if (gameOver)
        return;

    // Find the best move (try increasing depth for stronger play)
    std::pair<int, int> move = ai.getBestMove(game, 3);
    if (move.first == -1 || move.second == -1) {
        QMessageBox::information(this, "Game Over", "Draw!");
        gameOver = true;
        return;
    }

    game.makeMove(move.first, move.second, GameLogic::AI);
    update();

    if (game.checkWin(move.first, move.second, GameLogic::AI)) {
        QMessageBox::information(this, "Game Over", "AI wins!");
        gameOver = true;
        return;
    }

    humanTurn = true;
}
