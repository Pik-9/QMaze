#include <QApplication>

#include "QMemory.hpp"


int main (int argc, char *argv[])
{
  QApplication app (argc, argv);
  QMemory *qmemory = new QMemory ();
  return app.exec ();
}