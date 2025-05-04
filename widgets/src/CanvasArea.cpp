#include "CanvasArea.h"

#include <iostream>
#include <QPainter>
#include <QResizeEvent>

#include "BSpline.h"
#include "Polyline3D.h"


QPoint Point3DToQPoint(const Point3D& point)
{
    return {static_cast<int>(point.x), static_cast<int>(point.y)};
}

Point3D QPointToPoint3D(const QPoint& point)
{
    Point3D point3D{};
    point3D.x = point.x();
    point3D.y = point.y();
    return point3D;
}

CanvasArea::CanvasArea(QWidget* parent)
{
    setMouseTracking(true);
    setParent(parent);
}

void CanvasArea::resizeEvent(QResizeEvent* event)
{
    canvas = QImage(event->size().width(), event->size().height(), QImage::Format_ARGB32);
    canvas.fill(Qt::white);
    resize(canvas.width(), canvas.height());
    update();
    QWidget::resizeEvent(event);
}

void CanvasArea::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine));
    painter.drawImage(QRect(0, 0, canvas.width(), canvas.height()), canvas);
    std::vector<Point3D> points = {{0, 0, 0}, {1, 2, 0}, {4, 3, 0}, {6, -1, 0}, {10, 2, 0}};
    BSpline spline(points);
    spline.setCntParts(100);
    const auto& pointsV = spline.points();
    Point3D last = pointsV[0];
    for (int i = 1; i < pointsV.size(); ++i)
    {
        QPoint screenCenter = QPoint(this->size().width() / 2, this->size().height() / 2);

        Point3D lastPoint = (last - cameraCenter) * zoom;
        auto sp = Point3DToQPoint(lastPoint) + screenCenter;
        Point3D nextPoint = (pointsV[i] - cameraCenter) * zoom;
        auto fp = Point3DToQPoint(nextPoint) + screenCenter;
        painter.drawLine(sp, fp);
        last = pointsV[i];
    }
    painter.end();
    QWidget::paintEvent(event);
}


void CanvasArea::mousePressEvent(QMouseEvent* event)
{
    mousePressed_ = true;
    startMovePosition = event->pos();

    //todo проверить не попали ли мы в ключевую точку
    //попали левой клавишей -Ю двигаем точку
    //попали правой - открывается меню параметров точки
}

void CanvasArea::mouseMoveEvent(QMouseEvent* event)
{
    //Если не попали по точке -> сдвигаем видимую область
    if (mousePressed_ && chosenKeyPoint < 0)
    {
        cameraCenter += QPointToPoint3D(startMovePosition - event->pos()) / zoom;
        startMovePosition = event->pos();
    }
    else if (mousePressed_)
    {
        //Если попали по точке -> сдвигаем точку
    }
    update();
}

void CanvasArea::mouseReleaseEvent(QMouseEvent* event)
{
    mousePressed_ = false;
}

void CanvasArea::wheelEvent(QWheelEvent* event)
{
    //todo меняем zoom
    if (event->angleDelta().y() > 0)
    {
        zoom *= 1.1;
    } else
    {
        zoom /= 1.1;
    }
    update();
}