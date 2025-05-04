#include "PointMenu.h"
#include <QLineEdit>
#include <qvalidator.h>

PointMenu::PointMenu(double x, double y)
{
    xLabel = new QLabel("X: ");
    yLabel = new QLabel("Y: ");
    xLabel->setAlignment(Qt::AlignRight|Qt::AlignCenter);
    yLabel->setAlignment(Qt::AlignRight|Qt::AlignCenter);


    validator = new QDoubleValidator(this);
    validator->setNotation(QDoubleValidator::StandardNotation);
    validator->setDecimals(6);
    validator->setRange(-1000.0, 1000.0);

    xEdit = new QLineEdit();
    xEdit->setValidator(validator);
    xEdit->setText(QString::number(x));
    yEdit = new QLineEdit();
    yEdit->setValidator(validator);
    yEdit->setText(QString::number(-y));

    okButton = new QPushButton("OK");
    cancelButton = new QPushButton("Cancel");


    mLayout = new QVBoxLayout();
    mLayout->setContentsMargins(0, 0, 0, 0);

    xLayout = new QHBoxLayout();
    xLayout->addWidget(xLabel);
    xLayout->addWidget(xEdit);

    yLayout = new QHBoxLayout();
    yLayout->addWidget(yLabel);
    yLayout->addWidget(yEdit);

    btnLayout = new QHBoxLayout();
    btnLayout->addWidget(okButton);
    connect(okButton, SIGNAL(clicked()),this, SLOT(onOkClicked()));
    btnLayout->addWidget(cancelButton);
    connect(okButton, SIGNAL(clicked()),this, SLOT(onCancelClicked()));

    mLayout->addLayout(xLayout);
    mLayout->addLayout(yLayout);
    mLayout->addLayout(btnLayout);

    this->setLayout(mLayout);
    setWindowTitle("Point parameters");
    setFixedSize(QSize(240, 120));
}

void PointMenu::closeEvent(QCloseEvent* event)
{
    emit onCLose();
    QWidget::closeEvent(event);
}

void PointMenu::onPointMove(double x, double y)
{
    xEdit->setText(QString::number(x));
    yEdit->setText(QString::number(y));
    update();
}

void PointMenu::onOkClicked()
{
    emit valueUpdated(xEdit->text().toDouble(),-yEdit->text().toDouble());
    close();
}

void PointMenu::onCancelClicked()
{
    close();
}
