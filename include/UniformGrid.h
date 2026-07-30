#pragma once
#include "Config.h"
#include "tuple"
#include "utils/Float2.h"
#include <vector>

struct GridCoord {
  int x;
  int y;
};

struct Cell {
  std::vector<int> particleIndices;
};

class UniformGrid {

public:
  UniformGrid(float cellSize, float minX, float minY, float maxX, float maxY);

  void clear();
  void rebuild(const Float2 *positions, int particleCount);
  GridCoord getCellCoordFromPos(const Float2 &pos) const;

  int getCellIndex(int cellX, int cellY) const;
  bool isValidCell(int cellX, int cellY) const;

  const Cell &getCell(int cellX, int cellY) const;

private:
  int m_cellCount;
  float m_cellSize;

  float m_minX;
  float m_minY;
  float m_maxX;
  float m_maxY;

  int m_width;
  int m_height;
  std::vector<Cell> m_cells;
};
