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
  Maze *maze;
  uint32_t myPosition;

public:
  QMemory ();
  virtual ~QMemory ();
  
  void drawMaze (Maze*, QPainter*);
  bool move (int);
  
protected:
  void keyPressEvent (QKeyEvent*);
  void paintEvent (QPaintEvent*);
};

#endif // QMemory_H