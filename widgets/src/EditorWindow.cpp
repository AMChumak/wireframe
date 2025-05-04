#include "EditorWindow.h"

#include <qevent.h>
#include <QLabel>


EditorWindow::EditorWindow(QWidget* parent)
{
    mainLayout = new QVBoxLayout;
    canvasArea = new CanvasArea(parent);
    mainLayout->addWidget(canvasArea);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    toolsLayout = new QVBoxLayout;
    countersLayout = new QHBoxLayout();
    buttonsLayout = new QHBoxLayout();
    labelN = new QLabel("N");
    labelK = new QLabel("K");
    labelM = new QLabel("M");
    labelM1 = new QLabel("M1");

    nSpinbox = new QSpinBox();
    nSpinbox->installEventFilter(this);
    nSpinbox->setValue(1);                                                               // todo заменить на чтение из конфига
    nSpinbox->setRange(1, 200);
    nSpinbox->setToolTip("Count of parts in one B-Spline segment");
    connect(nSpinbox, SIGNAL(valueChanged(int)), canvasArea, SLOT(updateN(int)));

    kSpinbox = new QSpinBox();
    kSpinbox->installEventFilter(this);
    mSpinbox = new QSpinBox();
    mSpinbox->installEventFilter(this);
    m1Spinbox = new QSpinBox();
    m1Spinbox->installEventFilter(this);
    okButton = new QPushButton("OK");
    zoomInButton = new QPushButton("zoom in");
    zoomInButton->setIcon(QIcon(":/resources/zoom_in.png"));
    zoomOutButton = new QPushButton("zoom out");
    zoomOutButton->setIcon(QIcon(":/resources/zoom_out.png"));
    zoomResetButton = new QPushButton("zoom reset");
    zoomResetButton->setIcon(QIcon(":/resources/zoom_reset.png"));

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
    connect(addVertexButton, &QRadioButton::toggled, this, &EditorWindow::onRadioButtonClicked);
    connect(canvasArea, &CanvasArea::addVertexModeChanged, this, &EditorWindow::onAddVertexModeChanged);


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
    return QWidget::eventFilter(watched, event);
}

void EditorWindow::closeEditor()
{
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

void EditorWindow::resizeEvent(QResizeEvent* event)
{
    canvasArea->resize(width(), height() - 65);
    update();
    QWidget::resizeEvent(event);
}

void EditorWindow::zoomIn()
{
}

void EditorWindow::zoomOut()
{
}

void EditorWindow::zoomReset()
{
}

void EditorWindow::onRadioButtonClicked(bool checked)
{
    canvasArea->setAddVertexMode(checked);
}

void EditorWindow::onAddVertexModeChanged(bool mode)
{
    addVertexButton->setChecked(mode);
}
