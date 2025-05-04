#pragma once


#include <QWidget>
#include <QPen>

#include "Polyline3D.h"


QPoint Point3DToQPoint(const Point3D& point);
Point3D QPointToPoint3D(const QPoint& point);

class CanvasArea : public QWidget
{
    Q_OBJECT

public:
    explicit CanvasArea(QWidget* parent = nullptr);

public slots:
    void resizeEvent(QResizeEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:

private:
    //elements of cannvas
    QImage canvas;
    QPen axisXPen;
    QPen axisYPen;
    QPen mainPen;
    QPen auxiliaryPen;

    //elements of camera
    Point3D cameraCenter{};
    double zoom = 10.0;

    //interactive objects
    int chosenKeyPoint = -1;
    QPoint startMovePosition;

    //interactive flags
    bool mousePressed_{};


};
