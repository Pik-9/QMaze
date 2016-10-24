/******************************************************************************
 *  QMaze - A simple maze game                                                *
 *  Copyright 2016 Daniel Steinhauer <d.steinhauer@mailbox.org>               *
 *                                                                            *
 *                                                                            *
 *  This file is part of QMaze                                                *
 *                                                                            *
 *  QMaze is free software: you can redistribute it and/or modify             *
 *  it under the terms of the GNU General Public License as published by      *
 *  the Free Software Foundation, either version 3 of the License, or         *
 *  (at your option) any later version.                                       *
 *                                                                            *
 *  QMaze is distributed in the hope that it will be useful,                  *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 *  GNU General Public License for more details.                              *
 *                                                                            *
 *  You should have received a copy of the GNU General Public License         *
 *  along with QMaze.  If not, see <http://www.gnu.org/licenses/>.            *
 ******************************************************************************/

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