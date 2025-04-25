#include "../include/game-board-widget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QMessageBox>

/* ---------------------- BoardView ------------------------
 *
 * Класс BoardView наследуется от QGraphicsView и обрабатывает клики по игровому полю.
 */
BoardView::BoardView(QWidget *parent)
    : QGraphicsView(parent)
{
    setRenderHint(QPainter::Antialiasing);
    // Фиксированный размер поля: 15 клеток по 30px плюс небольшие отступы.
    setFixedSize(470, 470);
    setSceneRect(0, 0, 450, 450);
}

void BoardView::mousePressEvent(QMouseEvent *event)
{
    QPointF pt = mapToScene(event->pos());
    int row = pt.y() / 30;
    int col = pt.x() / 30;
    emit cellClicked(row, col);
    QGraphicsView::mousePressEvent(event);
}

/* ------------------ GameBoardWidget ----------------------
 *
 * Класс GameBoardWidget реализует логику игрового поля.
 * Отвечает за отрисовку доски, обработку ходов (игрока и ИИ),
 * сохранение и отмену ходов, а также подсказки и возврат в меню.
 */
GameBoardWidget::GameBoardWidget(bool playerVsBot, int difficulty, QWidget *parent)
    : QWidget(parent),
      botDepth(difficulty),
      playerVsBot(playerVsBot)
{
    setupUI();
    drawBoard();
    connect(boardView, &BoardView::cellClicked, this, &GameBoardWidget::onCellClicked);

    botTimer = new QTimer(this);
    botTimer->setInterval(1000);
    connect(botTimer, &QTimer::timeout, this, &GameBoardWidget::onBotMove);

    // Определение первого хода:
    // В режиме "Игрок против Бота" первым ходом всегда является игрок.
    // В режиме "Бот против Бота" мы чередуем ходы, начиная с первого бота, которого мы помечаем как GameLogic::Human.
    if(playerVsBot)
        currentTurn = GameLogic::Human;
    else {
        currentTurn = GameLogic::Human;  // Первый бот (отобразится голубым)
        botTimer->start();
    }
    updateBoard();
}

void GameBoardWidget::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    boardView = new BoardView(this);
    scene = new QGraphicsScene(this);
    boardView->setScene(scene);
    mainLayout->addWidget(boardView);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    btnReturn = new QPushButton("В меню", this);
    btnUndo   = new QPushButton("Отменить ход", this);
    btnHint   = new QPushButton("Подсказка", this);
    btnSave   = new QPushButton("Сохранить игру", this);
    btnLoad   = new QPushButton("Загрузить игру", this);
    buttonLayout->addWidget(btnReturn);
    buttonLayout->addWidget(btnUndo);
    buttonLayout->addWidget(btnHint);
    buttonLayout->addWidget(btnSave);
    buttonLayout->addWidget(btnLoad);
    mainLayout->addLayout(buttonLayout);

    // Для режима "Бот против Бота" кнопка отмены отключена.
    if(!playerVsBot)
        btnUndo->setEnabled(false);

    statusLabel = new QLabel("Ход: ", this);
    mainLayout->addWidget(statusLabel);

    connect(btnReturn, &QPushButton::clicked, this, &GameBoardWidget::onReturnToMenu);
    connect(btnUndo,   &QPushButton::clicked, this, &GameBoardWidget::onUndo);
    connect(btnHint,   &QPushButton::clicked, this, &GameBoardWidget::onHint);
    connect(btnSave,   &QPushButton::clicked, this, &GameBoardWidget::onSaveGame);
    connect(btnLoad,   &QPushButton::clicked, this, &GameBoardWidget::onLoadGame);

    // Стилизация: фон с зелёными оттенками и кнопки с голубым фоном.
    this->setStyleSheet("QWidget { background-color: #e8f5e9; } "
                        "QPushButton { background-color: #4fc3f7; color: white; border-radius: 5px; padding: 5px; } "
                        "QLabel { font-size: 16px; color: #2e7d32; }");
    // Фиксированный размер виджета согласован с меню: 600x600.
    this->setFixedSize(600, 600);
}

