#include "graphwidget.h"

GraphWidget::GraphWidget(const Matrice* data, QWidget* parent):
    QWidget{parent},
    matrice{data}
{
    // Compute strongly connected components
    clusters = matrice->kosaraju();

    // Compute the size of each cluster, as well as the graph radius
    uint32_t nbClusters = clusters.size();
    double graphPerimeter = 0;
    double* clustersRadii = new double[nbClusters];
    for (uint32_t i = 0; i < nbClusters; i++) {
        clustersRadii[i] = (NODE_SIZE + NODE_SPACING) * clusters[i].size() / M_PI / 2;
        graphPerimeter += clustersRadii[i] * 2 + NODE_SPACING;
    }
    double graphRadius = graphPerimeter / M_PI / 2;

    // Compute positions and colors of nodes
    nodes = new Node[matrice->getSize()];
    double offset = 0;
    for (uint32_t i = 0; i < nbClusters; i++) {
        // Compute the position of the center of the cluster
        offset += clustersRadii[i] + NODE_SPACING / 2;
        double angle = 2 * M_PI * offset / graphPerimeter;
        QPointF clusterPos(
            graphRadius * cos(angle),
            graphRadius * sin(angle)
        );
        offset += clustersRadii[i] + NODE_SPACING / 2;

        // Compute the position & color of each node in the cluster
        QColor* color = new QColor(QColor::fromHsv((i * 50) % 360, 192, 160));  // Offset by 50° allow for 36 distinct colors, with a good difference between each and a still good unity of colors
        for (uint32_t j = 0; j < clusters[i].size(); j++) {
            uint32_t nodeIndex = clusters[i][j];
            angle = 2 * M_PI * j / clusters[i].size();
            nodes[nodeIndex].position = QPointF(
                clusterPos.x() + clustersRadii[i] * cos(angle),
                clusterPos.y() + clustersRadii[i] * sin(angle)
            );

            // Assign a color based on the cluster index
            nodes[nodeIndex].color = color;
        }
    }

    // Free the resources
    delete[] clustersRadii;
}

GraphWidget::~GraphWidget()
{
    // Free the resources
    for (unsigned long long i = 0; i < clusters.size(); i++) {
        delete nodes[clusters[i][0]].color;
    }
    delete[] nodes;
}

void GraphWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Compute the center of the widget (the graph positions are based on (0;0) and not on the center of the widget)
    int centerX = width() / 2;
    int centerY = height() / 2;

    // Define the pen for drawing edges
    QPen pen(Qt::white);
    pen.setWidth(EDGE_SIZE);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);

    // Draw each edge
    for (uint32_t i = 0; i < matrice->getSize(); i++)
    {
        for (uint32_t j = 0; j < matrice->getSize(); j++)
        {
            // Skip if no edge
            if (matrice->getEdge(i, j) == 0) continue;

            // Compute the positions of the start and end of the edge (offset by the center of the widget)
            QPointF from = nodes[i].position + QPointF(centerX, centerY);
            QPointF to = nodes[j].position + QPointF(centerX, centerY);

            // Create the line (and skip if its length is 0)
            QLineF line(from, to);
            if (line.length() == 0) continue;

            // Offset the line to avoid overlapping the nodes
            double offset = NODE_SIZE / 2.0;
            line.setP1(line.pointAt(offset / line.length()));
            line.setP2(line.pointAt(1.0 - offset / line.length()));

            // Draw the line
            painter.drawLine(line);

            // Draw the arrow
            double angle = std::atan2(-line.dy(), line.dx());
            QPointF arrowP1 = line.p2() - QPointF(
                std::cos(angle + M_PI / 6) * ARROW_SIZE,
                -std::sin(angle + M_PI / 6) * ARROW_SIZE
            );
            QPointF arrowP2 = line.p2() - QPointF(
                std::cos(angle - M_PI / 6) * ARROW_SIZE,
                -std::sin(angle - M_PI / 6) * ARROW_SIZE
            );
            QPolygonF arrowHead;
            arrowHead << line.p2() << arrowP1 << arrowP2;
            painter.setBrush(Qt::white);
            painter.drawPolygon(arrowHead);
            painter.setBrush(Qt::NoBrush);
        }
    }

    // Draw each node
    for (uint32_t i = 0; i < matrice->getSize(); i++)
    {
        // Compute the position of the node (offset by the center of the widget)
        const auto &node = nodes[i];
        QPointF pos = node.position + QPointF(centerX, centerY);

        // Draw the node circle
        painter.setBrush(*node.color);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(pos, NODE_SIZE / 2.0, NODE_SIZE / 2.0);

        // Draw the node name (in white, centered)
        painter.setPen(Qt::white);
        painter.setBrush(Qt::NoBrush);
        QString name = QString::fromStdString(matrice->getName(i));
        QRectF textRect(
            pos.x() - NODE_SIZE, pos.y() - NODE_SIZE / 2.0,
            NODE_SIZE * 2, NODE_SIZE
        );
        painter.drawText(textRect, Qt::AlignCenter, name);
    }
}

void GraphWidget::mousePressEvent(QMouseEvent* event)
{
    // Retrieve the adjusted position of the mouse
    int centerX = width() / 2;
    int centerY = height() / 2;
    QPointF mousePos = event->pos() - QPointF(centerX, centerY);  // Graph coordinates are based on (0;0) and not on the center of the widget

    // Check if a node is under the mouse
    for (uint32_t i = 0; i < matrice->getSize(); i++) {
        QPointF nodePos = nodes[i].position;
        double distance = std::hypot(mousePos.x() - nodePos.x(), mousePos.y() - nodePos.y());  // Distance between the mouse and the node center

        // If the distance is less than the node radius, the node and its offset are stored
        if (distance <= NODE_SIZE / 2.0) {
            targetNode = &nodes[i];
            offset = mousePos - nodePos;
            break;
        }
    }
}

void GraphWidget::mouseReleaseEvent(QMouseEvent* event)
{
    Q_UNUSED(event);

    // Stop moving the node
    targetNode = nullptr;
}

void GraphWidget::mouseMoveEvent(QMouseEvent* event)
{
    // Skip if no node is being moved
    if (targetNode == nullptr) return;

    // Move the node to the mouse position
    int centerX = width() / 2;
    int centerY = height() / 2;
    QPointF mousePos = event->pos() - QPointF(centerX, centerY);  // Graph coordinates are based on (0;0) and not on the center of the widget
    targetNode->position = mousePos - offset;
    update();
}
