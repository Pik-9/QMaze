#include <QtGui/QApplication>
#include "QMemory.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QMemory qmemory;
    qmemory.show();
    return app.exec();
}
