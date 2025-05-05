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


signals:
    void mousePressed(const QPoint &pos);
    void mouseMoved(const QPoint &pos);

public slots:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void initFile();
    void loadFile(const QString &fileName);
    void saveFile(const QString &fileName);
    void resizeScreen(const QSize &size);

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
    std::vector<std::pair<Point3D, Point3D>> wireframeLines;
};
