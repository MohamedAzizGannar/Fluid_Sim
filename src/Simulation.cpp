#include "Simulation.h"
#include "Config.h"
#include "SDL.h"
#include "UniformGrid.h"
#include "utils/Float2.h"
#include "utils/Polies.h"
#include <iostream>

Simulation::Simulation(int countX, int countY, float spacing, BoundingBox box)
    : m_countX(countX), m_countY(countY), m_spacing(spacing), m_box(box),
      m_grid(Config::h / 2.f, m_box.minX, m_box.minY, m_box.maxX, m_box.maxY) {}

void Simulation::initBlock() {
  int index = 0;
  for (int i = 0; i < m_countX; i++) {
    for (int j = 0; j < m_countY; j++) {
      float jitterX = ((rand() % 100) / 100.f - 0.5f) * m_spacing * 0.1f;
      float jitterY = ((rand() % 100) / 100.f - 0.5f) * m_spacing * 0.1f;
      Float2 c_pos = Float2(m_spacing * i + Config::offsetX + jitterX,
                            m_spacing * j + Config::offsetY + jitterY);

      Float2 zeroes = Float2(0.f, 0.f);
      m_positions[index] = c_pos;
      m_velocities[index] = zeroes;
      m_acceleration[index] = zeroes;
      m_densities[index] = 0.f;
      m_pressures[index] = 0.f;
      m_forces[index] = zeroes;
      index++;
    }
  }
  m_activeCount = m_fluidCount;
}
void Simulation::initBoundary() {
  float s = Config::spacing;
  for (int layer = 0; layer < Config::boundary_layers; layer++) {
    float y = m_box.maxY + layer * s;
    for (float x = m_box.minX; x <= m_box.maxX; x += s) {
      addBoundaryParticle(Float2(x, y));
    }
  }
  for (int layer = 0; layer < Config::boundary_layers; layer++) {
    float x = m_box.maxX + layer * s;
    for (float y = m_box.minY; y <= m_box.maxY; y += s) {
      addBoundaryParticle(Float2(x, y));
    }
  }
  for (int layer = 0; layer < Config::boundary_layers; layer++) {
    float x = m_box.minX - layer * s;
    for (float y = m_box.minY; y <= m_box.maxY; y += s) {
      addBoundaryParticle(Float2(x, y));
    }
  }
}

void Simulation::addBoundaryParticle(Float2 pos) {
  int i = m_activeCount++;
  m_positions[i] = pos;
  m_velocities[i] = Float2(0.f, 0.f);
  m_acceleration[i] = Float2(0.f, 0.f);
  m_forces[i] = Float2(0.f, 0.f);
  m_densities[i] = Config::REST_DENSITY;
  m_pressures[i] = 0.f;
}
void Simulation::step(float dt) {
  clearForces();
  m_grid.rebuild(m_positions, m_activeCount);

  auto tA = SDL_GetPerformanceCounter();
  calculateDensity();
  auto tB = SDL_GetPerformanceCounter();
  calculatePressure();
  auto tC = SDL_GetPerformanceCounter();
  applyForces();
  auto tD = SDL_GetPerformanceCounter();
  integrate(dt);
  auto tE = SDL_GetPerformanceCounter();
  resolveCollisions();
}

void Simulation::integrate(float dt) {

  constexpr float maxSpeedAllowed = 80.f; // tune to your scene scale
  avg_density = 0.f;
  avg_pressure = 0.f;
  avg_speed = 0.f;
  max_speed = 0.f;

  for (int i = 0; i < m_fluidCount; i++) {
    m_acceleration[i] = m_forces[i];
    m_velocities[i] += m_acceleration[i] * dt;
    if (m_velocities[i].length() > maxSpeedAllowed) {
      m_velocities[i] =
          m_velocities[i] * (maxSpeedAllowed / m_velocities[i].length());
    }
    m_positions[i] += m_velocities[i] * dt;
    avg_speed += m_velocities[i].length();
    avg_density += m_densities[i];
    avg_pressure += m_pressures[i];
    if (m_velocities[i].length() > max_speed) {
      max_speed = m_velocities[i].length();
    }
  }
  avg_speed /= m_fluidCount;
  avg_density /= m_fluidCount;
  avg_pressure /= m_fluidCount;
}
void Simulation::applyGravity() {
  for (int i = 0; i < m_fluidCount; i++) {
    m_forces[i] += Float2(0.f, Config::GRAVITY_COEF);
  }
}
void Simulation::applyForces() {
  applyPressureAndViscosityForce();
  applyGravity();
}

