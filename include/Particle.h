#pragma once
#include "Config.h"
#include "utils/Float2.h"
class Particle {
public:
  Particle(Float2 pos, Float2 vel, Float2 acc);

  void setPos(Float2 _pos);
  void setVel(Float2 _vvel);
  void setAcc(Float2 _acc);
  void setForce(Float2 _force);

  const Float2 getPos() const;
  const Float2 getVel() const;
  const Float2 getAcc() const;
  const Float2 getForce() const;

  float getDensity() const;
  float getMass() const;
  float getPressure() const;

  void setPressure(float _p);
  void setDensity(float _d);

  void addForce(Float2 _force);

private:
  Float2 m_pos;
  Float2 m_vel;
  Float2 m_acc;
  Float2 m_force;

  float m_density = 0.f;
  float m_mass = Config::PARTICLE_MASS;
  float m_pressure = 0.f;
};
