#include "../include/main-window.h"
#include "../include/board-widget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    BoardWidget *boardWidget = new BoardWidget(this);
    setCentralWidget(boardWidget);
}
