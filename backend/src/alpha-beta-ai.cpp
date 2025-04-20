#include "../include/alpha-beta-ai.h"
#include <limits>

static const int WIN_SCORE = 100000;

// A helper function that evaluates a chain with length and open endpoints.
static int scoreChain(int count, bool openStart, bool openEnd) {
    if (count >= 5)
        return WIN_SCORE;
    if (count == 4)
        return (openStart && openEnd) ? 10000 : 1000;
    if (count == 3)
        return (openStart && openEnd) ? 1000 : 100;
    if (count == 2)
        return (openStart && openEnd) ? 100 : 10;
    return 1;
}

AlphaBetaAI::AlphaBetaAI()
{
}

int AlphaBetaAI::evaluate(GameLogic &game)
{
    int winner = game.checkWinner();
    if (winner == GameLogic::AI)
        return WIN_SCORE;
    else if (winner == GameLogic::Human)
        return -WIN_SCORE;

    int totalScore = 0;
    const int BOARD_SIZE = GameLogic::BOARD_SIZE;
    // Four directions: horizontal, vertical, diagonal, anti-diagonal.
    int directions[4][2] = { {0, 1}, {1, 0}, {1, 1}, {1, -1} };

    // Loop through the board.
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            int piece = game.board[r][c];
            if (piece == GameLogic::None)
                continue;

            // For each direction, only count chain if this is the starting cell.
            for (int d = 0; d < 4; d++) {
                int dx = directions[d][0];
                int dy = directions[d][1];

                // Skip if the previous cell in this direction is the same piece to avoid double counting.
                int prevR = r - dx;
                int prevC = c - dy;
                if (prevR >= 0 && prevR < BOARD_SIZE && prevC >= 0 && prevC < BOARD_SIZE) {
                    if (game.board[prevR][prevC] == piece)
                        continue;
                }

                // Count consecutive pieces.
                int count = 0;
                int curR = r, curC = c;
                while (curR >= 0 && curR < BOARD_SIZE &&
                       curC >= 0 && curC < BOARD_SIZE &&
                       game.board[curR][curC] == piece) {
                    count++;
                    curR += dx;
                    curC += dy;
                }

                // Check open ends.
                bool openStart = false, openEnd = false;
                int startR = r - dx, startC = c - dy;
                if (startR >= 0 && startR < BOARD_SIZE &&
                    startC >= 0 && startC < BOARD_SIZE &&
                    game.board[startR][startC] == GameLogic::None) {
                    openStart = true;
                }
                if (curR >= 0 && curR < BOARD_SIZE &&
                    curC >= 0 && curC < BOARD_SIZE &&
                    game.board[curR][curC] == GameLogic::None) {
                    openEnd = true;
                }

                int chainScore = scoreChain(count, openStart, openEnd);
                // For AI pieces add score; for Human pieces subtract.
                if (piece == GameLogic::AI)
                    totalScore += chainScore;
                else
                    totalScore -= chainScore;
            }
        }
    }
    return totalScore;
}

int AlphaBetaAI::alphaBeta(GameLogic &game, int depth, int alpha, int beta, bool maximizingPlayer)
{
    int score = evaluate(game);
    if (depth == 0 || score <= -WIN_SCORE || score >= WIN_SCORE)
        return score;

    std::vector<std::pair<int, int>> moves = game.getAvailableMoves();
    if (moves.empty())
        return 0;  // Draw

    if (maximizingPlayer) {
        int maxEval = -std::numeric_limits<int>::max();
        for (const std::pair<int, int>& move : moves) {
            game.makeMove(move.first, move.second, GameLogic::AI);
            int eval = alphaBeta(game, depth - 1, alpha, beta, false);
            game.undoMove(move.first, move.second);
            if (eval > maxEval)
                maxEval = eval;
            if (maxEval > alpha)
                alpha = maxEval;
            if (beta <= alpha)
                break;
        }
        return maxEval;
    } else {
        int minEval = std::numeric_limits<int>::max();
        for (const std::pair<int, int>& move : moves) {
            game.makeMove(move.first, move.second, GameLogic::Human);
            int eval = alphaBeta(game, depth - 1, alpha, beta, true);
            game.undoMove(move.first, move.second);
            if (eval < minEval)
                minEval = eval;
            if (minEval < beta)
                beta = minEval;
            if (beta <= alpha)
                break;
        }
        return minEval;
    }
}

std::pair<int, int> AlphaBetaAI::getBestMove(GameLogic &game, int depth)
{
    std::vector<std::pair<int, int>> moves = game.getAvailableMoves();
    if (moves.empty())
        return std::make_pair(-1, -1);

    std::pair<int, int> bestMove = moves[0];
    int bestScore = -std::numeric_limits<int>::max();
    int alpha = -std::numeric_limits<int>::max();
    int beta = std::numeric_limits<int>::max();

    for (const std::pair<int, int>& move : moves) {
        game.makeMove(move.first, move.second, GameLogic::AI);
        int score = alphaBeta(game, depth - 1, alpha, beta, false);
        game.undoMove(move.first, move.second);
        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }
        if (bestScore > alpha)
            alpha = bestScore;
    }
    return bestMove;
}
