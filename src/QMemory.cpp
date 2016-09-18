#include "QMemory.hpp"
#include "maze.hpp"

#include <QApplication>
#include <QPainter>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QDebug>

QMemory::QMemory ()
  : QMainWindow ()
{
  maze = new Maze (11, 7);
  myPosition = maze->getStart ().id;
  showFullScreen ();
}

QMemory::~QMemory ()
{
  delete maze;
}

void QMemory::drawMaze (Maze *maze, QPainter *painter)
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

bool QMemory::move (int direction)
{
  bool RET = false;
  Direction dir = (Direction) direction;
  CellID pos;
  pos.id = myPosition;
  
  Cell& cll = maze->cellAt (pos);
  qDebug () << "Move: (" << pos.coord.x << "|" << cll.id.coord.x << ", " << pos.coord.y << "|" << cll.id.coord.y << ") -> " << direction << " ** " << cll.walls;
  
  if (maze->canGo (pos, dir))  {
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

void QMemory::keyPressEvent (QKeyEvent *event)
{
  qDebug () << event->key ();
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
      qDebug () << "Pressed [Up]";
      move ((int) D_BOTTOM);
      break;
    }
    
    /* Pressed [Down] - Pretend to be [Up]. */
    case 16777237:  {
      qDebug () << "Pressed [Down]";
      move ((int) D_TOP);
      break;
    }
    
    /* Pressed [Right]. */
    case 16777236:  {
      qDebug () << "Pressed [Right]";
      move ((int) D_RIGHT);
      break;
    }
    
    /* Pressed [Left]. */
    case 16777234:  {
      qDebug () << "Pressed [Left]";
      move ((int) D_LEFT);
      break;
    }
  }
}

void QMemory::paintEvent (QPaintEvent *event)
{
  QWidget::paintEvent (event);
  QPainter pnt (this);
  drawMaze (maze, &pnt);
  pnt.end ();
}