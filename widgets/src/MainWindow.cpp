#include "MainWindow.h"
#include <QAction>
#include <QColorDialog>
#include <QFileDialog>
#include <QMenuBar>
#include <QResizeEvent>
#include <QStyle>
#include "AboutWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_layout = new QVBoxLayout;
    //m_layout->setSpacing(0);
    m_layout->setContentsMargins(0, 0, 0, 0);
    renderArea = new RenderArea(this);
    renderArea->resizeScreen(this->size());
    m_layout->addWidget(renderArea);
    window = new QWidget();
    window->setLayout(m_layout);
    setCentralWidget(window);


    createActions();
    createMenus();
    createToolBar();
    setWindowTitle("AMC Wireframe");
    setMinimumSize(640, 480);
    newFile();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    renderArea->resizeScreen(event->size());
    update();
    QMainWindow::resizeEvent(event);
}
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    QMainWindow::mouseMoveEvent(event);
}

void MainWindow::newFile()
{
    configKeeper = new ConfigKeeper();
    renderArea->onSetFormingLine(configKeeper->spline);
    renderArea->resetRotation();
    renderArea->setZoom(configKeeper->zForward);
    if (editorWindow != nullptr)
    {
        editorWindow->onSetSpline(configKeeper->spline);
        editorWindow->onSetCamera(configKeeper->cameraCenter, configKeeper->cameraZoom);
        editorWindow->onSetM(configKeeper->M);
        editorWindow->onSetM1(configKeeper->M1);
    }
}
void MainWindow::open()
{

}
void MainWindow::save()
{

}

void MainWindow::openEditor()
{
    if (editorWindow == nullptr)
    {
        editorWindow = new EditorWindow(this);
        editorWindow->show();
        connect (editorWindow, SIGNAL(editorClosed()), this, SLOT(onEditorClosed()));
        connect (editorWindow, SIGNAL(MChanged(int)), renderArea, SLOT(onMUpdated(int)));
        connect (editorWindow, SIGNAL(M1Changed(int)), renderArea, SLOT(onM1Updated(int)));
        connect (editorWindow, SIGNAL(splineChanged(BSpline)), renderArea, SLOT(onSetFormingLine(BSpline)));
    }
}

void MainWindow::resetAxis()
{
    renderArea->resetRotation();
}

void MainWindow::about()
{
    auto *aboutWindow = new AboutWindow();
    QRect rect = geometry();
    aboutWindow->move(
        rect.width() / 2 - aboutWindow->width() / 2,
        rect.height() / 2 - aboutWindow->height() / 2);
    aboutWindow->show();
}

void MainWindow::onCanvasPressed(const QPoint &point)
{

}
void MainWindow::onMouseMovedOverCanvas(const QPoint &point)
{

}

void MainWindow::createActions()
{
    newFileAct = new QAction("New", this);
    newFileAct->setStatusTip("Create a new file");
    newFileAct->setIcon(QIcon(":/resources/new_file.png"));
    connect(newFileAct, SIGNAL(triggered()), this, SLOT(newFile()));

    openAct = new QAction("Open", this);
    openAct->setStatusTip("Open a file");
    openAct->setIcon(QIcon(":/resources/open.png"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction("Save", this);
    saveAct->setStatusTip("Save the file");
    saveAct->setIcon(QIcon(":/resources/save.png"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    editorAct = new QAction("Editor", this);
    editorAct->setStatusTip("Edit wireframe configuration");
    editorAct->setIcon(QIcon(":/resources/plan.png"));
    connect(editorAct, SIGNAL(triggered()), this, SLOT(openEditor()));

    resetAct = new QAction("Reset", this);
    resetAct->setStatusTip("Reset axis rotation");
    resetAct->setIcon(QIcon(":/resources/reset.png"));
    connect(resetAct, SIGNAL(triggered()), this, SLOT(resetAxis()));

    aboutAct = new QAction("About", this);
    aboutAct->setStatusTip("About the application");
    aboutAct->setIcon(QIcon(":/resources/about.png"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}
void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu("File");
    fileMenu->setToolTipsVisible(true);
    fileMenu->addAction(newFileAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);

    editMenu = menuBar()->addMenu("Edit");
    editMenu->setToolTipsVisible(true);
    editMenu->addAction(resetAct);
    editMenu->addAction(editorAct);

    aboutMenu = menuBar()->addMenu("About");
    aboutMenu->setToolTipsVisible(true);
    aboutMenu->addAction(aboutAct);
}
void MainWindow::createToolBar()
{
    mainToolBar = addToolBar("Paint Toolbar");
    mainToolBar->addAction(newFileAct);
    mainToolBar->addAction(openAct);
    mainToolBar->addAction(saveAct);
    mainToolBar->addSeparator();
    mainToolBar->addAction(editorAct);
    mainToolBar->addSeparator();
    mainToolBar->addAction(resetAct);
    mainToolBar->addSeparator();
    mainToolBar->addAction(aboutAct);
}
