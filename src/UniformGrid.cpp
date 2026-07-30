#include "UniformGrid.h"
#include "utils/Float2.h"
#include <cmath>
#include <iostream>

UniformGrid::UniformGrid(float cellSize, float minX, float minY, float maxX,
                         float maxY)
    : m_cellSize(cellSize), m_minX(minX), m_minY(minY), m_maxX(maxX),
      m_maxY(maxY) {

  m_width = static_cast<int>(std::ceil((m_maxX - m_minX) / cellSize));
  m_height = static_cast<int>(std::ceil((m_maxY - m_minY) / cellSize));
  m_cells.resize(m_width * m_height);
}
void UniformGrid::clear() {
  for (auto &cell : m_cells) {
    cell.particleIndices.clear();
  }
}

void UniformGrid::rebuild(const Float2 *positions, int particleCount) {
  clear();
  for (int i = 0; i < particleCount; i++) {
    GridCoord cellCoord = getCellCoordFromPos(positions[i]);
    if (!isValidCell(cellCoord.x, cellCoord.y)) {
      continue;
    }
    int cellIndex = getCellIndex(cellCoord.x, cellCoord.y);
    m_cells[cellIndex].particleIndices.push_back(i);
  }
}

int UniformGrid::getCellIndex(int cellX, int cellY) const {
  return cellY * m_width + cellX;
}

const Cell &UniformGrid::getCell(int cellX, int cellY) const {
  int cellIndex = getCellIndex(cellX, cellY);
  return m_cells[cellIndex];
}

bool UniformGrid::isValidCell(int cellX, int cellY) const {
  return cellX >= 0 && cellX < m_width && cellY >= 0 && cellY < m_height;
}
GridCoord UniformGrid::getCellCoordFromPos(const Float2 &pos) const {
  int cellX = static_cast<int>(std::floor((pos.x - m_minX) / m_cellSize));
  int cellY = static_cast<int>(std::floor((pos.y - m_minY) / m_cellSize));
  return {cellX, cellY};
}
