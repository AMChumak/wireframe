#include "RenderArea.h"
#include <QSize>
#include <QTransform>
#include <QFile>
#include <qevent.h>

RenderArea::RenderArea(QWidget *parent)
    : QWidget(parent)
{
    setMouseTracking(true);
    initFile();
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
    screen.fill(Qt::lightGray);
    resize(screen.width(), screen.height());
    update();
}
void RenderArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawImage(QRect(0, 0, screen.width(), screen.height()), screen);
    painter.end();
}