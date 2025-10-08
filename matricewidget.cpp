#include "matricewidget.h"

#include <QPainter>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QBrush>
#include <QFont>

MatriceWidget::MatriceWidget(const Matrice* data, QWidget *parent):
    QWidget{parent},
    matrice{data}
{
    // Création du tableau
    auto* layout = new QVBoxLayout(this);
    table = new QTableWidget(this);

    int size = matrice->getSize();
    table->setRowCount(size);
    table->setColumnCount(size);

    // Définit les noms de colonnes et lignes
    QStringList headers;
    for (int i = 0; i < size; ++i)
        headers << QString::fromStdString(matrice->getName(i));
    table->setHorizontalHeaderLabels(headers);
    table->setVerticalHeaderLabels(headers);

    // Appliquer un style plus grand et en gras pour les en-têtes
    QFont headerFont = table->horizontalHeader()->font();
    headerFont.setBold(true);
    headerFont.setPointSize(headerFont.pointSize() + 2);
    table->horizontalHeader()->setFont(headerFont);
    table->verticalHeader()->setFont(headerFont);

    // Remplir les cases
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            uint16_t value = matrice->getEdge(i, j);
            auto* item = new QTableWidgetItem(value == UINT16_MAX ? "-" : QString::number(value));

            // Couleur selon la valeur
            if (value == 0 || value == UINT16_MAX)
                item->setBackground(QBrush(QColor(96, 24, 24)));
            else
                item->setBackground(QBrush(QColor(32, 96, 32)));

            item->setTextAlignment(Qt::AlignCenter);
            table->setItem(i, j, item);
        }
    }

    // Ajuster la taille des colonnes et lignes
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    layout->addWidget(table);
    setLayout(layout);
}
