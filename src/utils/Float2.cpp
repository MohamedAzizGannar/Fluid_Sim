#include "utils/Float2.h"
#include <cmath>
float distance(const Float2 &a, const Float2 &b) {
  float x = a.x - b.x;
  float y = a.y - b.y;
  return std::sqrt(x * x + y * y);
}
float distanceSqrd(const Float2 &a, const Float2 &b) {
  float x = a.x - b.x;
  float y = a.y - b.y;
  return (x * x + y * y);
}
