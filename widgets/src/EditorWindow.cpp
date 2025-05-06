#include "EditorWindow.h"

#include <iostream>
#include <qevent.h>
#include <QLabel>
#include <utility>


EditorWindow::EditorWindow(QWidget* parent)
{
    mainLayout = new QVBoxLayout;
    canvasArea = new CanvasArea(parent);
    canvasArea->installEventFilter(this);
    mainLayout->addWidget(canvasArea);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    toolsLayout = new QVBoxLayout;
    countersLayout = new QHBoxLayout();
    buttonsLayout = new QHBoxLayout();
    labelN = new QLabel("N: ");
    labelN->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    labelK = new QLabel("K: ");
    labelK->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    labelM = new QLabel("M: ");
    labelM->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    labelM1 = new QLabel("M1: ");
    labelM1->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    nSpinbox = new QSpinBox();
    nSpinbox->installEventFilter(this);
    nSpinbox->setValue(1);                                                               // todo заменить на чтение из конфига
    nSpinbox->setRange(1, 200);
    nSpinbox->setToolTip("Count of parts in one B-Spline segment");
    connect(nSpinbox, SIGNAL(valueChanged(int)), canvasArea, SLOT(updateN(int)));

    kSpinbox = new QSpinBox();
    kSpinbox->installEventFilter(this);
    kSpinbox->setValue(5);                                                               // todo заменить на чтение из конфига
    kSpinbox->setRange(4, 200);
    kSpinbox->setToolTip("Count of B-Spline segments");
    connect(kSpinbox, SIGNAL(valueChanged(int)), canvasArea, SLOT(updateK(int)));

    mSpinbox = new QSpinBox();
    mSpinbox->installEventFilter(this);
    mSpinbox->setValue(2);                                                               // todo заменить на чтение из конфига
    mSpinbox->setRange(2, 36);
    mSpinbox->setToolTip("Count of forming lines in wireframe");
    connect(mSpinbox, SIGNAL(valueChanged(int)), this, SLOT(onMChanged(int)));

    m1Spinbox = new QSpinBox();
    m1Spinbox->installEventFilter(this);
    m1Spinbox->setValue(1);                                                               // todo заменить на чтение из конфига
    m1Spinbox->setRange(1, 36);
    m1Spinbox->setToolTip("Count of parts in arc");
    connect(m1Spinbox, SIGNAL(valueChanged(int)), this, SLOT(onM1Changed(int)));

    okButton = new QPushButton("OK");

    zoomInButton = new QPushButton("zoom in");
    zoomInButton->setIcon(QIcon(":/resources/zoom_in.png"));
    connect(zoomInButton, SIGNAL(clicked()), this, SLOT(zoomIn()));
    zoomOutButton = new QPushButton("zoom out");
    zoomOutButton->setIcon(QIcon(":/resources/zoom_out.png"));
    connect(zoomOutButton, SIGNAL(clicked()), this, SLOT(zoomOut()));
    zoomResetButton = new QPushButton("smart zoom");
    zoomResetButton->setIcon(QIcon(":/resources/zoom_reset.png"));
    connect(zoomResetButton, SIGNAL(clicked()), this, SLOT(zoomReset()));

    addVertexButton = new QRadioButton("add vertex");
    addVertexButton->setIcon(QIcon(":/resources/add.svg"));

    countersLayout->addWidget(labelN);
    countersLayout->addWidget(nSpinbox);
    countersLayout->addWidget(labelK);
    countersLayout->addWidget(kSpinbox);
    countersLayout->addWidget(labelM);
    countersLayout->addWidget(mSpinbox);
    countersLayout->addWidget(labelM1);
    countersLayout->addWidget(m1Spinbox);

    buttonsLayout->addWidget(okButton);
    buttonsLayout->addWidget(addVertexButton);
    buttonsLayout->addWidget(zoomInButton);
    buttonsLayout->addWidget(zoomResetButton);
    buttonsLayout->addWidget(zoomOutButton);

    toolsLayout->addLayout(countersLayout);
    toolsLayout->addLayout(buttonsLayout);

    mainLayout->addLayout(toolsLayout);

    setLayout(mainLayout);


    //connections
    connect(okButton, SIGNAL(clicked()), this, SLOT(okButtonClicked()));
    connect(addVertexButton, &QRadioButton::toggled, this, &EditorWindow::onRadioButtonClicked);
    connect(canvasArea, &CanvasArea::addVertexModeChanged, this, &EditorWindow::onAddVertexModeChanged);
    connect(canvasArea, &CanvasArea::openedSettingsForVertex, this, &EditorWindow::openedSettings);
    connect(canvasArea, &CanvasArea::updatedCntKeyPoints, this, &EditorWindow::onCntKeyPointsChanged);
    connect(canvasArea, &CanvasArea::updatedSpline, this, &EditorWindow::onSplineChanged);
    connect(canvasArea, &CanvasArea::updatedCamera, this, &EditorWindow::onCameraChanged);
    connect(canvasArea, &CanvasArea::updatedZoom, this, &EditorWindow::onZoomChanged);

    //window settings
    resize(800, 600);
    setWindowTitle("Wireframe Editor");
}

