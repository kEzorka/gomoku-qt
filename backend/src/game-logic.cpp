#include "../include/game-logic.h"

GameLogic::GameLogic()
{
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            board[i][j] = None;
        }
    }
}

bool GameLogic::isMoveValid(int row, int col) const
{
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE)
        return false;
    return board[row][col] == None;
}

bool GameLogic::makeMove(int row, int col, Player player)
{
    if (!isMoveValid(row, col))
        return false;
    board[row][col] = player;
    return true;
}

void GameLogic::undoMove(int row, int col)
{
    if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE)
        board[row][col] = None;
}

bool GameLogic::checkWin(int row, int col, Player player) const
{
    // Directions: horizontal, vertical, diagonal, anti-diagonal
    int directions[4][2] = { {0, 1}, {1, 0}, {1, 1}, {1, -1} };

    for (int d = 0; d < 4; ++d) {
        int count = 1;
        int dx = directions[d][0], dy = directions[d][1];

        // Check in the forward direction
        int i = row + dx, j = col + dy;
        while (i >= 0 && i < BOARD_SIZE && j >= 0 && j < BOARD_SIZE && board[i][j] == player) {
            ++count;
            i += dx;
            j += dy;
        }

        // Check the opposite direction
        i = row - dx;
        j = col - dy;
        while (i >= 0 && i < BOARD_SIZE && j >= 0 && j < BOARD_SIZE && board[i][j] == player) {
            ++count;
            i -= dx;
            j -= dy;
        }

        if (count >= 5)
            return true;
    }
    return false;
}

int GameLogic::checkWinner() const
{
    // Scan the entire board and use checkWin for any non-empty cell.
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            int player = board[i][j];
            if (player != None && checkWin(i, j, static_cast<Player>(player)))
                return player;
        }
    }
    return None;
}

std::vector<std::pair<int, int>> GameLogic::getAvailableMoves() const
{
    std::vector<std::pair<int, int>> moves;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] == None)
                moves.push_back(std::make_pair(i, j));
        }
    }
    return moves;
}
