#pragma once
#include "SDL2/SDL.h"

class Renderer {

public:
  Renderer();
  ~Renderer();

  bool initRenderer(int width, int height, const char *title);
  void shutdown();

  void beginFrame();

  void endFrame();
  void drawTriangle(SDL_Point a, SDL_Point b, SDL_Point c);

private:
  SDL_Window *m_window = nullptr;
  SDL_Renderer *m_renderer = nullptr;
};
