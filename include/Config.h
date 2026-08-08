#pragma once
namespace Config {

constexpr float maxSpeedAllowed = 180.f;
constexpr int windowWidth = 1000;
constexpr int windowHeight = 600;

constexpr float max_dt = 1.f / 60.f;
constexpr float cfl_factor = 0.1f;
constexpr float min_dt = 0.0001f;

constexpr float spacing = 8.f;

constexpr int countX = 100;
constexpr int countY = 40;

constexpr int offsetX = 90;
constexpr int offsetY = 90;

constexpr int max_boundary_count = 2000;
constexpr int boundary_layers = 2;
constexpr float GRAVITY_COEF = 80.f;
constexpr float RESTITUTION_COEF = 0.15f;
constexpr float GAS_CONSTANT = 90000.f;
constexpr float REST_DENSITY = .05f;
constexpr float VISCOSITY_COEFF = 90.0f;
constexpr float PARTICLE_MASS = 1.f;
constexpr float h = 15.f;
const float h8 = h * h * h * h * h * h * h * h;
const float h2 = h * h;
const float h5 = h * h * h * h * h;
} // namespace Config
//
