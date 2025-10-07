#include "mainwindow.h"

#include "graph.h"
#include "matrice.h"
#include <iostream>

#include <QApplication>

// #define MATRICE_PATH "../../examples/mat3.txt"
#define MATRICE_PATH "../../examples/mat_x20.txt"

int main(int argc, char *argv[])
{
    // Tests, with outputs in the console
    try {
        // Matrice from file
        Matrice m(MATRICE_PATH);
        std::string s = m;
        std::cout << s << std::endl;

        // Floyd-Warshall
        Matrice m2 = m.floydWarshall();
        std::string s2 = m2;
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
        std::cout << s3 << std::endl;

    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    // QT Application
    QApplication a(argc, argv);
    MainWindow w;
    Matrice m = Matrice(MATRICE_PATH);
    Graph g = Graph(&m);
    w.setCentralWidget(&g);
    w.show();
    return a.exec();
}
