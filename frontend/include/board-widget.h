#pragma once

#include <QWidget>
#include "../../backend/include/game-logic.h"
#include "../../backend/include/alpha-beta-ai.h"

class BoardWidget : public QWidget {
    Q_OBJECT
public:
    explicit BoardWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    void aiMove();

private:
    GameLogic game;
    AlphaBetaAI ai;
    bool humanTurn;
    bool gameOver;  // New flag: becomes true when game is finished.
    int cellSize;   // in pixels
};
