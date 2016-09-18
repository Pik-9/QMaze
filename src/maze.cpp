#include "maze.hpp"
#include "random.hpp"

#include <stack>

Cell::Cell ()
{
  walls = 15;
}

bool Cell::visited ()
{
  return (walls & 16);
}

void Cell::setWall (Direction dir)
{
  walls |= (1 << (int) dir);
}

void Cell::unsetWall (Direction dir)
{
  walls &= ~(1 << (int) dir);
}

Maze::Maze (const uint16_t x, const uint16_t y)
  : x_cells (x), y_cells (y)
{
  cells = new Cell[x * y];
  for (uint16_t ix = 1; ix <= x; ++ix)  {
    for (uint16_t iy = 1; iy <= y; ++iy)  {
      cellAt (ix, iy).id.coord.x = ix;
      cellAt (ix, iy).id.coord.y = iy;
    }
  }
}

Maze::~Maze ()
{
  delete[] cells;
}

uint16_t Maze::get_x () const
{
  return x_cells;
}

uint16_t Maze::get_y () const
{
  return y_cells;
}

CellID Maze::getStart () const
{
  return start;
}

CellID Maze::getFinish () const
{
  return finish;
}

Cell& Maze::cellAt (const uint16_t x, const uint16_t y)
{
  if ((x > x_cells) || (y > y_cells))  {
    throw ECellOutOfMaze ();
  }
  return cells[x_cells * (y - 1) + x - 1];
}

Cell& Maze::cellAt (const CellID c)
{
  return cellAt (c.coord.x, c.coord.y);
}

uint32_t Maze::countUnvisited ()
{
  uint32_t RET = 0;
  for (uint32_t ii = 0; ii < x_cells * y_cells; ++ii)  {
    if (!cells[ii].visited ())  {
      RET++;
    }
  }
  
  return RET;
}

void Maze::connectCells (CellID a, CellID b)
{
  int32_t diff_x = a.coord.x - b.coord.x;
  int32_t diff_y = a.coord.y - b.coord.y;
  
  if ((diff_x == 0) && (diff_y == 1))  {
    cellAt (b).unsetWall (D_BOTTOM);
    cellAt (a).unsetWall (D_TOP);
  } else if ((diff_x == 0) && (diff_y == -1))  {
    cellAt (b).unsetWall (D_TOP);
    cellAt (a).unsetWall (D_BOTTOM);
  } else if ((diff_x == 1) && (diff_y == 0))  {
    cellAt (a).unsetWall (D_LEFT);
    cellAt (b).unsetWall (D_RIGHT);
  } else if ((diff_x == -1) && (diff_y == 0))  {
    cellAt (a).unsetWall (D_RIGHT);
    cellAt (b).unsetWall (D_LEFT);
  }
}

std::vector<CellID> Maze::getUnvisitedNeighbors (CellID id)
{
  std::vector<CellID> RET;
  CellID tmp = id;
  
  if (id.coord.x > 1)  {
    tmp.coord.x--;
    if (!cellAt (tmp).visited ())  {
      RET.push_back (tmp);
    }
    tmp.coord.x++;
  }
  
  if (id.coord.x < x_cells)  {
    tmp.coord.x++;
    if (!cellAt (tmp).visited ())  {
      RET.push_back (tmp);
    }
    tmp.coord.x--;
  }
  
  if (id.coord.y > 1)  {
    tmp.coord.y--;
    if (!cellAt (tmp).visited ())  {
      RET.push_back (tmp);
    }
    tmp.coord.y++;
  }
  
  if (id.coord.y < y_cells)  {
    tmp.coord.y++;
    if (!cellAt (tmp).visited ())  {
      RET.push_back (tmp);
    }
    tmp.coord.y--;
  }
  
  return RET;
}

void Maze::generate ()
{
  std::stack<uint32_t> cs;
  Random rr;
  
  start.coord.x = rr.getNumber (1, x_cells);
  start.coord.y = rr.getNumber (1, y_cells);
  CellID current = start;
  cellAt (current).walls |= 16;
  while (countUnvisited ())  {
    std::vector<CellID> ns = getUnvisitedNeighbors (current);
    if (ns.size ())  {
      CellID next = ns[rr.getNumber (0, ns.size () - 1)];
      cs.push (current.id);
      connectCells (current, next);
      current = next;
      cellAt (current).walls |= 16;
    } else  {
      if (cs.empty ())  {
        break;
      } else  {
        current.id = cs.top ();
        cs.pop ();
      }
    }
  }
  finish = current;
}