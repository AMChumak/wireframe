#pragma once


#include <QWidget>

class CanvasArea : public QWidget
{
    Q_OBJECT

public:
    explicit CanvasArea(QWidget* parent = nullptr);

public slots:
    void resizeEvent(QResizeEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    QImage canvas;
};
