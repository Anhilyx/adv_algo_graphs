#include "mainwindow.h"

#include "graphwidget.h"
#include "matrice.h"
#include "matricewidget.h"
#include <iostream>

#include <QApplication>

// #define MATRICE_PATH "../../examples/mat3.txt"
#define MATRICE_PATH "../../examples/mat_x20.txt"

int main(int argc, char *argv[])
{
    // Tests, with outputs in the console
    /* try {
        // Matrice from file
        Matrice m(MATRICE_PATH);
        std::string s = m;
        m.toMarkdown("E:/Downloads/matrice.md");
        m.toHtml("E:/Downloads/matrice.html");
        std::cout << s << std::endl;

        // Floyd-Warshall
        Matrice m2 = m.floydWarshall();
        std::string s2 = m2;
        m2.toMarkdown("E:/Downloads/matrice-2.md");
        m2.toHtml("E:/Downloads/matrice-2.html");
        std::cout << s2 << std::endl;

        // Kosaraju (strongly connected components)
        auto clusters = m.kosaraju();
        for (size_t i = 0; i < clusters.size(); i++) {
            for (size_t j = 0; j < clusters.at(i).size(); j++) {
                std::cout << clusters.at(i).at(j) << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;

        // Cluster Matrice
        Matrice m3 = m.clusterMatrice();
        std::string s3 = m3;
        m3.toMarkdown("E:/Downloads/matrice-3.md");
        m3.toHtml("E:/Downloads/matrice-3.html");
        std::cout << s3 << std::endl;

    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    } */

    // QT Application
    QApplication a(argc, argv);
    Matrice* m = new Matrice(MATRICE_PATH);
    MainWindow w(m);
    w.show();
    return a.exec();
}
