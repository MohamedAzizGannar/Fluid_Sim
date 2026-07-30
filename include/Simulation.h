#pragma once

#include "Config.h"
#include "UniformGrid.h"
#include "utils/Float2.h"

struct BoundingBox {
  float minX, maxX;
  float minY, maxY;
};
class Simulation {
public:
  Simulation(int countX, int countY, float spacing, BoundingBox box);

  void initBlock();
  void initBoundary();

  int getParticleCount() const { return m_activeCount; }
  bool isBoundary(int i) const { return i >= m_fluidCount; }
  void step(float dt);

  const Float2 *getPositions();
  const Float2 *getVelocities() { return m_velocities; }

  float avg_density;
  float avg_pressure;
  float avg_speed;
  float max_speed;
  float avg_neighbor_count;

private:
  void integrate(float dt);
  void resolveCollisions();

  void applyForces();
  void clearForces();

  void calculatePredictedPositions(float dt);
  void calculateDensity();
  void calculatePressure();

  void applyGravity();
  void applyPressureAndViscosityForce();
  void applyViscosityForce();
  void addBoundaryParticle(Float2 pos);

private:
  const static int m_fluidCount = Config::countX * Config::countY;

  const static int m_max_particle_count =
      m_fluidCount + Config::max_boundary_count;

  int m_activeCount = 0;

  Float2 m_positions[m_max_particle_count];
  Float2 m_velocities[m_max_particle_count];
  Float2 m_acceleration[m_max_particle_count];
  Float2 m_forces[m_max_particle_count];

  float m_densities[m_max_particle_count];
  float m_pressures[m_max_particle_count];

  int m_countX;
  int m_countY;

  float m_spacing;

  BoundingBox m_box;
  UniformGrid m_grid;
};
