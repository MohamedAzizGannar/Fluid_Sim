#include "Particle.h"

Particle::Particle(Float2 pos, Float2 vel, Float2 acc)
    : m_pos(pos), m_vel(vel), m_acc(acc) {}

void Particle::setPos(Float2 _pos) { m_pos = _pos; }
void Particle::setVel(Float2 _vel) { m_vel = _vel; }
void Particle::setAcc(Float2 _acc) { m_acc = _acc; }
void Particle::setForce(Float2 _force) { m_force = _force; }
void Particle::setPressure(float _p) { m_pressure = _p; }
void Particle::setDensity(float _d) { m_density = _d; }

const Float2 Particle::getPos() const { return m_pos; }
const Float2 Particle::getVel() const { return m_vel; }
const Float2 Particle::getAcc() const { return m_acc; }
const Float2 Particle::getForce() const { return m_force; }

float Particle::getPressure() const { return m_pressure; }
float Particle::getDensity() const { return m_density; }
float Particle::getMass() const { return m_mass; }

void Particle::addForce(Float2 _force) { m_force += _force; }
