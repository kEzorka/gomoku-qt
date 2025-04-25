#pragma once

#include <vector>
#include <utility>

/*
  Класс GameLogic отвечает за игровую логику.
  Он содержит игровое поле (массив board), методы для проверки и выполнения ходов,
  удаления хода (undo), проверки выигрыша и возвращения списка доступных ходов.
*/
class GameLogic {
public:
    static const int BOARD_SIZE = 15; // Размер игрового поля (15x15)

    // Определение игроков и пустой клетки
    enum Player {
        None = 0,   // Пустая клетка
        Human = 1,  // Игрок-человек
        AI = 2      // Компьютер (бот)
    };

    // Конструктор: инициализирует игровое поле значением None.
    GameLogic();

    // Проверка, является ли ход по координатам (row, col) допустимым.
    bool isMoveValid(int row, int col) const;

    // Выполнение хода для заданного игрока. Если ход не допустим, возвращает false.
    bool makeMove(int row, int col, Player player);

    // Отмена хода на указанной клетке.
    void undoMove(int row, int col);

    // Проверка, выиграл ли игрок, сделав ход в (row, col).
    bool checkWin(int row, int col, Player player) const;

    // Проверка всего поля на наличие победителя, возвращает игрока, если кто-то выиграл, иначе None.
    int checkWinner() const;

    // Возвращает список доступных ходов в виде вектора пар (row, col).
    std::vector<std::pair<int, int>> getAvailableMoves() const;

    // Игровое поле: двумерный массив, где записаны номера игроков или None.
    int board[BOARD_SIZE][BOARD_SIZE];
};
