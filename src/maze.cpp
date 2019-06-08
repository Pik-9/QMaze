/******************************************************************************
 *  QMaze - A simple maze game                                                *
 *  Copyright 2016 Daniel Steinhauer <d.steinhauer@mailbox.org>               *
 *                                                                            *
 *                                                                            *
 *  This file is part of QMaze                                                *
 *                                                                            *
 *  QMaze is free software: you can redistribute it and/or modify             *
 *  it under the terms of the GNU General Public License as published by      *
 *  the Free Software Foundation, either version 3 of the License, or         *
 *  (at your option) any later version.                                       *
 *                                                                            *
 *  QMaze is distributed in the hope that it will be useful,                  *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 *  GNU General Public License for more details.                              *
 *                                                                            *
 *  You should have received a copy of the GNU General Public License         *
 *  along with QMaze.  If not, see <http://www.gnu.org/licenses/>.            *
 ******************************************************************************/

#include "maze.hpp"

#include <stack>
#include <random>
#include <stdlib.h>

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
  walls |= (uint8_t) dir;
}

void Cell::unsetWall (Direction dir)
{
  walls &= ~((int) dir);
}

bool Cell::isWallSet (Direction dir)
{
  uint8_t shift = ((int) dir);
  bool RET = (walls & dir);
  return RET;
}

Maze::Maze (const uint16_t x, const uint16_t y)
  : x_cells (x), y_cells (y)
{
  cells.resize (x * y);
  uint8_t color = 0;
  for (uint16_t ix = 1; ix <= x; ++ix)  {
    for (uint16_t iy = 1; iy <= y; ++iy)  {
      cellAt (ix, iy).id.coord.x = ix;
      cellAt (ix, iy).id.coord.y = iy;
      cellAt (ix, iy).walls |= ((color++ % 6) << 5);
    }
  }

  generate ();
}

Maze::~Maze ()
{}

int32_t Maze::getRandomNr (const int32_t lb, const int32_t ub)
{
  static std::random_device rdevice;
  static std::mt19937 mt_engine (rdevice ());

  if (lb == ub)  {
    return lb;
  }

  return (ub > lb ?
    (mt_engine () % (ub - lb) + lb) :
    (mt_engine () % (lb - ub) + ub));
}

void Maze::refresh ()
{
}

bool Maze::canGo (const CellID field, const Direction dir)
{
  if ((dir == D_TOP)  && (field.coord.y >= y_cells))  {
    return false;
  }

  if ((dir == D_RIGHT)  && (field.coord.x >= x_cells))  {
    return false;
  }

  if ((dir == D_BOTTOM)  && (field.coord.y == 0))  {
    return false;
  }

  if ((dir == D_LEFT)  && (field.coord.x == 0))  {
    return false;
  }

  return (!cellAt (field).isWallSet (dir));
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

CellID Maze::getKeyField () const
{
  return key;
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
    cellAt (a).unsetWall (D_BOTTOM);
    cellAt (b).unsetWall (D_TOP);
  } else if ((diff_x == 0) && (diff_y == -1))  {
    cellAt (a).unsetWall (D_TOP);
    cellAt (b).unsetWall (D_BOTTOM);
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

  start.coord.x = getRandomNr (1, x_cells);
  start.coord.y = getRandomNr (1, y_cells);
  CellID current = start;
  cellAt (current).walls |= 16;
  while (countUnvisited ())  {
    std::vector<CellID> ns = getUnvisitedNeighbors (current);
    if (ns.size ())  {
      CellID next = ns[getRandomNr (0, ns.size ())];
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

  do  {
    uint32_t kfield = getRandomNr (0, x_cells * y_cells);
    div_t dv = div (kfield, x_cells);
    key.coord.x = dv.rem + 1;
    key.coord.y = dv.quot + 1;
  } while ((key.id == start.id) || (key.id == finish.id));
}