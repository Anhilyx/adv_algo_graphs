#ifndef ERRORWIDGET_H
#define ERRORWIDGET_H

#include <QPainter>
#include <QWidget>

#define TITLE_FONT_SIZE 32
#define SUBTITLE_FONT_SIZE 16

/**
 * @brief A widget to display when there is an error in the graph.
 */
class ErrorWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs an ErrorWidget with the given error message.
     * @param message The error message to display.
     * @param parent The parent widget (default is nullptr).
     */
    ErrorWidget(const QString& message, QWidget* parent = nullptr);

protected:
    /**
     * @brief Paints the error message on the widget.
     * @param event The paint event.
     */
    void paintEvent(QPaintEvent* event) override;

private:
    /**
     * @brief The error message to display.
     */
    QString errorMessage;
};

#endif // ERRORWIDGET_H
