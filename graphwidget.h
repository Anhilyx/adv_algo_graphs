#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include "matrice.h"

#include <QWidget>
#include <QPainter>

#define NODE_SIZE 40
#define NODE_SPACING 100

/**
 * @brief A widget to display a Graph
 */
class GraphWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Construct a new GraphWidget object
     * @param parent The parent widget
     */
    explicit GraphWidget(const Matrice* data, QWidget *parent = nullptr);

    /**
     * @brief Paint event handler
     * @param event The paint event
     */
    void paintEvent(QPaintEvent* event) override;

private:
    const Matrice* matrice;

signals:
};

#endif // GRAPHWIDGET_H
