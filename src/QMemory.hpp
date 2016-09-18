#ifndef QMemory_H
#define QMemory_H

#include <QMainWindow>

#include <stdint.h>

class QKeyEvent;
class Maze;
class QPainter;
class QPaintEvent;

class QMemory : public QMainWindow
{
  Q_OBJECT
private:
  Maze *the_maze;
  uint32_t myPosition;
  bool fullMaze;

public:
  QMemory ();
  virtual ~QMemory ();
  
  void drawMaze (Maze*, QPainter*);
  void drawCell (Maze*, QPainter*);
  bool move (int);
  
protected:
  void keyPressEvent (QKeyEvent*);
  void paintEvent (QPaintEvent*);
};

#endif // QMemory_H