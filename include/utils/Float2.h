#pragma once
#include <cmath>

struct Float2 {
  float x;
  float y;

  Float2() : x(0.0f), y(0.0f) {}
  Float2(float _x, float _y) : x(_x), y(_y) {}

  Float2 operator+(const Float2 &f) const { return Float2(x + f.x, y + f.y); }

  Float2 operator-(const Float2 &f) const { return Float2(x - f.x, y - f.y); }

  Float2 operator*(float f) const { return Float2(x * f, y * f); }

  Float2 operator/(float f) const { return Float2(x / f, y / f); }

  Float2 &operator+=(const Float2 &f) {
    x += f.x;
    y += f.y;
    return *this;
  }

  Float2 &operator-=(const Float2 &f) {
    x -= f.x;
    y -= f.y;
    return *this;
  }

  Float2 &operator*=(float f) {
    x *= f;
    y *= f;
    return *this;
  }

  Float2 &operator/=(float f) {
    x /= f;
    y /= f;
    return *this;
  }

  float lengthSq() const { return x * x + y * y; }

  float length() const { return std::sqrt(lengthSq()); }
};

float distance(const Float2 &a, const Float2 &b);

float distanceSqrd(const Float2 &a, const Float2 &b);
