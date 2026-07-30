#pragma once
namespace Config {

constexpr float maxSpeedAllowed = 80.f;
constexpr int windowWidth = 1000;
constexpr int windowHeight = 600;

constexpr float timeStep = 1.f / 120.f;

constexpr float spacing = 7.f;

constexpr int countX = 100;
constexpr int countY = 60;

constexpr int offsetX = 150;
constexpr int offsetY = 90;

constexpr int max_boundary_count = 2000;
constexpr int boundary_layers = 2;
constexpr float GRAVITY_COEF = 20.f;
constexpr float RESTITUTION_COEF = 0.15f;
constexpr float GAS_CONSTANT = 100000.f;
constexpr float REST_DENSITY = .1f;
constexpr float VISCOSITY_COEFF = 60.0f;
constexpr float PARTICLE_MASS = 3.f;
constexpr float h = 15.f;
const float h8 = h * h * h * h * h * h * h * h;
const float h2 = h * h;
const float h5 = h * h * h * h * h;
} // namespace Config
//
