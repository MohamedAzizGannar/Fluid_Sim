#pragma once
#include "SDL2/SDL.h"
class IRenderer {
public:
  virtual ~IRenderer() = default;
  virtual bool initRenderer() = 0;
  virtual void beginFrame() = 0;
  virtual void endFrame() = 0;
  virtual void drawCircle(float x, float y) = 0;
  virtual void drawCircle(float x, float y, SDL_Color color) = 0;
  virtual void drawPoints(const SDL_Point *points, int count,
                          SDL_Color color) = 0;
  virtual void shutdown() = 0;
};
