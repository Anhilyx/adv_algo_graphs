#include "matricewidget.h"

#include <QBrush>
#include <QFont>
#include <QHeaderView>
#include <QPainter>
#include <QVBoxLayout>

MatriceWidget::MatriceWidget(const Matrice* data, QWidget *parent):
    QWidget{parent},
    matrice{data}
{
    // Create layout and table
    auto* layout = new QVBoxLayout(this);
    table = new QTableWidget(this);
    uint32_t size = matrice->getSize();
    table->setRowCount(size);
    table->setColumnCount(size);

    // Set column and row names
    QStringList headers;
    for (uint32_t i = 0; i < size; ++i)
        headers << QString::fromStdString(matrice->getName(i));
    table->setHorizontalHeaderLabels(headers);
    table->setVerticalHeaderLabels(headers);

    // Highlight headers
    QFont headerFont = table->horizontalHeader()->font();
    headerFont.setBold(true);
    headerFont.setPointSize(headerFont.pointSize() + 2);
    table->horizontalHeader()->setFont(headerFont);
    table->verticalHeader()->setFont(headerFont);

    // Fill cells
    for (uint32_t i = 0; i < size; i++) {
        for (uint32_t j = 0; j < size; j++) {
            int64_t value = matrice->getEdge(i, j);
            auto* item = new QTableWidgetItem(value == INT64_MIN
                ? "-"
                : QString::number(value)
            );

            // Set background color according to value
            if (value == 0 || value == INT64_MIN)
                item->setBackground(QBrush(*colorRed));
            else
                item->setBackground(QBrush(*colorGreen));

            // Add item to table and center text
            item->setTextAlignment(Qt::AlignCenter);
            table->setItem(i, j, item);
        }
    }

    // Automatically adjust column and row sizes
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Add table to layout (and widget) (redraw is handled by the QTableWidget itself)
    layout->addWidget(table);
    setLayout(layout);
}

MatriceWidget::~MatriceWidget()
{
    // Free the resources
    delete colorRed;
    delete colorGreen;
}