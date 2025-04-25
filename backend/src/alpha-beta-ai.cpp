#include "../include/alpha-beta-ai.h"
#include <algorithm>
#include <limits>

// Константная оценка выигрыша – большой балл для мгновенной победы.
static const int WIN_SCORE = 100000;

AlphaBetaAI::AlphaBetaAI() {
    // Дополнительная инициализация не требуется.
}

/**
 * @brief evaluate Оценивает данную позицию.
 *
 * Если кто-либо выигрывает, возвращается WIN_SCORE или -WIN_SCORE.
 * Если нет, для каждой заполненной клетки, для которой не учитывается уже подсчитанная цепочка,
 * анализируются 4 направления (вправо, вниз, диагональ вниз-вправо, диагональ вверх-вправо).
 * Вычисляется длина цепочки и количество открытых концов (если свободна клетка перед или после цепочки).
 * В зависимости от этих параметров цепочке назначается определённое значение.
 *
 * @param game Текущее состояние игры.
 * @return Оценка позиции (положительный балл – в пользу ИИ, отрицательный – в пользу игрока).
 */
int AlphaBetaAI::evaluate(GameLogic &game) {
    int winner = game.checkWinner();
    if (winner == GameLogic::AI)
        return WIN_SCORE;
    else if (winner == GameLogic::Human)
        return -WIN_SCORE;

    int score = 0;
    const int BOARD_SIZE = GameLogic::BOARD_SIZE;
    
    // Определяем 4 направления: вправо, вниз, диагональ вниз-вправо, диагональ вверх-вправо.
    int dx[4] = { 0, 1, 1, -1 };
    int dy[4] = { 1, 0, 1, 1 };

    for (int i = 0; i < BOARD_SIZE; i++){
        for (int j = 0; j < BOARD_SIZE; j++){
            int piece = game.board[i][j];
            if (piece == GameLogic::None)
                continue;
            
            for (int d = 0; d < 4; d++){
                // Если предыдущая клетка в данном направлении имеет ту же фишку,
                // то цепочка уже оценивалась — пропускаем её.
                int prev_r = i - dx[d];
                int prev_c = j - dy[d];
                if (prev_r >= 0 && prev_r < BOARD_SIZE && prev_c >= 0 && prev_c < BOARD_SIZE &&
                    game.board[prev_r][prev_c] == piece) {
                    continue;
                }

                // Считаем длину цепочки, начиная с клетки (i, j).
                int count = 0;
                int r = i, c = j;
                while (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE &&
                       game.board[r][c] == piece) {
                    count++;
                    r += dx[d];
                    c += dy[d];
                }
                
                // Определяем число открытых концов цепочки.
                int openEnds = 0;
                if (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE &&
                    game.board[r][c] == GameLogic::None)
                    openEnds++;
                int before_r = i - dx[d];
                int before_c = j - dy[d];
                if (before_r >= 0 && before_r < BOARD_SIZE && before_c >= 0 && before_c < BOARD_SIZE &&
                    game.board[before_r][before_c] == GameLogic::None)
                    openEnds++;
                
                int chainScore = 0;
                if (count >= 5) {
                    chainScore = WIN_SCORE;
                }
                else if (count == 4) {
                    chainScore = (openEnds == 2) ? 10000 : 1000;
                }
                else if (count == 3) {
                    chainScore = (openEnds == 2) ? 1000 : 100;
                }
                else if (count == 2) {
                    chainScore = (openEnds == 2) ? 100 : 10;
                }
                else {
                    chainScore = 10;
                }
                
                if (piece == GameLogic::AI)
                    score += chainScore;
                else
                    score -= chainScore;
            }
        }
    }
    return score;
}

/**
 * @brief alphaBeta Рекурсивно ищет оптимальную оценку позиции с альфа-бета отсечением.
 * @param game Текущее состояние игры.
 * @param depth Глубина поиска.
 * @param alpha Текущее значение альфа.
 * @param beta Текущее значение бета.
 * @param maximizingPlayer Если true – максимизируем оценку (ход ИИ), иначе – минимизируем.
 * @return Полученная оценка позиции.
 */
