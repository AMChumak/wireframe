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
    //temporal set up
    std::vector<Point3D> points = {{0, 0, 0}, {1, 2, 0}, {4, 3, 0}, {6, -1, 0}, {10, 2, 0}};
    for (const auto& point : points)
    {
        spline.addKeyPoint(point);
    }

    //set pens
    axisXPen.setColor(QColor(255, 165, 0));
    axisXPen.setWidth(1);
    axisXPen.setStyle(Qt::DashDotLine);

    axisYPen.setColor(QColor(255, 192, 203));
    axisYPen.setWidth(1);
    axisYPen.setStyle(Qt::SolidLine);

    auxiliaryPen.setColor(Qt::black);
    auxiliaryPen.setWidth(1);
    auxiliaryPen.setStyle(Qt::SolidLine);

    mainPen.setColor(Qt::blue);
    mainPen.setWidth(1);
    mainPen.setStyle(Qt::SolidLine);

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
    QPoint screenCenter = QPoint(this->size().width() / 2, this->size().height() / 2);

    QPainter painter(this);
    //draw background
    painter.drawImage(QRect(0, 0, canvas.width(), canvas.height()), canvas);
    //draw axis Y

    Point3D upperPoint{0, -1.0 * size().height() / zoom + cameraCenter.y, 0};
    upperPoint = (upperPoint - cameraCenter) * zoom;

    Point3D bottomPoint{0, size().height() / zoom + cameraCenter.y, 0};
    bottomPoint = (bottomPoint - cameraCenter) * zoom;

    auto up = Point3DToQPoint(upperPoint) + screenCenter;
    auto bp = Point3DToQPoint(bottomPoint) + screenCenter;

    painter.setPen(axisYPen);
    painter.drawLine(up, bp);

    //draw axis X


    Point3D leftPoint{-1.0 * size().width() / zoom + cameraCenter.x, 0, 0};
    leftPoint = (leftPoint - cameraCenter) * zoom;

    Point3D rightPoint{size().width() / zoom + cameraCenter.x, 0, 0};
    rightPoint = (rightPoint - cameraCenter) * zoom;

    auto lp = Point3DToQPoint(leftPoint) + screenCenter;
    auto rp = Point3DToQPoint(rightPoint) + screenCenter;

    painter.setPen(axisXPen);
    painter.drawLine(lp, rp);


    //draw auxiliary lines
    painter.setPen(auxiliaryPen);
    const auto& keyPoints = spline.keyPoints();
    if (keyPoints.size() > 0)
    {
        Point3D lastKPoint = (keyPoints[0] - cameraCenter) * zoom;
        QPoint lastKeyPoint = Point3DToQPoint(lastKPoint) + screenCenter;
        QPoint firstKPoint = lastKeyPoint;

        for (int i = 1; i < keyPoints.size(); i++)
        {
            Point3D nextKPoint = (keyPoints[i] - cameraCenter) * zoom;
            QPoint nextKeyPoint = Point3DToQPoint(nextKPoint) + screenCenter;

            painter.drawLine(lastKeyPoint, nextKeyPoint);
            if (i == chosenKeyPoint)
            {
                auxiliaryPen.setColor(Qt::green);
                painter.setPen(auxiliaryPen);
            }
            painter.drawArc(QRect{nextKeyPoint - QPoint{6, 6}, nextKeyPoint + QPoint{6, 6},}, 0, 5760); //full circle

            if (i == chosenKeyPoint)
            {
                auxiliaryPen.setColor(Qt::black);
                painter.setPen(auxiliaryPen);
            }
            lastKeyPoint = nextKeyPoint;
        }

        if (chosenKeyPoint == 0)
        {
            auxiliaryPen.setColor(Qt::green);
            painter.setPen(auxiliaryPen);
            painter.drawArc(QRect{firstKPoint - QPoint{6, 6}, firstKPoint + QPoint{6, 6},}, 0, 5760); //full circle
            auxiliaryPen.setColor(Qt::black);
            painter.setPen(auxiliaryPen);
        }
        else
        {
            painter.drawArc(QRect{firstKPoint - QPoint{6, 6}, firstKPoint + QPoint{6, 6},}, 0, 5760); //full circle
        }
    }
    // draw polyline
    painter.setPen(mainPen);
    const auto& pointsV = spline.points();
    Point3D last = pointsV[0];
    for (int i = 1; i < pointsV.size(); ++i)
    {
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
    chosenKeyPoint = -1;

    QPoint screenCenter = QPoint(this->size().width() / 2, this->size().height() / 2);
    if (addVertexMode)
    {
        QPoint newQPoint = (event->pos() - screenCenter) / zoom;
        Point3D newKPoint = QPointToPoint3D(newQPoint);
        spline.addKeyPoint(newKPoint + cameraCenter);
        chosenKeyPoint = spline.keyPoints().size() - 1;
        update();
        emit updatedCntKeyPoints(spline.keyPoints().size());
        emit updatedSpline(spline);
        return;
    }

    auto& keyPoints = spline.keyPoints();
    for (int i = 0; i < keyPoints.size(); i++)
    {
        Point3D curKPoint = (keyPoints[i] - cameraCenter) * zoom;
        QPoint curKeyPoint = Point3DToQPoint(curKPoint) + screenCenter;
        QPoint dist = curKeyPoint - event->pos();
        if (dist.x() * dist.x() + dist.y() * dist.y() <= 36)
        {
            chosenKeyPoint = i;
            break;
        }
    }
    update();
}

