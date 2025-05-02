#include "EditorWindow.h"

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
    kSpinbox = new QSpinBox();
    mSpinbox = new QSpinBox();
    m1Spinbox = new QSpinBox();
    okButton = new QPushButton("OK");
    zoomInButton = new QPushButton("zoom in");
    zoomInButton->setIcon(QIcon(":/resources/zoom_in.png"));
    zoomOutButton = new QPushButton("zoom out");
    zoomOutButton->setIcon(QIcon(":/resources/zoom_out.png"));
    zoomResetButton = new QPushButton("zoom reset");
    zoomResetButton->setIcon(QIcon(":/resources/zoom_reset.png"));

    countersLayout->addWidget(labelN);
    countersLayout->addWidget(nSpinbox);
    countersLayout->addWidget(labelK);
    countersLayout->addWidget(kSpinbox);
    countersLayout->addWidget(labelM);
    countersLayout->addWidget(mSpinbox);
    countersLayout->addWidget(labelM1);
    countersLayout->addWidget(m1Spinbox);

    buttonsLayout->addWidget(okButton);
    buttonsLayout->addWidget(zoomInButton);
    buttonsLayout->addWidget(zoomResetButton);
    buttonsLayout->addWidget(zoomOutButton);

    toolsLayout->addLayout(countersLayout);
    toolsLayout->addLayout(buttonsLayout);

    mainLayout->addLayout(toolsLayout);

    setLayout(mainLayout);

    resize(800, 600);
    setWindowTitle("Wireframe Editor");
}

void EditorWindow::closeEditor()
{
}

void EditorWindow::resizeEvent(QResizeEvent* event)
{
    canvasArea->resize(width(), height()-65);
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