void Simulation::resolveCollisions() {
  for (int i = 0; i < m_fluidCount; i++) {
    if (m_positions[i].x <= m_box.minX) {
      m_positions[i] = Float2(m_box.minX + 1.f, m_positions[i].y);
      m_velocities[i] = Float2(m_velocities[i].x * (-Config::RESTITUTION_COEF),
                               m_velocities[i].y);
    }
    if (m_positions[i].x >= m_box.maxX) {
      m_positions[i] = Float2(m_box.maxX - 1.f, m_positions[i].y);
      m_velocities[i] = Float2(m_velocities[i].x * (-Config::RESTITUTION_COEF),
                               m_velocities[i].y);
    }
    if (m_positions[i].y <= m_box.minY) {
      m_positions[i] = Float2(m_positions[i].x, m_box.minY + 1.f);
      m_velocities[i] = Float2(m_velocities[i].x,
                               m_velocities[i].y * (-Config::RESTITUTION_COEF));
    }
    if (m_positions[i].y >= m_box.maxY) {
      m_positions[i] = Float2(m_positions[i].x, m_box.maxY - 1.f);
      m_velocities[i] = Float2(m_velocities[i].x,
                               m_velocities[i].y * (-Config::RESTITUTION_COEF));
    }
  }
}

void Simulation::calculateDensity() {
  for (int i = 0; i < m_activeCount; i++) {
    m_densities[i] = 0.f;
    GridCoord cellCoord = m_grid.getCellCoordFromPos(m_positions[i]);
    for (int dx = -2; dx <= 2; dx++) {
      for (int dy = -2; dy <= 2; dy++) {
        int nx = cellCoord.x + dx;
        int ny = cellCoord.y + dy;
        if (!m_grid.isValidCell(nx, ny))
          continue;
        const Cell &c_cell = m_grid.getCell(nx, ny);
        for (int j : c_cell.particleIndices) {
          float r2 = distanceSqrd(m_positions[i], m_positions[j]);
          if (r2 >= Config::h2)
            continue;
          m_densities[i] += Config::PARTICLE_MASS * WPoly6(r2);
        }
      }
    }
    if (i >= m_fluidCount && m_densities[i] < Config::REST_DENSITY) {
      m_densities[i] = Config::REST_DENSITY;
    }
    m_densities[i] = std::max(m_densities[i], Config::REST_DENSITY * 0.2f);
  }
}
void Simulation::calculatePressure() {

  for (int i = 0; i < m_activeCount; i++) {
    m_pressures[i] = Config::GAS_CONSTANT *
                     std::max(m_densities[i] - Config::REST_DENSITY, 0.f);
  }
}

void Simulation::applyPressureAndViscosityForce() {
  avg_neighbor_count = 0.f;

  constexpr float minDensityForForce = Config::REST_DENSITY * 0.3f;
  for (int i = 0; i < m_fluidCount; i++) {
    Float2 viscForce = Float2(0.f, 0.f);
    Float2 pressureForce = Float2(0.f, 0.f);

    GridCoord cellCoord = m_grid.getCellCoordFromPos(m_positions[i]);
    for (int dx = -2; dx <= 2; dx++) {
      for (int dy = -2; dy <= 2; dy++) {
        int nx = cellCoord.x + dx;
        int ny = cellCoord.y + dy;

        if (!m_grid.isValidCell(nx, ny))
          continue;
        const Cell &c_cell = m_grid.getCell(nx, ny);
        for (int j : c_cell.particleIndices) {
          if (i == j)
            continue;
          float r2 = distanceSqrd(m_positions[i], m_positions[j]);
          if (r2 >= Config::h2)
            continue;
          if (m_densities[i] <= minDensityForForce ||
              m_densities[j] <= minDensityForForce)
            continue;
          Float2 dir = (m_positions[i] - m_positions[j]);
          float coeff = (m_pressures[i] / (m_densities[i] * m_densities[i])) +
                        m_pressures[j] / (m_densities[j] * m_densities[j]);
          pressureForce -= GradientSpiky(dir) * coeff * Config::PARTICLE_MASS;
          if (m_densities[j] <= 1e-2)
            continue;
          Float2 coeff_visc =
              (m_velocities[j] - m_velocities[i]) / m_densities[j];
          float lapl = ViscosityLaplactian(sqrt(r2));
          viscForce += coeff_visc * lapl * Config::PARTICLE_MASS;
          avg_neighbor_count++;
        }
      }
    }

    m_forces[i] += viscForce * Config::VISCOSITY_COEFF;
    m_forces[i] += pressureForce;
  }
  avg_neighbor_count /= m_fluidCount;
}
void Simulation::clearForces() {
  for (int i = 0; i < m_activeCount; i++) {
    m_forces[i] = Float2(0.f, 0.f);
  }
}
const Float2 *Simulation::getPositions() { return m_positions; }