void CanvasArea::mouseMoveEvent(QMouseEvent* event)
{
    //Если не попали по точке -> сдвигаем видимую область
    if (mousePressed_ && chosenKeyPoint < 0)
    {
        cameraCenter += QPointToPoint3D(startMovePosition - event->pos()) / zoom;
        startMovePosition = event->pos();
        emit updatedCamera(cameraCenter);
    }
    else if (mousePressed_) //Если попали по точке -> сдвигаем точку
    {
        Point3D updatedPoint = spline.keyPoints()[chosenKeyPoint];
        updatedPoint += QPointToPoint3D(event->pos() - startMovePosition) / zoom;
        spline.updateKeyPoint(chosenKeyPoint, updatedPoint);
        startMovePosition = event->pos();
        emit updatedSpline(spline);
    }
    update();
}

void CanvasArea::mouseReleaseEvent(QMouseEvent* event)
{
    mousePressed_ = false;
}

void CanvasArea::wheelEvent(QWheelEvent* event)
{
    //меняем zoom
    if (event->angleDelta().y() > 0)
        zoom *= 1.1;
    else
        zoom /= 1.1;
    emit updatedZoom(zoom);
    update();
}

void CanvasArea::setAddVertexMode(bool mode)
{
    addVertexMode = mode;
    emit addVertexModeChanged(mode);
}

void CanvasArea::deleteChosenKeyPoint()
{
    if (spline.keyPoints().size() <= 4)
        return;

    if (chosenKeyPoint >= 0)
    {
        spline.removeKeyPoint(chosenKeyPoint);

        //choose point which next to deleted
        int sz = spline.keyPoints().size();
        if (sz > 0)
        {
            if (chosenKeyPoint >= sz)
            {
                chosenKeyPoint = sz - 1;
            }
        }
        else
        {
            chosenKeyPoint = -1;
        }
    }
    update();
    emit updatedCntKeyPoints(spline.keyPoints().size());
    emit updatedSpline(spline);
}

void CanvasArea::updateN(const int &newN)
{
    spline.setCntParts(newN);
    emit updatedSpline(spline);
    update();
}

void CanvasArea::mouseDoubleClickEvent(QMouseEvent* event)
{
    QPoint screenCenter = QPoint(this->size().width() / 2, this->size().height() / 2);
    chosenKeyPoint = -1;
    auto& keyPoints = spline.keyPoints();
    int x = 0, y = 0;
    for (int i = 0; i < keyPoints.size(); i++)
    {
        Point3D curKPoint = (keyPoints[i] - cameraCenter) * zoom;
        QPoint curKeyPoint = Point3DToQPoint(curKPoint) + screenCenter;
        QPoint dist = curKeyPoint - event->pos();
        if (dist.x() * dist.x() + dist.y() * dist.y() <= 36)
        {
            chosenKeyPoint = i;
            x = curKeyPoint.x();
            y = curKeyPoint.y();
            break;
        }
    }
    update();
    if (chosenKeyPoint < 0)
        return;
    emit openedSettingsForVertex(keyPoints[chosenKeyPoint].x, keyPoints[chosenKeyPoint].y, x, y);
}

void CanvasArea::updateChosenKeyPoint(double x, double y)
{
    spline.updateKeyPoint(chosenKeyPoint, x, y, 0);
    emit updatedSpline(spline);
    update();
}

void CanvasArea::updateK(const int &newK)
{
    auto& keyPoints = spline.keyPoints();

    int i = 1;
    while (keyPoints.size() < newK)
    {
        spline.addKeyPoint(keyPoints.back() + Point3D{10.0 * i,0,0});
        ++i;
    }

    while (keyPoints.size() > newK)
    {
        spline.removeKeyPoint(keyPoints.size() - 1);
    }

    emit updatedSpline(spline);
    update();
}

void CanvasArea::zoomIn()
{
    zoom *= 1.1;
    emit updatedZoom(zoom);
    update();
}

void CanvasArea::zoomOut()
{
    zoom /= 1.1;
    emit updatedZoom(zoom);
    update();
}

void CanvasArea::zoomReset()
{
    auto& keyPoints = spline.keyPoints();
    auto& points = spline.points();

    cameraCenter = {0, 0, 0};
    emit updatedCamera(cameraCenter);

    if (keyPoints.size() == 0)
    {
        zoom = 10.0;
        update();
        emit updatedZoom(zoom);
        return;
    }

    Point3D minSize{0, 0, 0};
    double sidesRatio = (double)size().height() / size().width();
    for (int i = 0; i < keyPoints.size(); i++)
    {
        auto point = keyPoints[i];
        if (point.x < 0)
            point.x *= -1;
        if (point.y < 0)
            point.y *= -1;

        if (point.x > minSize.x)
        {
            minSize.x = point.x;
            minSize.y = minSize.x * sidesRatio;
        }
        if (point.y > minSize.y)
        {
            minSize.y = point.y;
            minSize.x = minSize.y / sidesRatio;
        }
    }

    for (int i = 0; i < points.size(); i++)
    {
        auto point = points[i];
        if (point.x < 0)
            point.x *= -1;
        if (point.y < 0)
            point.y *= -1;

        if (point.x > minSize.x)
        {
            minSize.x = point.x;
            minSize.y = minSize.x * sidesRatio;
        }
        if (point.y > minSize.y)
        {
            minSize.y = point.y;
            minSize.x = minSize.y / sidesRatio;
        }
    }
    zoom = size().width() / (2 * minSize.x) / 1.2;
    emit updatedZoom(zoom);
    update();
}

void CanvasArea::setSpline(BSpline spline)
{
    this->spline = spline;
    emit updatedSpline(spline);
    update();
}

void CanvasArea::setCamera(const Point3D& camera, const double zoom)
{
    this->cameraCenter = camera;
    this->zoom = zoom;
    emit updatedCamera(cameraCenter);
    emit updatedZoom(zoom);
    update();
}
