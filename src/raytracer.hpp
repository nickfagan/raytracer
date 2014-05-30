#ifndef CS488_RENDERER_HPP
#define CS488_RENDERER_HPP

#include "algebra.hpp"
#include <iostream>
#include <string>
#include "algebra.hpp"
#include "scene.hpp"
#include "light.hpp"
#include "image.hpp"
#include "ray.hpp"

class RayTracer {
public:
  RayTracer(SceneNode* root,
               const std::string& filename,
               int width, int height,
               const Point3D& eye, const Vector3D& view,
               const Vector3D& up, double fov,
               const Colour& ambient,
               const std::list<Light*>& lights
               );
  ~RayTracer();

  void run();

private:
  Colour rayTrace(Ray ray, int ray_depth);
  Colour get_background_colour();

private:
  SceneNode* m_root;
  int m_pixel_x, m_pixel_y;
  double m_height, m_width;
  double m_width_ratio, m_height_ratio;
  double m_tanf, m_aspect;
  Vector3D m_view, m_up, m_right;
  Point3D m_eye;
  Image m_background_image;
  Image m_save_image;
  Colour m_ambient;
  std::list<Light*> m_lights;
  std::string m_filename;
};

#endif
