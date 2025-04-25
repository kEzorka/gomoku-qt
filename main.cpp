/*
 * main.cpp
 *
 * Точка входа в приложение.
 * Создаётся объект QApplication, устанавливаются глобальные стили,
 * создаётся и отображается главное окно (MainWindow).
 */

 #include <QApplication>
 #include "frontend/include/main-window.h"
 
 int main(int argc, char *argv[])
 {
     QApplication a(argc, argv);
 
     // Устанавливаем глобальную стилизацию: шрифт и фон главного окна (голубой оттенок)
     a.setStyleSheet(
         "QWidget { font-family: Arial; }"
         "QMainWindow { background-color: #e0f7fa; }"
     );
 
     MainWindow w;
     w.setWindowTitle("Гомоку");
     w.show();
     return a.exec();
 }
 