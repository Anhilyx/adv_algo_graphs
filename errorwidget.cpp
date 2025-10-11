#include "errorwidget.h"

ErrorWidget::ErrorWidget(const QString& message, QWidget* parent):
    QWidget{parent},
    errorMessage{message}
{}

void ErrorWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QColor(192, 32, 32));

    // Draw "Invalid Graph" in the center of the widget
    QFont font = painter.font();
    font.setPointSize(TITLE_FONT_SIZE);
    painter.setFont(font);
    QRect rect = this->rect();
    painter.drawText(rect, Qt::AlignCenter, "Invalid Graph");

    // Draw the error message in smaller font below the title
    font.setPointSize(SUBTITLE_FONT_SIZE);
    painter.setFont(font);
    QRect subtitleRect = rect;
    subtitleRect.setTop(rect.center().y() + TITLE_FONT_SIZE);
    painter.drawText(subtitleRect, Qt::AlignHCenter | Qt::AlignTop, errorMessage);
}
