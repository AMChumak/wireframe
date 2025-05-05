#pragma once

#include <QMainWindow>
#include <QToolBar>
#include <QVBoxLayout>

#include "EditorWindow.h"
#include "RenderArea.h"
#include "../../model/config/ConfigKeeper.h"

#if defined MAKE_UI_LIB
#define UI_LIB_EXPORT Q_DECL_EXPORT
#else
#define UI_LIB_EXPORT Q_DECL_IMPORT
#endif

class UI_LIB_EXPORT MainWindow final : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);



private slots:
    void resizeEvent(QResizeEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void newFile();
    void open();
    void save();
    void openEditor();
    void resetAxis();
    void about();

    void onCanvasPressed(const QPoint &point);
    void onMouseMovedOverCanvas(const QPoint &point);
    void onEditorClosed();


private:
    void createActions();
    void createMenus();
    void createToolBar();

private:
    QWidget *window;
    QVBoxLayout *m_layout;
    QMenu *fileMenu{};
    QMenu *editMenu{};
    QMenu *aboutMenu{};
    QAction *newFileAct{};
    QAction *openAct{};
    QAction *saveAct{};
    QAction *editorAct{};
    QAction *resetAct{};
    QAction *aboutAct{};
    QToolBar *mainToolBar{};
    RenderArea *renderArea{};
    EditorWindow *editorWindow{};
    ConfigKeeper *configKeeper{};
};

inline void MainWindow::onEditorClosed()
{
    editorWindow = nullptr;
}
