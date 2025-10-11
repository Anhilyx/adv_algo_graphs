#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "errorwidget.h"
#include "graphwidget.h"
#include "matricewidget.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget* parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    currentMatrice(nullptr)
{
    ui->setupUi(this);
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
    // Prompt the user to select a file
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Matrice File"), "", tr("Matrice Files (*.txt);;All Files (*)"));
    if (fileName.isEmpty()) {
        return;
    }

    // Free the resources and load the new matrice
    delete currentMatrice;
    currentMatrice = new Matrice(fileName.toStdString());

    // Update the displayed widget to show the new matrice as a graph
    if      (displayId ==   1) on_actionBasicGraphView_triggered();
    else if (displayId ==   2) on_actionPrimGraphView_triggered();
    else if (displayId ==   4) on_actionClustersGraphView_triggered();
    else if (displayId == 101) on_actionBasicMatriceView_triggered();
    else if (displayId == 102) on_actionPrimMatriceView_triggered();
    else if (displayId == 103) on_actionFloydWarshallMatriceView_triggered();
    else if (displayId == 104) on_actionClustersMatriceView_triggered();
}


void MainWindow::on_actionExport_triggered()
{
    // Ignore the action if there is no matrice loaded
    if (currentMatrice == nullptr) return;

    // Prompt the user to select a file
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image"), "", tr("PNG Image (*.png);;JPEG Image (*.jpg);;BMP Image (*.bmp);;All Files (*)"));
    if (fileName.isEmpty()) {
        return;
    }

    // Save the current displayed widget as an image
    displayedWidget->grab().save(fileName);
}


void MainWindow::on_actionExit_triggered()
{
    close();
}


void MainWindow::on_actionBasicGraphView_triggered()
{
    try {
        // Set the display ID
        displayId = 1;

        // Ignore the rest of the action if there is no matrice loaded
        if (currentMatrice == nullptr) return;

        // Set the displayed matrice to the current one and update the widget
        displayedMatrice = currentMatrice;
        displayedWidget = new GraphWidget(displayedMatrice, this);
        setCentralWidget(displayedWidget);
    
    // If there is an error, display the error widget
    } catch (const std::exception& e) {
        displayedWidget = new ErrorWidget(e.what(), this);
        setCentralWidget(displayedWidget);
    }
}


void MainWindow::on_actionPrimGraphView_triggered()
{
    try {
        // Set the display ID
        displayId = 2;

        // Ignore the rest of the action if there is no matrice loaded
        if (currentMatrice == nullptr) return;

        // Set the displayed matrice to the current one and update the widget
        displayedMatrice = currentMatrice->prim();
        displayedWidget = new GraphWidget(displayedMatrice, this);
        setCentralWidget(displayedWidget);
    
    // If there is an error, display the error widget
    } catch (const std::exception& e) {
        displayedWidget = new ErrorWidget(e.what(), this);
        setCentralWidget(displayedWidget);
    }
}


void MainWindow::on_actionClustersGraphView_triggered()
{
    try {
        // Set the display ID
        displayId = 4;

        // Ignore the rest of the action if there is no matrice loaded
        if (currentMatrice == nullptr) return;

        // Set the displayed matrice to the current one and update the widget
        displayedMatrice = currentMatrice->clusterMatrice();
        displayedWidget = new GraphWidget(displayedMatrice, this);
        setCentralWidget(displayedWidget);
    
    // If there is an error, display the error widget
    } catch (const std::exception& e) {
        displayedWidget = new ErrorWidget(e.what(), this);
        setCentralWidget(displayedWidget);
    }
}


void MainWindow::on_actionBasicMatriceView_triggered()
{
    try {
        // Set the display ID
        displayId = 101;

        // Ignore the rest of the action if there is no matrice loaded
        if (currentMatrice == nullptr) return;

        // Set the displayed matrice to the current one and update the widget
        displayedMatrice = currentMatrice;
        displayedWidget = new MatriceWidget(displayedMatrice, this);
        setCentralWidget(displayedWidget);
    
    // If there is an error, display the error widget
    } catch (const std::exception& e) {
        displayedWidget = new ErrorWidget(e.what(), this);
        setCentralWidget(displayedWidget);
    }
}


void MainWindow::on_actionPrimMatriceView_triggered()
{
    try {
        // Set the display ID
        displayId = 102;

        // Ignore the rest of the action if there is no matrice loaded
        if (currentMatrice == nullptr) return;

        // Set the displayed matrice to the current one and update the widget
        displayedMatrice = currentMatrice->prim();
        displayedWidget = new MatriceWidget(displayedMatrice, this);
        setCentralWidget(displayedWidget);
    
    // If there is an error, display the error widget
    } catch (const std::exception& e) {
        displayedWidget = new ErrorWidget(e.what(), this);
        setCentralWidget(displayedWidget);
    }
}


void MainWindow::on_actionFloydWarshallMatriceView_triggered()
{
    try {
        // Set the display ID
        displayId = 103;

        // Ignore the rest of the action if there is no matrice loaded
        if (currentMatrice == nullptr) return;

        // Set the displayed matrice to the current one and update the widget
        displayedMatrice = currentMatrice->floydWarshall();
        displayedWidget = new MatriceWidget(displayedMatrice, this);
        setCentralWidget(displayedWidget);
    
    // If there is an error, display the error widget
    } catch (const std::exception& e) {
        displayedWidget = new ErrorWidget(e.what(), this);
        setCentralWidget(displayedWidget);
    }
}


void MainWindow::on_actionClustersMatriceView_triggered()
{
    try {
        // Set the display ID
        displayId = 104;

        // Ignore the rest of the action if there is no matrice loaded
        if (currentMatrice == nullptr) return;

        // Set the displayed matrice to the current one and update the widget
        displayedMatrice = currentMatrice->clusterMatrice();
        displayedWidget = new MatriceWidget(displayedMatrice, this);
        setCentralWidget(displayedWidget);
    
    // If there is an error, display the error widget
    } catch (const std::exception& e) {
        displayedWidget = new ErrorWidget(e.what(), this);
        setCentralWidget(displayedWidget);
    }
}
