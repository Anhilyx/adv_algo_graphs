#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "graphwidget.h"
#include "matricewidget.h"
#include <iostream>
#include <string>

MainWindow::MainWindow(Matrice* matrice, QWidget *parent)
    : QMainWindow(parent)
    , currentMatrice(matrice)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    GraphWidget* widget = new GraphWidget(currentMatrice, this);
    setCentralWidget(widget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionImport_matrice_triggered()
{

}


void MainWindow::on_actionSave_Graph_triggered()
{

}


void MainWindow::on_actionExit_triggered()
{
    close();
}


void MainWindow::on_actionGraph_triggered()
{
    GraphWidget* widget = new GraphWidget(currentMatrice, this);
    setCentralWidget(widget);
}


void MainWindow::on_actionClusters_triggered()
{
    Matrice* matrice = currentMatrice->clusterMatrice();
    GraphWidget* widget = new GraphWidget(matrice, this);
    setCentralWidget(widget);
}


void MainWindow::on_actionMatrice_triggered()
{
    MatriceWidget* widget = new MatriceWidget(currentMatrice, this);
    setCentralWidget(widget);
}


void MainWindow::on_actionFloyd_Warshall_triggered()
{
    Matrice* matrice = currentMatrice->floydWarshall();
    MatriceWidget* widget = new MatriceWidget(matrice, this);
    setCentralWidget(widget);

}


void MainWindow::on_actionCluster_Matrice_triggered()
{
    Matrice* matrice = currentMatrice->clusterMatrice();
    MatriceWidget* widget = new MatriceWidget(matrice, this);
    setCentralWidget(widget);
}

