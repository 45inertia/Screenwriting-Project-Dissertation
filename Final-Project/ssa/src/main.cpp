#include "view/mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setStyleSheet(R"(

    )");

    MainWindow w;
    w.show();
    return a.exec();
}