bool EditorWindow::eventFilter(QObject* watched, QEvent* event)
{
    if ((watched == nSpinbox || watched == kSpinbox || watched == mSpinbox || watched == m1Spinbox)
        && event->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Delete)
        {
            canvasArea->deleteChosenKeyPoint();
            return true;
        }
    }
    else if (event->type() == QEvent::MouseButtonPress && watched == canvasArea)
    {
        if (pointMenu != nullptr)
        {
            pointMenu->closeEvent(new QCloseEvent{});
            pointMenu = nullptr;
        }
    }
    return QWidget::eventFilter(watched, event);
}


void EditorWindow::closeEvent(QCloseEvent* event)
{
    emit editorClosed();
    QWidget::closeEvent(event);
}

void EditorWindow::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Control)
    {
        canvasArea->setAddVertexMode(true);
    }
    if (event->key() == Qt::Key_Delete)
    {
        canvasArea->deleteChosenKeyPoint();
    }
    QWidget::keyPressEvent(event);
}

void EditorWindow::keyReleaseEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Control)
    {
        canvasArea->setAddVertexMode(false);
    }
    QWidget::keyReleaseEvent(event);
}

void EditorWindow::openedSettings(double x, double y, int j, int i)
{
    if (pointMenu != nullptr)
    {
        pointMenu->close();
    }
    pointMenu = new PointMenu(x,y);
    connect(pointMenu, &PointMenu::onCLose, this, &EditorWindow::onPointMenuClosed);
    connect(pointMenu, &PointMenu::valueUpdated, this, &EditorWindow::onPointMenuUpdated);
    pointMenu->show();
}

void EditorWindow::onSetSpline(BSpline spline)
{
    canvasArea->setSpline(spline);
    kSpinbox->setValue(spline.keyPoints().size());
    nSpinbox->setValue(spline.getCntParts());
}

void EditorWindow::onSetCamera(Point3D camera, double zoom)
{
    canvasArea->setCamera(camera, zoom);
}

void EditorWindow::onSetM1(int m1)
{
    m1Spinbox->setValue(m1);
    emit M1Changed(m1);
}

void EditorWindow::onSetM(int m)
{
    mSpinbox->setValue(m);
    emit MChanged(m);
}

void EditorWindow::resizeEvent(QResizeEvent* event)
{
    canvasArea->resize(width(), height() - 65);
    update();
    QWidget::resizeEvent(event);
}

void EditorWindow::zoomIn()
{
    canvasArea->zoomIn();
}

void EditorWindow::zoomOut()
{
    canvasArea->zoomOut();
}

void EditorWindow::zoomReset()
{
    canvasArea->zoomReset();
}

void EditorWindow::okButtonClicked()
{
    close();
}

void EditorWindow::onRadioButtonClicked(bool checked)
{
    canvasArea->setAddVertexMode(checked);
}

void EditorWindow::onAddVertexModeChanged(bool mode)
{
    addVertexButton->setChecked(mode);
}

void EditorWindow::onPointMenuClosed()
{
    pointMenu->close();
    pointMenu = nullptr;
}

void EditorWindow::onPointMenuUpdated(double x, double y)
{
    canvasArea->updateChosenKeyPoint(x,y);
}

void EditorWindow::onCntKeyPointsChanged(int count)
{
    kSpinbox->setValue(count);
}

void EditorWindow::onMChanged(int m)
{
    emit MChanged(m);
}

void EditorWindow::onM1Changed(int m1)
{
    emit M1Changed(m1);
}

void EditorWindow::onSplineChanged(BSpline spline)
{
    emit splineChanged(spline);
}

void EditorWindow::onZoomChanged(double zoom)
{

    emit zoomChanged(zoom);
}

void EditorWindow::onCameraChanged(Point3D camera)
{
    emit cameraChanged(camera);
}
