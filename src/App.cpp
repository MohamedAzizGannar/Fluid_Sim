#include "App.h"
#include "Config.h"
#include "IRenderer.h"
#include "SDL.h"
#include "Simulation.h"
#include <iostream>

App::App(IRenderer *renderer, Simulation *sim)
    : m_renderer(renderer), m_simulation(sim) {

  m_renderer->initRenderer();
  m_simulation->initBlock();
  m_simulation->initBoundary();
  m_running = true;
}
void App::update(float dt) {
  m_simulation->step(dt);
  render();
}
void App::render() {
  m_renderer->beginFrame();
  SDL_Color red;
  red.r = 255;
  red.b = 0;
  red.g = 0;
  red.a = 255;
  SDL_Color blue;
  blue.b = 255;
  blue.r = 0;
  blue.g = 0;
  blue.a = 255;

  const Float2 *positions = m_simulation->getPositions();
  int count = m_simulation->getParticleCount();

  for (int i = 0; i < count; i++) {
    if (m_simulation->isBoundary(i)) {
      continue;
    }

    m_renderer->drawCircle(positions[i].x, positions[i].y, blue);
  }

  m_renderer->endFrame();
}
void App::run() {

  Uint64 previous = SDL_GetPerformanceCounter();
  double accumulator = 0.0;
  double fpsTimer = 0.0;
  int frameCount = 0;
  int fps = 0;
  double stepTimeSum = 0.0;
  int stepCountThisSecond = 0;
  while (m_running) {
    Uint64 current = SDL_GetPerformanceCounter();
    double frameTime = static_cast<double>(current - previous) /
                       static_cast<double>(SDL_GetPerformanceFrequency());
    previous = current;

    if (frameTime > 0.25)
      frameTime = 0.25;
    accumulator += frameTime;
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT)
        m_running = false;
    }
    constexpr int maxStepsPerFrame = 8;
    int stepsThisFrame = 0;
    while (accumulator >= Config::timeStep &&
           stepsThisFrame < maxStepsPerFrame) {
      auto t0 = SDL_GetPerformanceCounter();
      m_simulation->step(Config::timeStep);
      auto t1 = SDL_GetPerformanceCounter();
      double ms = (t1 - t0) * 1000.0 / SDL_GetPerformanceFrequency();
      accumulator -= Config::timeStep;
      stepsThisFrame++;
      stepTimeSum += ms;
      stepCountThisSecond++;
    }
    if (stepsThisFrame >= maxStepsPerFrame) {
      accumulator = 0.0; // drop debt rather than let it compound forever
      std::cout << "Falling behind: capped at " << stepsThisFrame << " steps\n";
    }
    render();
    frameCount++;
    fpsTimer += frameTime;

    if (fpsTimer >= 1.0) {
      fps = frameCount;
      frameCount = 0;
      fpsTimer -= 1.0;
      double avgStepMs =
          stepCountThisSecond > 0 ? stepTimeSum / stepCountThisSecond : 0.0;
      std::cout << "FPS: " << fps << "\nAvg Step ms" << avgStepMs << "\n";
      std::cout << " Avg Speed : " << m_simulation->avg_speed
                << "\nAvg Density : " << m_simulation->avg_density
                << "\nAvg Pressure : " << m_simulation->avg_pressure
                << "\nMax Speed : " << m_simulation->max_speed
                << "\nAvg neighbor Count : " << m_simulation->avg_neighbor_count
                << "\n";
      stepTimeSum = 0.0;
      stepCountThisSecond = 0;
    }
  }
}
void App::shutdown() {
  m_running = false;
  m_renderer->shutdown();
}
