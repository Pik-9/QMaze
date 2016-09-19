#include <QApplication>

#include "QMazeWindow.hpp"

int main (int argc, char *argv[])
{
  QApplication app (argc, argv);
  QMazeWindow *mwin = new QMazeWindow ();
  return app.exec ();
}