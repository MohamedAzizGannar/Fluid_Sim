#include "App.h"
#include "Config.h"
#include "SDLRenderer.h"
#include "Simulation.h"
#include <SDL2/SDL.h>
#include <iostream>

int main() {
  BoundingBox m_box;
  m_box.minX = 10.f;
  m_box.minY = 10.f;
  m_box.maxY = Config::windowHeight - 10.f;
  m_box.maxX = Config::windowWidth - 10.f;
  SDLRenderer renderer(Config::windowWidth, Config::windowHeight, "Hello");
  Simulation sim(Config::countX, Config::countY, Config::spacing, m_box);
  App app(&renderer, &sim);
  app.run();
  app.shutdown();

  SDL_Quit();
  return 0;
}
