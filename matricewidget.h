#ifndef MATRICEWIDGET_H
#define MATRICEWIDGET_H

#include "matrice.h"

#include <QTableWidget>
#include <QWidget>

/**
 * @brief A widget to display a Matrice
 */
class MatriceWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Construct a new MatriceWidget object
     * @param matrice The matrice to display
     * @param parent The parent widget
     */
    MatriceWidget(const Matrice* matrice, QWidget* parent = nullptr);

    /**
     * @brief Destroy the MatriceWidget object
     */
    ~MatriceWidget();

private:
    /**
     * @brief The matrice to display
     */
    const Matrice* matrice;

    /**
     * @brief The table widget to display the matrice
     */
    QTableWidget* table;

    /**
     * @brief Color for zero (or infinite, aka INT64_MAX) values (avoid defining it multiple times)
     */
    QColor* colorRed = new QColor(QColor::fromHsv(0, 192, 64));

    /**
     * @brief Color for non-zero values (avoid defining it multiple times)
     */
    QColor* colorGreen = new QColor(QColor::fromHsv(120, 192, 64));
};

#endif // MATRICEWIDGET_H
