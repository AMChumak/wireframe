#include <QApplication>
#include "MainWindow.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window{};
    window.show();
    Q_INIT_RESOURCE(resources);
    return app.exec();
}
