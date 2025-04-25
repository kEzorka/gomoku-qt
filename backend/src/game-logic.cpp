#include "../include/game-logic.h"

// Конструктор: заполняет игровое поле значениями None.
GameLogic::GameLogic() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = None;
        }
    }
}

// Проверяет, что координаты (row, col) находятся в пределах поля и клетка пуста.
bool GameLogic::isMoveValid(int row, int col) const {
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE)
        return false;
    return board[row][col] == None;
}

// Делает ход: если клетка пустая, ставит номер игрока и возвращает true.
bool GameLogic::makeMove(int row, int col, Player player) {
    if (!isMoveValid(row, col))
        return false;
    board[row][col] = player;
    return true;
}

// Отменяет ход, устанавливая клетку на None.
void GameLogic::undoMove(int row, int col) {
    if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE)
        board[row][col] = None;
}

// Проверяет, выиграл ли игрок, сделав ход в точке (row, col).
// Для этого проверяются 4 направления: горизонталь, вертикаль и две диагонали.
bool GameLogic::checkWin(int row, int col, Player player) const {
    int directions[4][2] = { {0, 1}, {1, 0}, {1, 1}, {1, -1} };

    for (int d = 0; d < 4; d++) {
        int count = 1;  // Начальное количество в цепочке равно 1 (считая сам ход)
        int dx = directions[d][0], dy = directions[d][1];

        // Смотрим в одном направлении.
        int r = row + dx, c = col + dy;
        while (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE && board[r][c] == player) {
            count++;
            r += dx;
            c += dy;
        }
        // Смотрим в противоположном направлении.
        r = row - dx, c = col - dy;
        while (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE && board[r][c] == player) {
            count++;
            r -= dx;
            c -= dy;
        }
        // Если найдено 5 или более подряд, игрок выигрывает.
        if (count >= 5)
            return true;
    }
    return false;
}

// Проверяет игровое поле на наличие победителя.
// Проходит по всем клеткам и, если клетка не пуста, вызывает checkWin.
// Если найден победитель, возвращает номер игрока.
int GameLogic::checkWinner() const {
    for (int i = 0; i < BOARD_SIZE; i++){
        for (int j = 0; j < BOARD_SIZE; j++){
            int player = board[i][j];
            if (player != None && checkWin(i, j, static_cast<Player>(player)))
                return player;
        }
    }
    return None;
}

// Возвращает вектор пар координат для всех пустых клеток (доступных ходов).
std::vector<std::pair<int, int>> GameLogic::getAvailableMoves() const {
    std::vector<std::pair<int, int>> moves;
    for (int i = 0; i < BOARD_SIZE; i++){
        for (int j = 0; j < BOARD_SIZE; j++){
            if (board[i][j] == None)
                moves.push_back(std::make_pair(i, j));
        }
    }
    return moves;
}
