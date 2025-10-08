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
     * @param parent The parent widget
     */
    MatriceWidget(const Matrice* matrice, QWidget* parent = nullptr);

private:
    const Matrice* matrice;
    QTableWidget* table;

signals:
};

#endif // MATRICEWIDGET_H
