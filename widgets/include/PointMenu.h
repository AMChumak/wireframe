#pragma once
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <qvalidator.h>
#include <QVBoxLayout>
#include <QWidget>


class PointMenu : public QWidget
{
    Q_OBJECT

public:
    explicit PointMenu(double x, double y);

signals:
    void valueUpdated(double x, double y);
    void onCLose();

public slots:
    void closeEvent(QCloseEvent* event) override;
    void onPointMove(double x, double y);

private slots:
    void onOkClicked();
    void onCancelClicked();

    private:
    QVBoxLayout* mLayout;
    QHBoxLayout* xLayout;
    QHBoxLayout* yLayout;
    QLabel* xLabel;
    QLabel* yLabel;
    QLineEdit* xEdit;
    QLineEdit* yEdit;
    QDoubleValidator *validator;
    QHBoxLayout* btnLayout;
    QPushButton* okButton;
    QPushButton* cancelButton;
};
