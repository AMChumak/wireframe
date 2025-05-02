#include "CanvasArea.h"

#include <QPainter>
#include <QResizeEvent>

CanvasArea::CanvasArea(QWidget* parent)
{
    setMouseTracking(true);
    setParent(parent);
}

void CanvasArea::resizeEvent(QResizeEvent* event)
{
    canvas = QImage(event->size().width(), event->size().height(), QImage::Format_ARGB32);
    canvas.fill(Qt::lightGray);
    resize(canvas.width(), canvas.height());
    update();
    QWidget::resizeEvent(event);
}

void CanvasArea::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.drawImage(QRect(0, 0, canvas.width(), canvas.height()), canvas);
    QWidget::paintEvent(event);
    painter.end();
}
