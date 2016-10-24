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

#ifndef MAZE_HPP
#define MAZE_HPP

#include <stdint.h>
#include <vector>
#include <exception>

union CellID
{
  uint32_t id;
  struct
  {
    uint16_t x;
    uint16_t y;
  } coord;
};

enum Direction  {
  D_TOP = 1,
  D_BOTTOM = 2,
  D_RIGHT = 4,
  D_LEFT = 8
};

struct Cell
{
  CellID id;
  uint8_t walls;
  
  Cell ();
  
  bool visited ();
  
  void setWall (Direction);
  void unsetWall (Direction);
  bool isWallSet (Direction);
};

class ECellOutOfMaze : public std::exception {};

class Maze
{
private:
  uint16_t x_cells, y_cells;
  Cell *cells;
  CellID start, finish, key;
  
protected:
  void refresh ();
  
  uint32_t countUnvisited ();
  void connectCells (CellID, CellID);
  std::vector<CellID> getUnvisitedNeighbors (CellID);
  
  void generate ();
  
public:
  Maze (const uint16_t, const uint16_t);
  virtual ~Maze ();
  
  bool canGo (const CellID, const Direction);
  
  uint16_t get_x () const;
  uint16_t get_y () const;
  CellID getStart () const;
  CellID getFinish () const;
  CellID getKeyField () const;
  
  Cell& cellAt (const uint16_t, const uint16_t);
  Cell& cellAt (const CellID);
};

#endif