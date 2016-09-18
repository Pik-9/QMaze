#ifndef QMemory_H
#define QMemory_H

#include <QMainWindow>

class QKeyEvent;

class QMemory : public QMainWindow
{
  Q_OBJECT
public:
  QMemory ();
  virtual ~QMemory ();
  
protected:
  void keyPressEvent (QKeyEvent*);
};

#endif // QMemory_H