#ifndef CS488_MATERIAL_HPP
#define CS488_MATERIAL_HPP

#include "algebra.hpp"
#include "light.hpp"

class Material {
public:
  Material(const Colour& kd, const Colour& ks, double shininess);
  virtual ~Material();
  virtual Colour get_kd();
  virtual Colour get_ks();
  virtual double get_shininess();
  virtual double get_reflection();
  virtual bool isLight();
  virtual Light* getLight();

protected:
  Colour m_kd;
  Colour m_ks;
  double m_shininess;
};

class RefMaterial : public Material {
public:
  RefMaterial(const Colour& kd, const Colour& ks, double shininess, double reflection);
  virtual ~RefMaterial();
  virtual double get_reflection();
  double m_reflection;
};

class LightMaterial : public Material {
public:
  LightMaterial(const Colour& kd, Light* l);
  virtual ~LightMaterial();
  virtual bool isLight();
  virtual Light* getLight();
  
private:
  Light* mLight;
};

#endif