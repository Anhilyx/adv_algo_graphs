#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "matrice.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Construct a new Main Window object
     * @param parent The parent widget
     */
    MainWindow(Matrice* matrice, QWidget *parent = nullptr);
    
    /**
     * @brief Destroy the Main Window object
     */
    ~MainWindow();

private slots:
    /**
     * @brief Slot for the "Import matrice" action
     */
    void on_actionImport_matrice_triggered();

    /**
     * @brief Slot for the "Save Graph" action
     */
    void on_actionSave_Graph_triggered();

    /**
     * @brief Slot for the "Exit" action
     */
    void on_actionExit_triggered();

    /**
     * @brief Slot for the "Basic Graph" view
     */
    void on_actionGraph_triggered();

    /**
     * @brief Slot for the "Clusters Graph" view
     */
    void on_actionClusters_triggered();

    /**
     * @brief Slot for the "Basic Matrice" view
     */
    void on_actionMatrice_triggered();

    /**
     * @brief Slot for the "Floyd-Warshall Matrice" view
     */
    void on_actionFloyd_Warshall_triggered();

    /**
     * @brief Slot for the "Cluster Matrice" view
     */
    void on_actionCluster_Matrice_triggered();

private:
    Ui::MainWindow *ui;
    const Matrice* currentMatrice;
};
#endif // MAINWINDOW_H
