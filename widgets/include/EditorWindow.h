#pragma once

#include <QColorDialog>
#include <QLabel>
#include <QToolBar>
#include <QWidget>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QPushButton>
#include <QRadioButton>

#include "Camera.h"
#include "CanvasArea.h"
#include "PointMenu.h"

#if defined MAKE_UI_LIB
#define UI_LIB_EXPORT Q_DECL_EXPORT
#else
#define UI_LIB_EXPORT Q_DECL_IMPORT
#endif

class UI_LIB_EXPORT EditorWindow final : public QWidget
{
    Q_OBJECT

public:
    explicit EditorWindow(QWidget* parent = nullptr);

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

signals:
    void editorClosed();
    void MChanged(int m);
    void M1Changed(int m1);
    void splineChanged(BSpline spline);
    void zoomChanged(double zoom);
    void cameraChanged(Point3D camera);

public slots:
    void closeEvent(QCloseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void openedSettings(double x, double y, int j, int i);
    void onSetSpline(BSpline spline);
    void onSetCamera(Point3D camera, double zoom);
    void onSetM1(int m1);
    void onSetM(int m);


private slots:
    void resizeEvent(QResizeEvent* event) override;
    void zoomIn();
    void zoomOut();
    void zoomReset();
    void okButtonClicked();
    void onRadioButtonClicked(bool checked);
    void onAddVertexModeChanged(bool mode);
    void onPointMenuClosed();
    void onPointMenuUpdated(double x, double y);
    void onCntKeyPointsChanged(int count);
    void onMChanged(int m);
    void onM1Changed(int m1);
    void onSplineChanged(BSpline spline);
    void onZoomChanged(double zoom);
    void onCameraChanged(Point3D camera);

private:
    QWidget* window;
    QVBoxLayout* mainLayout;
    CanvasArea* canvasArea;
    QVBoxLayout* toolsLayout;
    QHBoxLayout* countersLayout;
    QHBoxLayout* buttonsLayout;
    QLabel* labelN;
    QLabel* labelK;
    QLabel* labelM;
    QLabel* labelM1;
    QSpinBox* nSpinbox;
    QSpinBox* kSpinbox;
    QSpinBox* mSpinbox;
    QSpinBox* m1Spinbox;
    QPushButton* zoomInButton;
    QPushButton* zoomOutButton;
    QPushButton* zoomResetButton;
    QPushButton* okButton;
    QRadioButton* addVertexButton;
    PointMenu* pointMenu{};
};
