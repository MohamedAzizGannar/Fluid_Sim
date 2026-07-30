
#pragma once
#include "IRenderer.h"
#include "SDL2/SDL.h"
#include <string>

class SDLRenderer : public IRenderer {

public:
  SDLRenderer(float w, float h, const char *title);
  ~SDLRenderer() override;

  bool initRenderer() override;
  void shutdown() override;

  void beginFrame() override;

  void endFrame() override;
  void drawTriangle(SDL_Point a, SDL_Point b, SDL_Point c);
  void drawCircle(float x, float y) override;
  void drawCircle(float x, float y, SDL_Color color) override;

private:
  float m_w;
  float m_h;
  std::string m_title;
  SDL_Window *m_window = nullptr;
  SDL_Renderer *m_renderer = nullptr;
};