void GameBoardWidget::drawBoard()
{
    scene->clear();
    // Рисуем горизонтальные и вертикальные линии сетки.
    for (int i = 0; i <= GameLogic::BOARD_SIZE; ++i) {
        scene->addLine(0, i * cellSize, GameLogic::BOARD_SIZE * cellSize, i * cellSize, QPen(Qt::black));
        scene->addLine(i * cellSize, 0, i * cellSize, GameLogic::BOARD_SIZE * cellSize, QPen(Qt::black));
    }
    // Отрисовываем фишки для каждой клетки, если она занята.
    for (int i = 0; i < GameLogic::BOARD_SIZE; i++){
        for (int j = 0; j < GameLogic::BOARD_SIZE; j++){
            int piece = game.board[i][j];
            if(piece != GameLogic::None) {
                // Цвет определяется по коду: для GameLogic::Human (первый бот или игрок) — синий; для GameLogic::AI (второй бот) — красный.
                QColor color = (piece == GameLogic::Human) ? Qt::blue : Qt::red;
                int margin = 4;
                scene->addEllipse(j * cellSize + margin, i * cellSize + margin,
                                  cellSize - 2 * margin, cellSize - 2 * margin,
                                  QPen(Qt::black), QBrush(color));
            }
        }
    }
}

void GameBoardWidget::updateBoard()
{
    drawBoard();
    QString turnText;
    int winner = game.checkWinner();
    if(winner != GameLogic::None){
        turnText = (winner == GameLogic::Human) ? "Победил Первый бот!" : "Победил Второй бот!";
        btnHint->setEnabled(false);
        btnUndo->setEnabled(false);
        botTimer->stop();
    }
    else if (game.getAvailableMoves().empty()){
        turnText = "Ничья!";
        btnHint->setEnabled(false);
        btnUndo->setEnabled(false);
        botTimer->stop();
    }
    else {
        // Для режима "Игрок против Бота" отображается "Ход: Игрок" или "Ход: Бот".
        // Для Bot vs Bot отображается "Ход: Первый бот" если currentTurn == Human и "Ход: Второй бот" если currentTurn == AI.
        if(playerVsBot)
            turnText = (currentTurn == GameLogic::Human) ? "Ход: Игрок" : "Ход: Бот";
        else
            turnText = (currentTurn == GameLogic::Human) ? "Ход: Первый бот" : "Ход: Второй бот";
    }
    statusLabel->setText(turnText);
}

void GameBoardWidget::onCellClicked(int row, int col)
{
    // Обработка кликов работает только в режиме "Игрок против Бота".
    if(game.checkWinner() != GameLogic::None || !playerVsBot)
        return;
    
    // Если сейчас не очередь игрока, игнорируем клик.
    if(currentTurn != GameLogic::Human)
        return;

    if(game.isMoveValid(row, col)){
        game.makeMove(row, col, GameLogic::Human);
        currentTurn = GameLogic::AI;
        addMoveToHistory();
        updateBoard();
        if(checkGameOver()) return;
        QTimer::singleShot(1000, this, SLOT(onBotMove()));
    }
}

void GameBoardWidget::onBotMove()
{
    // Если игра окончена, прекращаем работу таймера.
    if(game.checkWinner() != GameLogic::None) {
        botTimer->stop();
        return;
    }
    // В режиме "Игрок против Бота" выполняем ход ИИ только если его очередь.
    if(playerVsBot && currentTurn != GameLogic::AI)
        return;
    
    // Если бот против бота, выбираем лучшую ставку в зависимости от currentTurn:
    std::pair<int,int> move;
    if(playerVsBot) {
         // В режиме "Игрок против Бота" ход всегда для AI (maximizing = true).
         move = ai.getBestMove(game, botDepth);
         // Затем, после хода, переключаем ход на игрока.
         game.makeMove(move.first, move.second, GameLogic::AI);
         currentTurn = GameLogic::Human;
    } else {
         // В режиме "Бот против Бота" ход зависит от currentTurn:
         if(currentTurn == GameLogic::AI) {
             // Второй бот – для него вызываем оптимальный ход в максимизирующем режиме.
             move = ai.getBestMove(game, botDepth, true);
             game.makeMove(move.first, move.second, GameLogic::AI);
             currentTurn = GameLogic::Human;
         } else {
             // Первый бот – для него вызываем оптимальный ход в минимизирующем режиме.
             move = ai.getBestMove(game, botDepth, false);
             game.makeMove(move.first, move.second, GameLogic::Human);
             currentTurn = GameLogic::AI;
         }
    }
    addMoveToHistory();
    updateBoard();
    if(checkGameOver()) return;
}

void GameBoardWidget::onReturnToMenu()
{
    botTimer->stop();
    emit returnToMenu();
}

