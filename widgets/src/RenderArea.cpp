#include "RenderArea.h"

#include <iostream>
#include <QSize>
#include <QTransform>
#include <QFile>
#include <qevent.h>

RenderArea::RenderArea(QWidget* parent)
    : QWidget(parent)
{
    setMouseTracking(true);
    width_ = 4;
    double height = 6.0 / 8.0 * width_;
    camera = new Camera(width_, height, 9, 12);
    wireframe = new Wireframe();
}

QSize RenderArea::minimumSizeHint() const
{
    return QSize(0, 0);
}

QSize RenderArea::sizeHint() const
{
    return QSize(402, 402);
}

void RenderArea::setZoom(double zoom)
{
    camera->setZoom(zoom);
    wireframeLines = camera->render(wireframe->getTransform(), wireframe->getPolylines());
    update();
}

void RenderArea::setRotation(const Eigen::Matrix4d& rotation)
{
    wireframe->resetRotation();
    wireframe->addRotation(rotation);
    wireframeLines = camera->render(wireframe->getTransform(), wireframe->getPolylines());
    update();
}

void RenderArea::resizeScreen(const QSize& size)
{
    screen = QImage(size.width(), size.height(), QImage::Format_ARGB32);
    screen.fill(Qt::white);
    resize(screen.width(), screen.height());
    camera->updateSizes(width_, width_ * (double)screen.height() / screen.width());
    wireframeLines = camera->render(wireframe->getTransform(), wireframe->getPolylines());
    update();
}

void RenderArea::wheelEvent(QWheelEvent* event)
{
    if (event->angleDelta().y() > 0)
    {
        camera->zoom(1.05);
    }
    else
    {
        camera->zoom(0.9090909);
    }
    emit zoomChanged(camera->getZoom());
    wireframeLines = camera->render(wireframe->getTransform(), wireframe->getPolylines());
    update();
    QWidget::wheelEvent(event);
}


void RenderArea::mousePressEvent(QMouseEvent* event)
{
    mousePressed_ = true; //todo разблокировать
    double x = event->pos().x() - screen.width() / 2;
    double y = event->pos().y() - screen.height() / 2;
    lastPointPos = camera->findPointOnSphere(Point3D{
        x * 2.0 / size().width(), y * 2.0 / size().height(), 0
    });

    QWidget::mousePressEvent(event);
}

void RenderArea::mouseReleaseEvent(QMouseEvent* event)
{
    mousePressed_ = false;
    QWidget::mouseReleaseEvent(event);
}

void RenderArea::mouseMoveEvent(QMouseEvent* event)
{
    if (mousePressed_ && lastPointPos.x != -10)
    {
        double x = event->pos().x() - screen.width() / 2;
        double y = event->pos().y() - screen.height() / 2;
        auto newPointPos = camera->findPointOnSphere(Point3D{
            x * 2.0 / size().width(), y * 2.0 / size().height(), 0
        });

        wireframe->addRotation(lastPointPos, newPointPos);
        lastPointPos = newPointPos;
        wireframeLines = camera->render(wireframe->getTransform(), wireframe->getPolylines());
        emit rotationChanged(wireframe->getRotationMatrix());
        update();
    }
    QWidget::mouseMoveEvent(event);
}


void RenderArea::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.drawImage(QRect(0, 0, screen.width(), screen.height()), screen);
    drawWireframe(&painter);
    drawAxes(&painter);
    painter.end();
}

void RenderArea::drawAxes(QPainter* painter)
{
    double q = camera->getZoom();
    Polyline3D axisX{std::vector<Point3D>{Point3D{0,0,0},Point3D{0.5/q,0,0}}};
    Polyline3D axisY{std::vector<Point3D>{Point3D{0,0,0},Point3D{0,0.5/q,0}}};
    Polyline3D axisZ{std::vector<Point3D>{Point3D{0,0,0},Point3D{0,0,0.5/q}}};

    std::vector<Polyline3D> axes = {axisX, axisY, axisZ};

    auto lines = camera->render(wireframe->getRotationMatrix(), axes, false);

    int w = size().width();
    int h = size().height();

    std::vector<QColor> colors = {Qt::red, Qt::green, Qt::blue};
    for (int i = 0; i < lines.size(); i++)
    {
        auto pr = lines[i];
        int x1 = w * pr.first.x + h*0.05;
        int y1 = h * pr.first.y + h*0.05;
        int x2 = w * pr.second.x + h*0.05;
        int y2 = h * pr.second.y + h*0.05;

        painter->setPen(QPen(colors[i], 1));
        painter->drawLine(x1, y1, x2, y2);
    }
}

void RenderArea::drawWireframe(QPainter* painter)
{
    int w = size().width();
    int h = size().height();

    for (auto& pr : wireframeLines)
    {
        int x1 = w * pr.first.x + w / 2;
        int y1 = h * pr.first.y + h / 2;
        int x2 = w * pr.second.x + w / 2;
        int y2 = h * pr.second.y + h / 2;
        QLinearGradient linearGradient(QPointF(x1, y1), QPointF(x2, y2));
        linearGradient.setColorAt(0, QColor(255 * (1 - pr.first.z), 0, 255 * pr.first.z));
        linearGradient.setColorAt(1, QColor(255 * (1 - pr.second.z), 0, 255 * pr.second.z));
        painter->setPen(QPen(QBrush(linearGradient), 1));
        painter->drawLine(x1, y1, x2, y2);
    }
}

void RenderArea::resetRotation()
{
    wireframe->resetRotation();
    wireframeLines = camera->render(wireframe->getTransform(), wireframe->getPolylines());
    update();
}

void RenderArea::onMUpdated(int m)
{
    wireframe->setCntFormingLines(m);
    wireframeLines = camera->render(wireframe->getTransform(), wireframe->getPolylines());
    update();
}

void RenderArea::onM1Updated(int m1)
{
    wireframe->setCntPartsInSegment(m1);
    wireframeLines = camera->render(wireframe->getTransform(), wireframe->getPolylines());
    update();
}

void RenderArea::onSetFormingLine(BSpline formingLine)
{
    wireframe->setForming(formingLine);
    wireframeLines = camera->render(wireframe->getTransform(), wireframe->getPolylines());
    update();
}
