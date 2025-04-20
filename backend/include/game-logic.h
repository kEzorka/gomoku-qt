#pragma once
#include <vector>
#include <utility>

class GameLogic
{
public:
    static const int BOARD_SIZE = 15;
    enum Player {
        None = 0,
        Human = 1,
        AI = 2
    };

    GameLogic();

    // Returns whether the cell at (row, col) is empty
    bool isMoveValid(int row, int col) const;

    // Places a move for the given player; returns false if invalid
    bool makeMove(int row, int col, Player player);

    // Undoes a move at (row, col)
    void undoMove(int row, int col);

    // Checks whether the last move made by 'player' produced a win
    bool checkWin(int row, int col, Player player) const;

    // Checks the entire board for a winner; returns None if no winner
    int checkWinner() const;

    // Returns a list of all available moves (row, col)
    std::vector<std::pair<int, int>> getAvailableMoves() const;

    // Public board state; board[row][col] holds None, Human, or AI.
    int board[BOARD_SIZE][BOARD_SIZE];
};
