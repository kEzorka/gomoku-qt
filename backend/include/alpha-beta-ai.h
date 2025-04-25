#pragma once
/*
 * alpha-beta-ai.h
 *
 * Заголовочный файл класса AlphaBetaAI, реализующего поиск лучшего хода с помощью
 * алгоритма альфа-бета отсечения.
 *
 * Функция getBestMove сначала проверяет возможность мгновенной победы для текущего игрока,
 * затем – блокирует немедленную победу противника. Если ни одна из этих проверок не срабатывает,
 * выполняется стандартный поиск с отсечением.
 *
 * Для режима "Бот против Бота" добавлена перегрузка функции getBestMove,
 * позволяющая задать дополнительный параметр maximizingPlayer.
 */

#include "game-logic.h"
#include <utility>
#include <vector>
#include <limits>

class AlphaBetaAI {
public:
    // Конструктор – дополнительная инициализация не требуется.
    AlphaBetaAI();

    /**
     * @brief getBestMove Определяет лучший ход для ИИ (по умолчанию для максимизирующего игрока).
     * @param game Текущее состояние игры.
     * @param depth Глубина поиска.
     * @return Пара координат (row, col) для лучшего хода.
     */
    std::pair<int, int> getBestMove(GameLogic &game, int depth);

    /**
     * @brief getBestMove Перегруженный метод, определяющий лучший ход для выбранного игрока.
     * @param game Текущее состояние игры.
     * @param depth Глубина поиска.
     * @param maximizingPlayer Если true – выбирается оптимальный ход для максимизирующего игрока,
     *                         если false – для минимизирующего.
     * @return Пара координат (row, col) для лучшего хода.
     */
    std::pair<int, int> getBestMove(GameLogic &game, int depth, bool maximizingPlayer);

private:
    /**
     * @brief alphaBeta Рекурсивная функция поиска с альфа-бета отсечением.
     * @param game Текущее состояние игры.
     * @param depth Текущая глубина поиска.
     * @param alpha Текущее значение альфа.
     * @param beta Текущее значение бета.
     * @param maximizingPlayer Если true – максимизируем (ход ИИ), иначе – минимизируем (ход игрока).
     * @return Числовая оценка позиции.
     */
    int alphaBeta(GameLogic &game, int depth, int alpha, int beta, bool maximizingPlayer);

    /**
     * @brief evaluate Оценивает позицию на доске.
     *
     * Если кто-либо выигрывает, возвращается WIN_SCORE или -WIN_SCORE.
     * Иначе производится анализ цепочек фишек в 4 направлениях с учётом длины и количества открытых концов.
     *
     * @param game Текущее состояние игры.
     * @return Оценка позиции.
     */
    int evaluate(GameLogic &game);
};
