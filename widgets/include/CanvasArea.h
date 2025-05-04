#pragma once


#include <QWidget>
#include <QPen>

#include "BSpline.h"
#include "Polyline3D.h"


QPoint Point3DToQPoint(const Point3D& point);
Point3D QPointToPoint3D(const QPoint& point);

class CanvasArea : public QWidget
{
    Q_OBJECT

public:
    explicit CanvasArea(QWidget* parent = nullptr);

signals:
    void addVertexModeChanged(bool mode);

public slots:
    void resizeEvent(QResizeEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void setAddVertexMode(bool mode);
    void deleteChosenKeyPoint();
    void updateN(int newN);

private:
    //elements of cannvas
    QImage canvas;
    QPen axisXPen;
    QPen axisYPen;
    QPen mainPen;
    QPen auxiliaryPen;
    BSpline spline;

    //elements of camera
    Point3D cameraCenter{};
    double zoom = 10.0;

    //interactive objects
    int chosenKeyPoint = -1;
    QPoint startMovePosition;

    //interactive flags
    bool mousePressed_{};
    bool addVertexMode{};


};

