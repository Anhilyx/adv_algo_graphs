#include "graphwidget.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

GraphWidget::GraphWidget(const Matrice* data, QWidget* parent):
    QWidget{parent},
    matrice{data}
{
    // Compute strongly connected components
    clusters = matrice->kosaraju();

    // Compute the positions and colors of the nodes
    nodes = nullptr;
    computeNodes();

    // Create zoom slider
    zoomSlider = new QSlider(Qt::Horizontal, this);
    zoomSlider->setRange(MIN_ZOOM, MAX_ZOOM);
    zoomSlider->setValue(100);
    zoomSlider->setFixedWidth(150);
    zoomSlider->setStyleSheet(R"(
        QSlider::groove:horizontal {
            height: 6px;
            background: #666;
            border-radius: 3px;
        }
        QSlider::handle:horizontal {
            background: #fff;
            width: 14px;
            border-radius: 7px;
            margin: -4px 0;
        }
    )");

    // Create zoom slider label
    QHBoxLayout* zoomLayout = new QHBoxLayout();
    zoomLayout->addWidget(zoomSlider);
    zoomLayout->setAlignment(Qt::AlignRight | Qt::AlignTop);
    zoomLayout->setContentsMargins(0, 0, 10, 10);  // Margin to the right and bottom

    // Main layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(zoomLayout);
    mainLayout->addStretch();
    setLayout(mainLayout);

    // Bind zoom to the slider
    connect(zoomSlider, &QSlider::valueChanged, this, [this](int value) {
        zoom = static_cast<float>(value) / 100.0;
        computeNodes();
        update();
    });
}

GraphWidget::~GraphWidget()
{
    // Free the resources
    /* if (matrice != nullptr) {
        for (unsigned long long i = 0; i < clusters.size(); i++) {
            delete nodes[clusters[i][0]].color;
        }
        delete[] nodes;
    } */
}

void GraphWidget::forScreenshot(bool screenshot)
{
    zoomSlider->setVisible(!screenshot);
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
            QPointF from = nodes[i].position + QPointF(centerX, centerY) + graphOffset;
            QPointF to = nodes[j].position + QPointF(centerX, centerY) + graphOffset;

            // Create the line (and skip if its length is 0)
            QLineF line(from, to);
            if (line.length() == 0) continue;

            // Offset the line to avoid overlapping the nodes
            double offset = NODE_SIZE * zoom / 2.0;
            line.setP1(line.pointAt(offset / line.length()));
            line.setP2(line.pointAt(1.0 - offset / line.length()));

            // Draw the line
            painter.drawLine(line);

            // Draw the arrow if the graph is directed
            if (matrice->isOriented()) {
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

            // Draw the edge weight near the middle of the edge, offset perpendicularly
            if (matrice->isWeighted()) {
                QString weight = QString::number(matrice->getEdge(i, j));
                // Use a smaller font for edge weights
                QFont f = painter.font();
                f.setPointSizeF(f.pointSizeF() - 2.0);
                painter.setFont(f);

                // Retrieve the middle of the line
                QPointF mid = line.pointAt(0.5);

                // Compute the perpendicular vector to the line
                double lx = line.dx();
                double ly = line.dy();
                double llen = std::hypot(lx, ly);
                QPointF perp(0, 0);
                if (llen != 0) {
                    perp = QPointF(-ly / llen, lx / llen);
                }

                // Offset the label a little so it does not overlap the edge
                const double TEXT_OFFSET = 10.0;
                QPointF textPos = mid + perp * TEXT_OFFSET;

                // Draw text centered on that position
                QRectF textRect(textPos.x() - 20.0, textPos.y() - 10.0, 40.0, 20.0);
                painter.drawText(textRect, Qt::AlignCenter, weight);

                // Restore the base font
                f.setPointSizeF(f.pointSizeF() + 2.0);
                painter.setFont(f);
            }
        }
    }

    // Draw each node
    for (uint32_t i = 0; i < matrice->getSize(); i++)
    {
        // Compute the position of the node (offset by the center of the widget)
        const auto &node = nodes[i];
        QPointF pos = node.position + QPointF(centerX, centerY) + graphOffset;

        // Draw the node circle
        painter.setBrush(*node.color);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(pos, NODE_SIZE * zoom / 2.0, NODE_SIZE * zoom/ 2.0);

        // Draw the node name (in white, centered)
        painter.setPen(Qt::white);
        painter.setBrush(Qt::NoBrush);
        QString name = QString::fromStdString(matrice->getName(i));
        QRectF textRect(
            pos.x() - NODE_SIZE * zoom, pos.y() - NODE_SIZE * zoom / 2.0,
            NODE_SIZE * zoom * 2, NODE_SIZE * zoom
        );
        painter.drawText(textRect, Qt::AlignCenter, name);
    }
}

