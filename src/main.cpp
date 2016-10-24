#include <QApplication>
#include <QTranslator>
#include <QLocale>

#include "QMazeWindow.hpp"

int main (int argc, char *argv[])
{
  QApplication app (argc, argv);
  QTranslator trans;
  QLocale loc;
  trans.load (QString (":/trans_%1").arg (loc.name ()));
  app.installTranslator (&trans);
  QMazeWindow *mwin = new QMazeWindow ();
  return app.exec ();
}