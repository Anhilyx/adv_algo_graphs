/* #include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
} */

#include "matrice.h"
#include <iostream>

int main() {
    try {
        Matrice m("../../examples/mat3.txt");
        std::string s = m;
        std::cout << s << std::endl;

        Matrice m2 = m.floydWarshall();
        std::string s2 = m2;
        std::cout << s2 << std::endl;

        auto clusters = m.kosaraju();
        for (size_t i = 0; i < clusters.size(); i++) {
            for (size_t j = 0; j < clusters.at(i).size(); j++) {
                std::cout << clusters.at(i).at(j) << " ";
            }
            std::cout << std::endl;
        }

    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