void GraphWidget::mousePressEvent(QMouseEvent* event)
{
    // Skip if not a left click
    if (event->button() != Qt::LeftButton) return;

    // Set the cursor to a closed hand
    setCursor(Qt::ClosedHandCursor);

    // Retrieve the adjusted position of the mouse
    int centerX = width() / 2;
    int centerY = height() / 2;
    QPointF mousePos = event->pos() - QPointF(centerX, centerY) - graphOffset;  // Graph coordinates are based on (0;0) and not on the center of the widget

    // Check if a node is under the mouse
    for (uint32_t i = 0; i < matrice->getSize(); i++) {
        QPointF nodePos = nodes[i].position;
        double distance = std::hypot(mousePos.x() - nodePos.x(), mousePos.y() - nodePos.y());  // Distance between the mouse and the node center

        // If the distance is less than the node radius, the node and its offset are stored
        if (distance <= NODE_SIZE * zoom / 2.0) {
            targetNode = &nodes[i];
            offset = mousePos - nodePos;
            return;
        }
    }

    // If no node was found, offset the entire graph
    mousePosStart = event->pos();
    graphOffsetStart = new QPointF(graphOffset);
}

void GraphWidget::mouseReleaseEvent(QMouseEvent* event)
{
    // Skip if not a left click
    if (event->button() != Qt::LeftButton) return;

    // Stop moving the node
    targetNode = nullptr;

    // Stop moving the graph
    if (graphOffsetStart != nullptr) {
        delete graphOffsetStart;
        graphOffsetStart = nullptr;
    }

    // Reset the cursor
    setCursor(Qt::ArrowCursor);
}

void GraphWidget::mouseMoveEvent(QMouseEvent* event)
{
    // Skip if no node nor graph is being moved
    if (targetNode == nullptr && graphOffsetStart == nullptr) return;

    // Move the node to the mouse position
    if (targetNode) {
        int centerX = width() / 2;
        int centerY = height() / 2;
        QPointF mousePos = event->pos() - QPointF(centerX, centerY) - graphOffset;  // Graph coordinates are based on (0;0) and not on the center of the widget
        targetNode->position = mousePos - offset;
        update();
    }

    // Move the graph based on the mouse movement
    if (graphOffsetStart != nullptr) {
        QPointF delta = event->pos() - mousePosStart;
        graphOffset = *graphOffsetStart + delta;
        update();
    }
}

void GraphWidget::wheelEvent(QWheelEvent *event)
{
    // Zoom in or out based on the wheel delta
    int delta = event->angleDelta().y();
    if (delta > 0) zoom *= ZOOM_MULTIPLIER;
    else if (delta < 0)  zoom /= ZOOM_MULTIPLIER;

    // Clamp the zoom level
    if (zoom < static_cast<float>(MIN_ZOOM) / 100.0f) zoom = static_cast<float>(MIN_ZOOM) / 100.0f;
    if (zoom > static_cast<float>(MAX_ZOOM) / 100.0f) zoom = static_cast<float>(MAX_ZOOM) / 100.0f;

    // Recompute node positions and update the display
    zoomSlider->setValue(static_cast<int>(zoom * 100));
    computeNodes();
    update();
}

void GraphWidget::computeNodes()
{
    // Compute the size of each cluster, as well as the graph radius
    uint32_t nbClusters = clusters.size();
    double graphPerimeter = 0;
    double* clustersRadii = new double[nbClusters];
    for (uint32_t i = 0; i < nbClusters; i++) {
        clustersRadii[i] = (NODE_SIZE * zoom + NODE_SPACING * zoom) * clusters[i].size() / M_PI / 2;
        graphPerimeter += clustersRadii[i] * 2 + NODE_SPACING * zoom;
    }
    double graphRadius = graphPerimeter / M_PI / 2;

    // Compute positions and colors of nodes
    nodes = new Node[matrice->getSize()];
    double offset = 0;
    for (uint32_t i = 0; i < nbClusters; i++) {
        // Compute the position of the center of the cluster
        offset += clustersRadii[i] + NODE_SPACING * zoom / 2;
        double angle = 2 * M_PI * offset / graphPerimeter;
        QPointF clusterPos(
            graphRadius * cos(angle),
            graphRadius * sin(angle)
        );
        offset += clustersRadii[i] + NODE_SPACING * zoom / 2;

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
