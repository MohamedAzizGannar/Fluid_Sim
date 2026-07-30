
#include "SDLRenderer.h"
#include <SDL_pixels.h>
#include <SDL_render.h>
#include <cstddef>
#include <iostream>

SDLRenderer::SDLRenderer(float w, float h, const char *title)
    : m_w(w), m_h(h), m_title(title) {}

SDLRenderer::~SDLRenderer() { shutdown(); }

bool SDLRenderer::initRenderer() {

  m_window =
      SDL_CreateWindow(m_title.c_str(), SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, m_w, m_h, SDL_WINDOW_SHOWN);
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
void SDLRenderer::shutdown() {
  if (m_renderer) {
    SDL_DestroyRenderer(m_renderer);
    m_renderer = nullptr;
  }

  if (m_window) {

    SDL_DestroyWindow(m_window);
    m_window = nullptr;
  }
}

void SDLRenderer::beginFrame() {
  SDL_SetRenderDrawColor(m_renderer, 10, 10, 40, 255);
  SDL_RenderClear(m_renderer);
}
void SDLRenderer::endFrame() { SDL_RenderPresent(m_renderer); }

void SDLRenderer::drawTriangle(SDL_Point a, SDL_Point b, SDL_Point c) {
  SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
  SDL_RenderDrawLine(m_renderer, a.x, a.y, b.x, b.y);
  SDL_RenderDrawLine(m_renderer, a.x, a.y, c.x, c.y);
  SDL_RenderDrawLine(m_renderer, c.x, c.y, b.x, b.y);
}
void SDLRenderer::drawCircle(float x, float y) {
  SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, 255);

  SDL_RenderDrawPoint(m_renderer, x, y);
}
void SDLRenderer::drawCircle(float x, float y, SDL_Color color) {
  SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);

  SDL_RenderDrawPoint(m_renderer, x, y);
}
void SDLRenderer::drawPoints(const SDL_Point *points, int count,
                             SDL_Color color) {

  SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
  SDL_RenderDrawPoints(m_renderer, points, count);
}
