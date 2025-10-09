#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "graphwidget.h"
#include "matricewidget.h"
#include <iostream>

MainWindow::MainWindow(Matrice* matrice, QWidget* parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    currentMatrice(matrice)
{
    ui->setupUi(this);

    // Create and set the initial central widget to display the graph
    displayedMatrice = currentMatrice;
    displayedWidget = new GraphWidget(currentMatrice, this);
    setCentralWidget(displayedWidget);
}

MainWindow::~MainWindow()
{
    delete ui;

    if (displayedMatrice != currentMatrice) {
        delete displayedMatrice;
    }
    delete currentMatrice;
}

void MainWindow::on_actionImport_triggered()
{
    // TODO
}


void MainWindow::on_actionExport_triggered()
{
    // TODO
}


void MainWindow::on_actionExit_triggered()
{
    close();
}


void MainWindow::on_actionBasicGraphView_triggered()
{
    displayedMatrice = currentMatrice;
    displayedWidget = new GraphWidget(displayedMatrice, this);
    setCentralWidget(displayedWidget);
}


void MainWindow::on_actionClustersGraphView_triggered()
{
    displayedMatrice = currentMatrice->clusterMatrice();
    displayedWidget = new GraphWidget(displayedMatrice, this);
    setCentralWidget(displayedWidget);
}


void MainWindow::on_actionBasicMatriceView_triggered()
{
    displayedMatrice = currentMatrice;
    displayedWidget = new MatriceWidget(displayedMatrice, this);
    setCentralWidget(displayedWidget);
}


void MainWindow::on_actionFloydWarshallMatriceView_triggered()
{
    displayedMatrice = currentMatrice->floydWarshall();
    displayedWidget = new MatriceWidget(displayedMatrice, this);
    setCentralWidget(displayedWidget);

}


void MainWindow::on_actionClustersMatriceView_triggered()
{
    displayedMatrice = currentMatrice->clusterMatrice();
    displayedWidget = new MatriceWidget(displayedMatrice, this);
    setCentralWidget(displayedWidget);
}
