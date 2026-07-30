
#include "utils/Polies.h"
#include "Config.h"
#include "utils/Float2.h"
#include <algorithm>
float WPoly6(float r_sqrd) {

  if (r_sqrd < Config::h2) {
    float coef = 4.f / (M_PI * Config::h8);
    return coef * (Config::h2 - r_sqrd) * (Config::h2 - r_sqrd) *
           (Config::h2 - r_sqrd);
  }
  return 0;
}
Float2 GradientSpiky(const Float2 &r_ij) {
  float r = r_ij.length();
  if (r <= 0.f || r >= Config::h) {
    return Float2(0.f, 0.f);
  }
  r = std::max(r_ij.length(), 1e-4f);
  float coef = -10.f / (M_PI * Config::h5);
  float diff = Config::h - r;
  return r_ij * coef * diff * diff / r;
}
float ViscosityLaplactian(float r) {
  if (r <= 0.f || r >= Config::h) {
    return 0;
  }
  float coeff = 40.f / (M_PI * Config::h5);
  return coeff * (Config::h - r);
}
