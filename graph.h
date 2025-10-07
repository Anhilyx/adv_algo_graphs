#ifndef GRAPH_H
#define GRAPH_H

#include "matrice.h"

#include <QWidget>
#include <QPainter>

#define NODE_SIZE 40
#define NODE_SPACING 100

/**
 * @brief A widget to display a graph
 */
class Graph : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Construct a new Graph object
     * @param parent The parent widget
     */
    explicit Graph(Matrice* data, QWidget *parent = nullptr);

    /**
     * @brief Paint event handler
     * @param event The paint event
     */
    void paintEvent(QPaintEvent* event) override;

private:
    Matrice matrice;

signals:
};

#endif // GRAPH_H