int AlphaBetaAI::alphaBeta(GameLogic &game, int depth, int alpha, int beta, bool maximizingPlayer) {
    int currentScore = evaluate(game);
    if (depth == 0 || currentScore >= WIN_SCORE || currentScore <= -WIN_SCORE)
        return currentScore;

    std::vector<std::pair<int, int>> moves = game.getAvailableMoves();
    if (moves.empty())
        return 0;  // ничья

    if (maximizingPlayer) {
        int maxEval = std::numeric_limits<int>::min();
        for (auto move : moves) {
            game.makeMove(move.first, move.second, GameLogic::AI);
            int eval = alphaBeta(game, depth - 1, alpha, beta, false);
            game.undoMove(move.first, move.second);
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, maxEval);
            if (beta <= alpha)
                break;  // отсечение
        }
        return maxEval;
    } else {
        int minEval = std::numeric_limits<int>::max();
        for (auto move : moves) {
            game.makeMove(move.first, move.second, GameLogic::Human);
            int eval = alphaBeta(game, depth - 1, alpha, beta, true);
            game.undoMove(move.first, move.second);
            minEval = std::min(minEval, eval);
            beta = std::min(beta, minEval);
            if (beta <= alpha)
                break;  // отсечение
        }
        return minEval;
    }
}

/**
 * @brief getBestMove Определяет лучший ход для ИИ (по умолчанию для максимизирующего игрока).
 */
std::pair<int, int> AlphaBetaAI::getBestMove(GameLogic &game, int depth) {
    return getBestMove(game, depth, true);
}

/**
 * @brief getBestMove Перегруженный метод, определяющий лучший ход для выбранного игрока.
 *
 * Если maximizingPlayer == true, считается, что оптимальный ход выбирается для максимизирующего игрока (например, AI).
 * Если false – для минимизирующего (например, Human). Перед выполнением основного поиска
 * проверяются возможность мгновенной победы и возможность блокировки.
 *
 * @param game Текущее состояние игры.
 * @param depth Глубина поиска.
 * @param maximizingPlayer Если true, оптимизируем для максимизирующего игрока, иначе для минимизирующего.
 * @return Пара координат (row, col) лучшего хода.
 */
std::pair<int, int> AlphaBetaAI::getBestMove(GameLogic &game, int depth, bool maximizingPlayer) {
    std::vector<std::pair<int, int>> moves = game.getAvailableMoves();
    if (moves.empty())
        return std::make_pair(-1, -1);

    if (maximizingPlayer) {
        // 1. Проверка: может ли AI выиграть за один ход.
        for (auto move : moves) {
            game.makeMove(move.first, move.second, GameLogic::AI);
            if (game.checkWin(move.first, move.second, GameLogic::AI)) {
                game.undoMove(move.first, move.second);
                return move;
            }
            game.undoMove(move.first, move.second);
        }
        
        // 2. Проверка: может ли Human выиграть за один ход – блокируем.
        for (auto move : moves) {
            game.makeMove(move.first, move.second, GameLogic::Human);
            if (game.checkWin(move.first, move.second, GameLogic::Human)) {
                game.undoMove(move.first, move.second);
                return move;
            }
            game.undoMove(move.first, move.second);
        }
        
        // 3. Если прямых вариантов нет, выполняем стандартный поиск для максимизирующего игрока.
        int bestScore = std::numeric_limits<int>::min();
        std::pair<int, int> bestMove = moves[0];
        int alpha = std::numeric_limits<int>::min();
        int beta = std::numeric_limits<int>::max();
        for (auto move : moves) {
            game.makeMove(move.first, move.second, GameLogic::AI);
            int score = alphaBeta(game, depth - 1, alpha, beta, false);
            game.undoMove(move.first, move.second);
            if (score > bestScore) {
                bestScore = score;
                bestMove = move;
            }
            alpha = std::max(alpha, bestScore);
        }
        return bestMove;
    } else {
        // Для минимизирующего игрока (Human):
        // 1. Проверяем возможность мгновенной победы для Human.
        for (auto move : moves) {
            game.makeMove(move.first, move.second, GameLogic::Human);
            if (game.checkWin(move.first, move.second, GameLogic::Human)) {
                game.undoMove(move.first, move.second);
                return move;
            }
            game.undoMove(move.first, move.second);
        }
        // 2. Проверяем, может ли AI выиграть за один ход – блокируем.
        for (auto move : moves) {
            game.makeMove(move.first, move.second, GameLogic::AI);
            if (game.checkWin(move.first, move.second, GameLogic::AI)) {
                game.undoMove(move.first, move.second);
                return move;
            }
            game.undoMove(move.first, move.second);
        }
        // 3. Стандартный поиск для минимизирующего игрока.
        int bestScore = std::numeric_limits<int>::max();
        std::pair<int, int> bestMove = moves[0];
        int alpha = std::numeric_limits<int>::min();
        int beta = std::numeric_limits<int>::max();
        for (auto move : moves) {
            game.makeMove(move.first, move.second, GameLogic::Human);
            int score = alphaBeta(game, depth - 1, alpha, beta, true);
            game.undoMove(move.first, move.second);
            if (score < bestScore) {
                bestScore = score;
                bestMove = move;
            }
            beta = std::min(beta, bestScore);
        }
        return bestMove;
    }
}
