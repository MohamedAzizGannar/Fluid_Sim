#pragma once

#include "IRenderer.h"
#include "Simulation.h"
class App {
public:
  App(IRenderer *renderer, Simulation *sim);
  void update(float dt);
  void run();
  void shutdown();

private:
  void render();
  bool m_running;
  IRenderer *m_renderer;
  Simulation *m_simulation;
};
