#include "./../include/Renderer.h"
#include <cstddef>
#include <iostream>

Renderer::Renderer() = default;

Renderer::~Renderer() { shutdown(); }

bool Renderer::initRenderer(int width, int height, const char *title) {

  m_window =
      SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       width, height, SDL_WINDOW_SHOWN);
  if (!m_window) {
    std::cerr << "Failed to Create Window; " << SDL_GetError() << std::endl;
    return false;
  }

  m_renderer = SDL_CreateRenderer(m_window, 1, SDL_RENDERER_ACCELERATED);
  if (!m_renderer) {
    std::cerr << "Failed to Create Renderer; " << SDL_GetError() << std::endl;
    return false;
  }
  return true;
}
void Renderer::shutdown() {
  if (m_renderer) {
    SDL_DestroyRenderer(m_renderer);
    m_renderer = nullptr;
  }

  if (m_window) {

    SDL_DestroyWindow(m_window);
    m_window = nullptr;
  }
}

void Renderer::beginFrame() {
  SDL_SetRenderDrawColor(m_renderer, 10, 10, 40, 255);
  SDL_RenderClear(m_renderer);
}
void Renderer::endFrame() { SDL_RenderPresent(m_renderer); }
void Renderer::drawTriangle(SDL_Point a, SDL_Point b, SDL_Point c) {
  SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
  SDL_RenderDrawLine(m_renderer, a.x, a.y, b.x, b.y);
  SDL_RenderDrawLine(m_renderer, a.x, a.y, c.x, c.y);
  SDL_RenderDrawLine(m_renderer, c.x, c.y, b.x, b.y);
}
