#ifndef QMemory_H
#define QMemory_H

#include <QMainWindow>

class QKeyEvent;
class Maze;
class QPainter;
class QPaintEvent;

class QMemory : public QMainWindow
{
  Q_OBJECT
public:
  QMemory ();
  virtual ~QMemory ();
  
  void drawMaze (Maze*, QPainter*);
  
protected:
  void keyPressEvent (QKeyEvent*);
  void paintEvent (QPaintEvent*);
};

#endif // QMemory_H