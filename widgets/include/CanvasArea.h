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
    void zoomIn();
    void zoomOut();
    void zoomReset();

    void setSpline(BSpline spline);
    void setCamera(const Point3D& camera, double zoom);

signals:
    void addVertexModeChanged(bool mode);
    void openedSettingsForVertex(double x, double y, int j, int i);
    void updatedCntKeyPoints(int cnt);
    void updatedSpline(BSpline spline);
    void updatedCamera(Point3D camera);
    void updatedZoom(double zoom);

public slots:
    void resizeEvent(QResizeEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void setAddVertexMode(bool mode);
    void deleteChosenKeyPoint();
    void updateN(const int &newN);
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void updateChosenKeyPoint(double x, double y);
    void updateK(const int &newK);


private:
    //elements of canvas
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

