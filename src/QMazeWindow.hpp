#ifndef QMemory_H
#define QMemory_H

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
  uint32_t myPosition, myLevel;
  bool fullMaze;
  
  const static uint32_t level[6];

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