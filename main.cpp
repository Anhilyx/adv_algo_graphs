#include "mainwindow.h"

#include "matrice.h"

#include <QApplication>

#define MATRICE_PATH "../../examples/mat3.txt"
// #define MATRICE_PATH "../../examples/mat_x20.txt"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Matrice* m = new Matrice(MATRICE_PATH);
    MainWindow w(m);
    w.show();
    return a.exec();
}
