#include "RenderArea.h"

#include <iostream>
#include <QSize>
#include <QTransform>
#include <QFile>
#include <qevent.h>

RenderArea::RenderArea(QWidget *parent)
    : QWidget(parent)
{
    setMouseTracking(true);
    initFile();
    width_ = 4;
    double height = 9.0/16.0 * width_;
    camera = new Camera(width_, height ,9, 12);

    std::vector<Point3D> points = {{0, 0, 0}, {1, 2, 0}, {4, 3, 0}, {6, -1, 0}, {10, 2, 0}};
    BSpline example{points};
    example.setCntParts(10);
    wireframe = new Wireframe(example);
    wireframeLines = camera->render(wireframe->getTransform(),wireframe->getPolylines());
}

QSize RenderArea::minimumSizeHint() const
{
    return QSize(0, 0);
}
QSize RenderArea::sizeHint() const
{
    return QSize(402, 402);
}

void RenderArea::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
}
void RenderArea::mouseMoveEvent(QMouseEvent *event)
{
    emit mouseMoved(event->position().toPoint());
    QWidget::mouseMoveEvent(event);
}
void RenderArea::initFile()
{
    update();
}
void RenderArea::loadFile(const QString &fileName)
{
    update();
}
void RenderArea::saveFile(const QString &fileName)
{
    //todo сделать менеджер файлов, подготовить файл, запустить виджет сохранения для файла
}

void RenderArea::resizeScreen(const QSize &size)
{
    screen = QImage(size.width(), size.height(), QImage::Format_ARGB32);
    screen.fill(Qt::white);
    resize(screen.width(), screen.height());
    camera->updateSizes(width_, width_ * (double)screen.height()/screen.width());
    wireframeLines = camera->render(wireframe->getTransform(),wireframe->getPolylines());
    update();
}

void RenderArea::wheelEvent(QWheelEvent* event)
{
    if (event->angleDelta().y() > 0)
    {
        camera->zoom(1.1);
    } else
    {
        camera->zoom(0.9090909);
    }
    wireframeLines = camera->render(wireframe->getTransform(),wireframe->getPolylines());
    update();
    QWidget::wheelEvent(event);
}

void RenderArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawImage(QRect(0, 0, screen.width(), screen.height()), screen);
    drawWireframe(&painter);
    painter.end();
}

void RenderArea::drawWireframe(QPainter* painter)
{
    int w = size().width();
    int h = size().height();

    for (auto &pr : wireframeLines)
    {
        int x1 = w * pr.first.x + w/2;
        int y1 = h * pr.first.y + h/2;
        int x2 = w * pr.second.x + w/2;
        int y2 = h * pr.second.y + h/2;
        QLinearGradient linearGradient(QPointF(x1, y1), QPointF(x2, y2));
        linearGradient.setColorAt(0, QColor(255 * (1 - pr.first.z), 0, 255 * pr.first.z));
        linearGradient.setColorAt(1, QColor(255 * (1 - pr.second.z), 0, 255 * pr.second.z));
        painter->setPen(QPen(QBrush(linearGradient),1));
        painter->drawLine(x1, y1, x2, y2);
    }
}
