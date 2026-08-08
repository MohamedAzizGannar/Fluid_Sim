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

  const Float2 *positions = m_simulation->getPositions();
  const Float2 *velocities = m_simulation->getVelocities();
  int count = m_simulation->getParticleCount();

  SDL_Color color;
  for (int i = 0; i < count; i++) {
    if (m_simulation->isBoundary(i)) {
      continue;
    }
    float speed = velocities[i].length();
    float t = std::clamp(speed / Config::maxSpeedAllowed, 0.0f, 1.0f);

    if (t < 0.33f) {
      float u = t / 0.33f;
      color.r = 0;
      color.g = static_cast<Uint8>(255.0f * u);
      color.b = 255;
    } else if (t < 0.66f) {
      float u = (t - 0.33f) / 0.33f;
      color.r = static_cast<Uint8>(255.0f * u);
      color.g = 255;
      color.b = static_cast<Uint8>(255.0f * (1.0f - u));
    } else {
      float u = (t - 0.66f) / 0.34f;
      color.r = 255;
      color.g = static_cast<Uint8>(255.0f * (1.0f - u));
      color.b = 0;
    }
    color.a = 255;

    m_renderer->drawCircle(positions[i].x, positions[i].y, color);
  }

  m_renderer->endFrame();
}
void App::renderOptimized() {
  m_renderer->beginFrame();
  SDL_Color blue;
  blue.b = 255;
  blue.r = 0;
  blue.g = 0;
  blue.a = 255;
  const int count = m_simulation->getParticleCount();
  const Float2 *positions = m_simulation->getPositions();
  SDL_Point points[count];
  for (int i = 0; i < count; i++) {
    points[i].x = positions[i].x;
    points[i].y = positions[i].y;
  }
  m_renderer->drawPoints(points, count, blue);
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
    while (accumulator > 0.f && stepsThisFrame < maxStepsPerFrame) {
      float dt = m_simulation->calculateStableTimeStep();
      dt = std::min(dt, static_cast<float>(accumulator));
      auto t0 = SDL_GetPerformanceCounter();
      m_simulation->stepMP(dt);
      auto t1 = SDL_GetPerformanceCounter();
      double ms = (t1 - t0) * 1000.0 / SDL_GetPerformanceFrequency();
      accumulator -= dt;
      stepsThisFrame++;
      stepTimeSum += ms;
      stepCountThisSecond++;
    }
    if (stepsThisFrame >= maxStepsPerFrame) {
      accumulator = 0.0; // drop debt rather than let it compound forever
      std::cout << "Falling behind: capped at " << stepsThisFrame << " steps\n";
    }
    renderOptimized();
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
