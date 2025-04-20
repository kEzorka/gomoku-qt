#pragma once
#include "game-logic.h"
#include <utility>
#include <vector>

class AlphaBetaAI
{
public:
    AlphaBetaAI();

    // Returns the best move as (row, col); returns (-1, -1) if no moves left.
    std::pair<int, int> getBestMove(GameLogic &game, int depth);

private:
    int alphaBeta(GameLogic &game, int depth, int alpha, int beta, bool maximizingPlayer);
    int evaluate(GameLogic &game);
};
