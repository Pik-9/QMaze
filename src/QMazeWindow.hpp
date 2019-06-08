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

#ifndef QMemory_H
#define QMemory_H

#define COUNT_LEVEL 8

#include <QMainWindow>

#include <stdint.h>
#include <memory>

class QKeyEvent;
class Maze;
class QPainter;
class QPaintEvent;

class QMazeWindow : public QMainWindow
{
  Q_OBJECT
private:
  std::unique_ptr<Maze> the_maze;
  uint32_t myPosition, myLevel, complMazes;
  uint8_t cameFrom;
  bool fullMaze, haveKey;
  
  const static uint32_t level[COUNT_LEVEL];

public:
  QMazeWindow ();
  virtual ~QMazeWindow ();
  
  void drawFinish (QRect, QPainter*);
  void drawMaze (QPainter*);
  void drawCell (QPainter*);
  bool move (int);
  
protected:
  void keyPressEvent (QKeyEvent*);
  void paintEvent (QPaintEvent*);
};

#endif