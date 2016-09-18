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
  showFullScreen ();
}

QMemory::~QMemory ()
{}

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
      
      int walls = maze->cellAt (ix + 1, iy + 1).walls;
      QString dbg_msg = QString ("c(%1, %2) = %3").arg (ix).arg (iy).arg (walls);
      CellID ii;
      ii.coord.x = ix + 1;
      ii.coord.y = iy + 1;
      if (ii.id == maze->getStart ().id)  {
        dbg_msg += " (Start)";
      }
      if (ii.id == maze->getFinish ().id)  {
        dbg_msg += " (Finish)";
      }
      if (walls & (1 << D_BOTTOM))  {
        painter->drawLine (cells[ix][iy].bottomLeft (), cells[ix][iy].bottomRight ());
      }
      if (walls & (1 << D_TOP))  {
        painter->drawLine (cells[ix][iy].topLeft (), cells[ix][iy].topRight ());
      }
      if (walls & (1 << D_RIGHT))  {
        painter->drawLine (cells[ix][iy].bottomRight (), cells[ix][iy].topRight ());
      }
      if (walls & (1 << D_LEFT))  {
        painter->drawLine (cells[ix][iy].bottomLeft (), cells[ix][iy].topLeft ());
      }
      painter->drawText (QPointF (cells[ix][iy].center ()), dbg_msg);
    }
  }
}

void QMemory::keyPressEvent (QKeyEvent *event)
{
  qDebug () << event->key ();
  qApp->quit ();
}

void QMemory::paintEvent (QPaintEvent *event)
{
  QWidget::paintEvent (event);
  QPainter pnt (this);
  Maze mz (10, 10);
  mz.generate ();
  drawMaze (&mz, &pnt);
  pnt.end ();
}