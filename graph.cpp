#include "graph.h"

Graph::Graph(Matrice* data, QWidget *parent):
    QWidget{parent},
    matrice{*data}
{}

void Graph::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int w = width();
    int h = height();
    int radius = std::min(w, h) / 2 - 50; // rayon pour disposer les nodes
    int nodeRadius = 20;                  // rayon des nodes

    uint16_t n = matrice.getSize();           // nombre de nodes
    std::vector<QPointF> positions(n);

    // 1️⃣ Calculer la position de chaque node (layout circulaire)
    for (uint16_t i = 0; i < n; ++i) {
        double angle = 2 * M_PI * i / n;
        positions[i] = QPointF(w / 2 + radius * cos(angle),
                               h / 2 + radius * sin(angle));
    }

    // 2️⃣ Récupérer les composantes fortement connexes
    auto clusters = matrice.kosaraju();

    // 3️⃣ Dessiner les edges
    painter.setPen(QPen(Qt::white, 2));
    for (uint16_t i = 0; i < n; ++i) {
        for (uint16_t j = 0; j < n; ++j) {
            if (matrice.getEdge(i, j) != 0) {
                QPointF p1 = positions[i];
                QPointF p2 = positions[j];

                // Déplacer le point de départ et d'arrivée pour ne pas chevaucher le node
                double dx = p2.x() - p1.x();
                double dy = p2.y() - p1.y();
                double len = sqrt(dx*dx + dy*dy);
                QPointF start = p1 + QPointF(dx / len * nodeRadius, dy / len * nodeRadius);
                QPointF end   = p2 - QPointF(dx / len * nodeRadius, dy / len * nodeRadius);

                // Dessiner une flèche simple
                painter.drawLine(start, end);

                // Dessiner la pointe de la flèche
                double arrowSize = 10;
                double angle = atan2(dy, dx);
                QPointF arrowP1 = end - QPointF(arrowSize * cos(angle - M_PI / 6),
                                                arrowSize * sin(angle - M_PI / 6));
                QPointF arrowP2 = end - QPointF(arrowSize * cos(angle + M_PI / 6),
                                                arrowSize * sin(angle + M_PI / 6));
                QPolygonF arrowHead;
                arrowHead << end << arrowP1 << arrowP2;
                painter.setBrush(Qt::white);
                painter.drawPolygon(arrowHead);
            }
        }
    }

    // 4️⃣ Dessiner les nodes
    for (uint16_t i = 0; i < n; ++i) {
        // Déterminer la couleur selon la composante
        QColor color = Qt::gray;
        for (size_t c = 0; c < clusters.size(); ++c) {
            if (std::find(clusters[c].begin(), clusters[c].end(), i) != clusters[c].end()) {
                color = QColor::fromHsv((c * 50) % 360, 200, 255); // couleur par cluster
                break;
            }
        }

        painter.setBrush(color);
        painter.setPen(QPen(Qt::black, 2));
        painter.drawEllipse(positions[i], nodeRadius, nodeRadius);

        // Ajouter le nom du node
        painter.setPen(Qt::black);
        painter.drawText(QRectF(positions[i].x() - nodeRadius, positions[i].y() - nodeRadius,
                                2 * nodeRadius, 2 * nodeRadius),
                         Qt::AlignCenter, QString::fromStdString(matrice.getName(i)));
    }
}
