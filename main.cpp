#include <QApplication>
#include "frontend/include/main-window.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.resize(600, 600);
    window.show();
    return app.exec();
}
