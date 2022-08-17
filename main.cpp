#include <QApplication>
#include <QCoreApplication>

#include "visuCFS.h"
#include "glwidget.h"

int main(int argc, char* argv[])
{

    QApplication app(argc, argv);

    MaForme visuCFS;
    visuCFS.showMaximized();

    return app.exec();
}
