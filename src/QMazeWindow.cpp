#include "QMazeWindow.hpp"
#include "maze.hpp"

#include <QApplication>
#include <QPainter>
#include <QPaintEvent>
#include <QKeyEvent>

QMazeWindow::QMazeWindow ()
  : QMainWindow ()
{
  fullMaze = false;
  the_maze = new Maze (11, 7);
  myPosition = the_maze->getStart ().id;
  showFullScreen ();
}

QMazeWindow::~QMazeWindow ()
{
  delete the_maze;
}

void QMazeWindow::drawMaze (Maze *maze, QPainter *painter)
{
  QRect cells[maze->get_x ()][maze->get_y ()];
  const int wd = width () / maze->get_x ();
  const int ht = height () / maze->get_y ();
  
  painter->fillRect (rect (), Qt::black);
  painter->setPen (Qt::white);
  
  for (int ix = 0; ix < maze->get_x (); ++ix)  {
    for (int iy = 0; iy < maze->get_y (); ++iy)  {
      cells[ix][iy] = QRect (wd * ix, ht * iy, wd, ht);
      Cell& cll = maze->cellAt (ix + 1, iy + 1);
      
      if (cll.id.id == maze->getStart ().id)  {
        painter->drawText (QPointF (cells[ix][iy].center ()), tr ("Start"));
      }
      if (cll.id.id == maze->getFinish ().id)  {
        painter->drawText (QPointF (cells[ix][iy].center ()), tr ("Finish"));
      }
      
      if (cll.isWallSet (D_BOTTOM))  {
        painter->drawLine (cells[ix][iy].topLeft (), cells[ix][iy].topRight ());
      }
      if (cll.isWallSet (D_TOP))  {
        painter->drawLine (cells[ix][iy].bottomLeft (), cells[ix][iy].bottomRight ());
      }
      if (cll.isWallSet (D_RIGHT))  {
        painter->drawLine (cells[ix][iy].bottomRight (), cells[ix][iy].topRight ());
      }
      if (cll.isWallSet (D_LEFT))  {
        painter->drawLine (cells[ix][iy].bottomLeft (), cells[ix][iy].topLeft ());
      }
    }
  }
  
  QPainterPath path;
  QRectF bounding;
  CellID pos;
  pos.id = myPosition;
  bounding.setWidth (0.6 * wd);
  bounding.setHeight (0.6 * ht);
  bounding.moveCenter (cells[pos.coord.x - 1][pos.coord.y - 1].center ());
  path.addEllipse (bounding);
  painter->fillPath (path, Qt::green);
}

void QMazeWindow::drawCell (Maze *maze, QPainter *painter)
{
  QRect corners[3][3];
  const int wd = width () / 3;
  const int ht = height () / 3;
  
  painter->fillRect (rect (), Qt::black);
  
  QColor colors[8] = {
    Qt::red,
    Qt::magenta,
    Qt::blue,
    Qt::cyan,
    Qt::green,
    Qt::yellow,
    Qt::white,
    Qt::white
  };
  
  CellID pos;
  pos.id = myPosition;
  QColor col = colors[maze->cellAt (pos).walls >> 5];
  
  for (int ix = 0; ix < 3; ++ix)  {
    for (int iy = 0; iy < 3; ++iy)  {
      corners[ix][iy] = QRect (wd * ix, ht * iy, wd, ht);
    }
  }
  
  painter->fillRect (corners[0][0], col);
  painter->fillRect (corners[0][2], col);
  painter->fillRect (corners[2][0], col);
  painter->fillRect (corners[2][2], col);
  
  if (maze->cellAt (pos).isWallSet (D_RIGHT))  {
    painter->fillRect (corners[2][1], col);
  }
  
  if (maze->cellAt (pos).isWallSet (D_LEFT))  {
    painter->fillRect (corners[0][1], col);
  }
  
  if (maze->cellAt (pos).isWallSet (D_TOP))  {
    painter->fillRect (corners[1][2], col);
  }
  
  if (maze->cellAt (pos).isWallSet (D_BOTTOM))  {
    painter->fillRect (corners[1][0], col);
  }
  
  painter->setPen (col);
  if (myPosition == maze->getStart ().id)  {
    painter->drawText (QPointF (corners[1][1].center ()), tr ("Start"));
  }
  
  if (myPosition == maze->getFinish ().id)  {
    painter->drawText (QPointF (corners[1][1].center ()), tr ("Finish"));
  }
}

bool QMazeWindow::move (int direction)
{
  bool RET = false;
  Direction dir = (Direction) direction;
  CellID pos;
  pos.id = myPosition;
  
  if (the_maze->canGo (pos, dir))  {
    RET = true;
    switch (dir)  {
      case D_TOP:  {
        pos.coord.y++;
        myPosition = pos.id;
        break;
      }
      
      case D_RIGHT:  {
        pos.coord.x++;
        myPosition = pos.id;
        break;
      }
      
      case D_BOTTOM:  {
        pos.coord.y--;
        myPosition = pos.id;
        break;
      }
      
      case D_LEFT:  {
        pos.coord.x--;
        myPosition = pos.id;
        break;
      }
    }
  }
  repaint ();
  
  return RET;
}

void QMazeWindow::keyPressEvent (QKeyEvent *event)
{
  CellID pos;
  pos.id = myPosition;
  int kk = event->key ();
  switch (kk)  {
    /* Pressed [Esc]: */
    case 16777216:  {
      qApp->quit ();
      break;
    }
    
    /* Pressed [Up] - Pretend to be [Down]: */
    case 16777235:  {
      move ((int) D_BOTTOM);
      break;
    }
    
    /* Pressed [Down] - Pretend to be [Up]. */
    case 16777237:  {
      move ((int) D_TOP);
      break;
    }
    
    /* Pressed [Right]. */
    case 16777236:  {
      move ((int) D_RIGHT);
      break;
    }
    
    /* Pressed [Left]. */
    case 16777234:  {
      move ((int) D_LEFT);
      break;
    }
    
    /* Pressed [F]. */
    case 70:  {
      fullMaze = true;
      repaint ();
      break;
    }
    
    /* Pressed [C]. */
    case 67:  {
      fullMaze = false;
      repaint ();
      break;
    }
  }
}

void QMazeWindow::paintEvent (QPaintEvent *event)
{
  QWidget::paintEvent (event);
  QPainter pnt (this);
  if (fullMaze)  {
    drawMaze (the_maze, &pnt);
  } else  {
    drawCell (the_maze, &pnt);
  }
  pnt.end ();
}