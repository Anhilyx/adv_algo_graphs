#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include "matrice.h"

#include <QMouseEvent>
#include <QWheelEvent>
#include <QPainter>
#include <QSlider>
#include <QWidget>

#define ARROW_SIZE 10
#define EDGE_SIZE 2
#define NODE_SIZE 40
#define NODE_SPACING 100

#define MIN_ZOOM 20  // Minimum zoom level (in percent)
#define MAX_ZOOM 300  // Maximum zoom level (in percent)
#define ZOOM_MULTIPLIER 1.1f  // Multiplier for each wheel step

/**
 * @brief A widget to display a Graph
 */
class GraphWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Construct a new GraphWidget object
     * @param data The matrice representing the graph
     * @param parent The parent widget
     */
    explicit GraphWidget(const Matrice* data, QWidget* parent = nullptr);

    /**
     * @brief Destroy the GraphWidget object
     */
    ~GraphWidget() override;

protected:
    /**
     * @brief Paint event handler
     * @param event The paint event (unused)
     */
    void paintEvent(QPaintEvent* event) override;

    /**
     * @brief Mouse down event handler, to allow moving nodes
     * @param event The mouse event
     */
    void mousePressEvent(QMouseEvent* event) override;
    
    /**
     * @brief Mouse up event handler, to allow moving nodes
     * @param event The mouse event (unused)
     */
    void mouseReleaseEvent(QMouseEvent* event) override;
    
    /**
     * @brief Mouse move event handler, to allow moving nodes
     * @param event The mouse event
     */
    void mouseMoveEvent(QMouseEvent* event) override;

    /**
     * @brief Mouse wheel event handler, to allow zooming
     * @param event The mouse wheel event
     */
    void wheelEvent(QWheelEvent* event) override;

private:
    /**
     * @brief Stores graphical information about a node
     */
    struct Node {
        /**
         * @brief The position of the node
         */
        QPointF position;

        /**
         * @brief A pointer to the color of the node (usually, multiple nodes will share the same color)
         */
        QColor* color;
    };

    /**
     * @brief Compute the positions and colors of the nodes
     */
    void computeNodes();
    
    /**
     * @brief The matrice representing the graph
     */
    const Matrice* matrice;

    /**
     * @brief The zoom slider
     */
    QSlider* zoomSlider;

    /**
     * @brief The clusters in the matrice (a list of list of node indexes)
     */
    std::vector<std::vector<uint32_t>> clusters;

    /**
     * @brief The data of the nodes
     */
    Node* nodes;

    /**
    * @brief A pointer to the node being moved, or nullptr if no node is being moved
    */
   Node* targetNode = nullptr;

    /**
     * @brief The offset between the mouse position and the center of the node being moved
     */
   QPointF offset;

    /**
     * @brief The zoom level of the graph
     */
   float zoom = 1.0f;

    /**
     * @brief The offset of the graph (to allow moving the entire graph)
     */
   QPointF graphOffset = QPointF(0, 0);

    /**
     * @brief A pointer to the starting offset of the graph when moving it, or nullptr if the graph is not being moved
     */
   QPointF* graphOffsetStart = nullptr;

    /**
     * @brief The starting position of the mouse when moving the graph
     */
   QPointF mousePosStart;
};

#endif // GRAPHWIDGET_H
