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
  CellID start, finish;
  
public:
  Maze (const uint16_t, const uint16_t);
  virtual ~Maze ();
  
  void refresh ();
  bool canGo (const CellID, const Direction);
  
  uint16_t get_x () const;
  uint16_t get_y () const;
  CellID getStart () const;
  CellID getFinish () const;
  
  Cell& cellAt (const uint16_t, const uint16_t);
  Cell& cellAt (const CellID);
  
  uint32_t countUnvisited ();
  void connectCells (CellID, CellID);
  std::vector<CellID> getUnvisitedNeighbors (CellID);
  
  void generate ();
};

#endif