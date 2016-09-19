#ifndef QMemory_H
#define QMemory_H

#define COUNT_LEVEL 7

#include <QMainWindow>

#include <stdint.h>

class QKeyEvent;
class Maze;
class QPainter;
class QPaintEvent;

class QMazeWindow : public QMainWindow
{
  Q_OBJECT
private:
  Maze *the_maze;
  uint32_t myPosition, myLevel, complMazes;
  uint8_t cameFrom;
  bool fullMaze;
  
  const static uint32_t level[COUNT_LEVEL];

public:
  QMazeWindow ();
  virtual ~QMazeWindow ();
  
  void drawMaze (Maze*, QPainter*);
  void drawCell (Maze*, QPainter*);
  bool move (int);
  
protected:
  void keyPressEvent (QKeyEvent*);
  void paintEvent (QPaintEvent*);
};

#endif