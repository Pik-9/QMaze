#include "QMazeWindow.hpp"
#include "maze.hpp"

#include <QApplication>
#include <QPainter>
#include <QPaintEvent>
#include <QKeyEvent>

const uint32_t QMazeWindow::level[COUNT_LEVEL] = {5, 7, 11, 13, 17, 23, 27};

QMazeWindow::QMazeWindow ()
  : QMainWindow ()
{
  setCursor (Qt::BlankCursor);
  fullMaze = false;
  myLevel = 1;
  complMazes = 0;
  cameFrom = 16;
  the_maze = new Maze (level[myLevel], level[myLevel - 1]);
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
      
      QFont textFont = painter->font ();
      textFont.setPointSize (30);
      painter->setFont (textFont);
      if (cll.id.id == maze->getStart ().id)  {
        painter->setPen (QColor (0, 128, 255));
        painter->drawText (cells[ix][iy], Qt::AlignCenter, tr ("Start"));
      }
      if (cll.id.id == maze->getFinish ().id)  {
        painter->setPen (QColor (255, 128, 0));
        painter->drawText (cells[ix][iy], Qt::AlignCenter, tr ("Finish"));
      }
      
      painter->setPen (Qt::white);
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
    painter->drawText (corners[1][1], Qt::AlignCenter, tr ("Start"));
  }
  
  if (myPosition == maze->getFinish ().id)  {
    painter->drawText (corners[1][1], Qt::AlignCenter, tr ("Finish - Press [Enter] to get to the next level!"));
  }
  
  /* Draw info box. */
  QRect infoBox;
  infoBox.setWidth (210);
  infoBox.setHeight (260);
  infoBox.moveTopRight (rect ().topRight ());
  QPen infoBoxStyle (Qt::white, 2, Qt::DashDotLine);
  painter->fillRect (infoBox, Qt::black);
  QPoint ctr = infoBox.center ();
  infoBox.setWidth (200);
  infoBox.setHeight (250);
  infoBox.moveCenter (ctr);
  painter->setPen (infoBoxStyle);
  painter->drawRect (infoBox);
  
  QRect infoText = QRect (0, 0, infoBox.width () - 10, 50);
  infoText.moveTopRight (infoBox.topRight ());
  painter->drawText (infoText, Qt::AlignLeft, tr ("Level: %1").arg (myLevel));
  infoText.moveTop (infoText.bottom ());
  painter->drawText (infoText, Qt::AlignLeft, tr ("%1 mazes completed").arg (complMazes));
  infoText.moveTop (infoText.bottom ());
  painter->drawText (infoText, Qt::AlignLeft, tr ("[Esc]: Quit application"));
  infoText.moveTop (infoText.bottom ());
  painter->drawText (infoText, Qt::AlignLeft, tr ("[F]: Show maze map"));
  infoText.moveTop (infoText.bottom ());
  painter->drawText (infoText, Qt::AlignLeft, tr ("[C]: Show current cell"));  
  
  
  /* Draw arrow. */
  QPen arrow_style (Qt::white, 5);
  painter->setPen (arrow_style);
  Direction dir = (Direction) cameFrom;
  switch (dir)  {
    case D_TOP:  {
      QPoint outer = QPoint (corners[1][2].center ().x (), corners[1][2].bottom ());
      painter->drawLine (outer, corners[1][2].center ());
      QPainterPath arrowPointPath;
      QPolygonF arrowPoint;
      QPointF tri = corners[1][2].center ();
      tri.rx () += 10;
      arrowPoint << tri;
      tri.rx () -= 20;
      arrowPoint << tri;
      tri.rx () += 10;
      tri.ry () -= 17;
      arrowPoint << tri;
      arrowPointPath.addPolygon (arrowPoint);
      painter->fillPath (arrowPointPath, Qt::white);
      break;
    }
    
    case D_BOTTOM:  {
      QPoint outer = QPoint (corners[1][0].center ().x (), corners[1][0].top ());
      painter->drawLine (outer, corners[1][0].center ());
      QPainterPath arrowPointPath;
      QPolygonF arrowPoint;
      QPointF tri = corners[1][0].center ();
      tri.rx () += 10;
      arrowPoint << tri;
      tri.rx () -= 20;
      arrowPoint << tri;
      tri.rx () += 10;
      tri.ry () += 17;
      arrowPoint << tri;
      arrowPointPath.addPolygon (arrowPoint);
      painter->fillPath (arrowPointPath, Qt::white);
      break;
    }
    
    case D_RIGHT:  {
      QPoint outer = QPoint (corners[2][1].right (), corners[2][1].center ().y ());
      painter->drawLine (outer, corners[2][1].center ());
      QPainterPath arrowPointPath;
      QPolygonF arrowPoint;
      QPointF tri = corners[2][1].center ();
      tri.ry () += 10;
      arrowPoint << tri;
      tri.ry () -= 20;
      arrowPoint << tri;
      tri.ry () += 10;
      tri.rx () -= 17;
      arrowPoint << tri;
      arrowPointPath.addPolygon (arrowPoint);
      painter->fillPath (arrowPointPath, Qt::white);
      break;
    }
    
    case D_LEFT:  {
      QPoint outer = QPoint (corners[0][1].left (), corners[0][1].center ().y ());
      painter->drawLine (outer, corners[0][1].center ());
      QPainterPath arrowPointPath;
      QPolygonF arrowPoint;
      QPointF tri = corners[0][1].center ();
      tri.ry () += 10;
      arrowPoint << tri;
      tri.ry () -= 20;
      arrowPoint << tri;
      tri.ry () += 10;
      tri.rx () += 17;
      arrowPoint << tri;
      arrowPointPath.addPolygon (arrowPoint);
      painter->fillPath (arrowPointPath, Qt::white);
      break;
    }
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
        cameFrom = (uint8_t) D_BOTTOM;
        break;
      }
      
      case D_RIGHT:  {
        pos.coord.x++;
        myPosition = pos.id;
        cameFrom = (uint8_t) D_LEFT;
        break;
      }
      
      case D_BOTTOM:  {
        pos.coord.y--;
        myPosition = pos.id;
        cameFrom = (uint8_t) D_TOP;
        break;
      }
      
      case D_LEFT:  {
        pos.coord.x--;
        myPosition = pos.id;
        cameFrom = (uint8_t) D_RIGHT;
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
    
    /* Pressed [Enter]. */
    case 16777220:  {
      if (myPosition == the_maze->getFinish ().id)  {
        delete the_maze;
        if (myLevel < (COUNT_LEVEL - 1))  {
          myLevel++;
        }
        complMazes++;
        the_maze = new Maze (level[myLevel], level[myLevel - 1]);
        myPosition = the_maze->getStart ().id;
        cameFrom = 16;
        repaint ();
      }
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