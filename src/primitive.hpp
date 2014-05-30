#ifndef CS488_PRIMITIVE_HPP
#define CS488_PRIMITIVE_HPP

#include "algebra.hpp"
#include <iostream>

class Primitive {
public:
  virtual ~Primitive();

  virtual double intersect(Point3D ray_point, Vector3D ray_dir, Point3D &intersection_point, Vector3D &intersection_normal) = 0;

protected:
  double intersect_cube(Point3D ray_point, Vector3D ray_dir, Point3D &intersection_point, Vector3D &intersection_normal, Point3D min_pos, Point3D max_pos);
  double intersect_sphere(Point3D ray_point, Vector3D ray_dir, Point3D &intersection_point, Vector3D &intersection_normal, Point3D pos, int radius);
};

class Sphere : public Primitive {
public:
  virtual ~Sphere();

  virtual double intersect(Point3D ray_point, Vector3D ray_dir, Point3D &intersection_point, Vector3D &intersection_normal);
};

class Cube : public Primitive {
public:
  virtual ~Cube();

  virtual double intersect(Point3D ray_point, Vector3D ray_dir, Point3D &intersection_point, Vector3D &intersection_normal);
};

class NonhierSphere : public Primitive {
public:
  NonhierSphere(const Point3D& pos, double radius)
    : m_pos(pos), m_radius(radius)
  {
  }
  virtual ~NonhierSphere();

  virtual double intersect(Point3D ray_point, Vector3D ray_dir, Point3D &intersection_point, Vector3D &intersection_normal);

private:
  Point3D m_pos;
  double m_radius;
};

class NonhierBox : public Primitive {
public:
  NonhierBox(const Point3D& pos, double size)
    : m_pos(pos), m_size(size)
  {
  }
  
  virtual ~NonhierBox();

  virtual double intersect(Point3D ray_point, Vector3D ray_dir, Point3D &intersection_point, Vector3D &intersection_normal);

private:
  Point3D m_pos;
  double m_size;
};

#endif