void GameBoardWidget::onUndo()
{
    if(!playerVsBot)
        return;
    int historySize = moveHistory.size();
    if(historySize > 0) {
        moveHistory.pop_back();
        if (moveHistory.size() > 0) {
            moveHistory.pop_back();
        }
        if(moveHistory.empty()){
            for(int i = 0; i < GameLogic::BOARD_SIZE; i++){
                for(int j = 0; j < GameLogic::BOARD_SIZE; j++){
                    game.board[i][j] = GameLogic::None;
                }
            }
            currentTurn = GameLogic::Human;
        } else {
            const SavedState &lastState = moveHistory.back();
            for(int i = 0; i < GameLogic::BOARD_SIZE; i++){
                for(int j = 0; j < GameLogic::BOARD_SIZE; j++){
                    game.board[i][j] = lastState.board[i][j];
                }
            }
            currentTurn = lastState.currentPlayer;
        }
        updateBoard();
    }
}

void GameBoardWidget::onHint()
{
    if(game.checkWinner() != GameLogic::None)
        return;
    std::pair<int, int> hintMove = ai.getBestMove(game, botDepth);
    if(hintMove.first == -1)
        return;
    int margin = 4;
    QPen pen(Qt::green);
    pen.setWidth(3);
    scene->addEllipse(hintMove.second * cellSize + margin, hintMove.first * cellSize + margin,
                      cellSize - 2 * margin, cellSize - 2 * margin,
                      pen, QBrush(Qt::NoBrush));
}

void GameBoardWidget::onSaveGame()
{
    lastSavedState.board.clear();
    lastSavedState.board.resize(GameLogic::BOARD_SIZE, std::vector<int>(GameLogic::BOARD_SIZE, GameLogic::None));
    for (int i = 0; i < GameLogic::BOARD_SIZE; i++){
        for (int j = 0; j < GameLogic::BOARD_SIZE; j++){
            lastSavedState.board[i][j] = game.board[i][j];
        }
    }
    lastSavedState.currentPlayer = currentTurn;
    hasSavedState = true;
    QMessageBox::information(this, "Сохранение", "Игра сохранена.");
}

void GameBoardWidget::onLoadGame()
{
    if(!hasSavedState){
        QMessageBox::warning(this, "Загрузка", "Нет сохраненной игры.");
        return;
    }
    for (int i = 0; i < GameLogic::BOARD_SIZE; i++){
        for (int j = 0; j < GameLogic::BOARD_SIZE; j++){
            game.board[i][j] = lastSavedState.board[i][j];
        }
    }
    moveHistory.clear();
    SavedState state;
    state.board = lastSavedState.board;
    state.currentPlayer = lastSavedState.currentPlayer;
    moveHistory.push_back(state);
    currentTurn = state.currentPlayer;
    updateBoard();
    QMessageBox::information(this, "Загрузка", "Игра загружена.");
}

void GameBoardWidget::addMoveToHistory()
{
    SavedState state;
    state.board.resize(GameLogic::BOARD_SIZE, std::vector<int>(GameLogic::BOARD_SIZE, GameLogic::None));
    for (int i = 0; i < GameLogic::BOARD_SIZE; i++){
        for (int j = 0; j < GameLogic::BOARD_SIZE; j++){
            state.board[i][j] = game.board[i][j];
        }
    }
    state.currentPlayer = currentTurn;
    moveHistory.push_back(state);
}

/**
 * @brief checkGameOver Проверяет, завершилась ли игра.
 *
 * Если один из игроков выиграл или если больше нет допустимых ходов (ничья),
 * выводится соответствующее сообщение, таймер останавливается,
 * и функция возвращает true. После этого кнопка "В меню" остаётся активной.
 *
 * @return true, если игра закончена, иначе false.
 */
bool GameBoardWidget::checkGameOver()
{
    int winner = game.checkWinner();
    if(winner != GameLogic::None){
        updateBoard();
        QString winnerText = (winner == GameLogic::Human) ? "Игрок победил!" : "Бот победил!";
        // В режиме Bot vs Bot уточним сообщение:
        if(!playerVsBot)
            winnerText = (winner == GameLogic::Human) ? "Первый бот победил!" : "Второй бот победил!";
        QMessageBox::information(this, "Игра окончена", winnerText);
        botTimer->stop();
        return true;
    }
    
    if(game.getAvailableMoves().empty()){
        updateBoard();
        QMessageBox::information(this, "Игра окончена", "Ничья!");
        botTimer->stop();
        return true;
    }
    return false;
}
