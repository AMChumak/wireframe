#pragma once

#include <QPainter>
#include <QScrollArea>

#include "Camera.h"
#include "Wireframe.h"

class RenderArea final : public QWidget
{
    Q_OBJECT

public:
    explicit RenderArea(QWidget *parent = nullptr);

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    void setZoom(double zoom);


signals:
    void mousePressed(const QPoint &pos);
    void mouseMoved(const QPoint &pos);

public slots:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void resizeScreen(const QSize &size);
    void wheelEvent(QWheelEvent *event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void resetRotation();
    void onMUpdated(int m);
    void onM1Updated(int m1);
    void onSetFormingLine(BSpline formingLine);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void drawWireframe(QPainter *painter);

private:
    double width_ = 1;
    QImage screen;
    QPen pen;
    Wireframe *wireframe;
    Camera *camera;
    std::vector<std::pair<Point3D, Point3D>> wireframeLines{};

    bool mousePressed_{};
    Point3D lastPointPos{-10,-10,-10};
};
