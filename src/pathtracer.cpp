#include "pathtracer.hpp"
#include <math.h>
#include "a4.hpp"
#include "image.hpp"
#include "ray.hpp"
#include "intersection.hpp"

#include <stdlib.h>     /* srand, rand */
#include <time.h> 	/* time */

#define RAND_01 ((double) rand() / (RAND_MAX))

#define RECURSIVE_DEPTH 8
#define SAMPLES 100

PathTracer::PathTracer(SceneNode* root,
           const std::string& filename,
           int width, int height,
           const Point3D& eye, const Vector3D& view,
           const Vector3D& up, double fov,
           const Colour& ambient,
           const std::list<Light*>& lights) : m_ambient(ambient) {

  srand (time(NULL));

  // Initialize some variables
  m_view = view;
  m_view.normalize();
  m_up = up;
  m_up.normalize();
  m_right = m_view.cross(m_up);
  m_right.normalize();
  m_tanf = tan((fov/2)*M_PI/180);
  m_aspect = (double)width/(double)height;

  // Load the background png
  m_background_image.loadPng("background.png");

  // Get the ratio so we can use the right background pixels
  m_width_ratio = (double)m_background_image.width()/(double)width;
  m_height_ratio = (double)m_background_image.height()/(double)height;

  // Create the image to save to

  m_width = width;
  m_height = height;
  m_ambient = ambient;
  m_lights = lights;
  m_root = root;
  m_eye = eye;
  m_filename = filename;

  for (std::list<Light*>::const_iterator I = m_lights.begin(); I != m_lights.end(); ++I) {
    Light *light = *I;

    GeometryNode* node = NULL;
    switch(light->getLightType()) {
      case 0:
        node = new GeometryNode("light", new NonhierSphere(light->position, 1));
        break;
      case 1:
        node = new GeometryNode("light", new NonhierSphere(light->position, light->getSize()));
        break;
    }

    node->set_material(new LightMaterial(light->colour, light));
    
    m_root->add_child(node);
  }
}

PathTracer::~PathTracer() {

}

void PathTracer::run() {
  Image m_save_image(m_width, m_height, 3);
  int cur_percent = 0;

  for(m_pixel_y = 0; m_pixel_y < m_height; m_pixel_y++) {
    for(m_pixel_x = 0; m_pixel_x < m_width; m_pixel_x++) {
      double r, g, b;

      r = g = b = 0;

      for(double i = 0; i < SAMPLES; i++) {
          double rayX = m_pixel_x + RAND_01*0.999999;
          double rayY = m_pixel_y + RAND_01*0.999999;

          Ray ray(m_eye, m_view + ((rayX/m_width)-0.5)*m_tanf*m_aspect*m_right + ((rayY/m_height)-0.5)*m_tanf*m_up);
          ray.direction.normalize();
          Colour pixel_colour = pathTrace(ray);

          r += pixel_colour.R();
          g += pixel_colour.G();
          b += pixel_colour.B();
      }

      m_save_image(m_pixel_x, m_height-m_pixel_y-1, 0) = r/SAMPLES;
      m_save_image(m_pixel_x, m_height-m_pixel_y-1, 1) = g/SAMPLES;
      m_save_image(m_pixel_x, m_height-m_pixel_y-1, 2) = b/SAMPLES;
    }
    
    if(((double)m_pixel_y/(double)m_height)*100 >= cur_percent + 10) {
      cur_percent += 10;
      std::cout << cur_percent << '%' << " complete" << std::endl; 
    }
  }

  m_save_image.savePng(m_filename);
}

Colour PathTracer::pathTrace(Ray ray) {

  Colour pixelColor = Colour(0.0);
  Colour recursiveColour = Colour(1);

  for(int depth = 0; depth < RECURSIVE_DEPTH; depth++) {
    Colour directLighting = Colour(0);
    Intersection rayI;

    m_root->intersect(ray, rayI);

    // If no intersection was found, return background color
    if(rayI.t == -1) {
      if(depth == 1) {
        return get_background_colour();
      } else {
        return Colour(0.9205, 0.9607, 1.0);
      }
    }

    Colour diffused = rayI.material->get_kd();
    double shininess = rayI.material->get_shininess();
    rayI.normal.normalize();

    if(rayI.material->isLight()) {
      return diffused;
    }

/**************************************************************************************
 ******** Calculate Direct Lighting
 **************************************************************************************/

    std::list<Light*>::const_iterator iter = m_lights.begin();
    for(int i = 0; i < rand() % m_lights.size(); i++){
      iter++;
    }
    Light *light = *iter;	// get a random light

    // Get light information
    Vector3D light_dir = light->position - rayI.point;
    double lightDistance = light_dir.length();
    light_dir.normalize();
    Ray lightRay(rayI.point, light_dir);

    // Check if the surface the intersection point is on is facing the light
    double lambent_coef = light_dir.dot(rayI.normal);
    if(lambent_coef < 0)  {
      continue;
    }

    double theta = RAND_01*(2.0 * M_PI); // theta == [0, 2*PI]
    double x = RAND_01*2.0 - 1.0;	 // x == [-1, 1]
    double s = sqrt(1.0 - x * x);
    Vector3D v(x, s * cos(theta), s * sin(theta));
    Point3D randomPoint = (light->position + (light->size*v));

    Vector3D sampleDir = randomPoint - rayI.point;
    sampleDir.normalize();
    Ray sampleRay(rayI.point, sampleDir);
      
    Intersection lightRayI;
    m_root->intersect(sampleRay, lightRayI, true);
    if(lightRayI.t != -1 && lightRayI.t < lightDistance && lightRayI.material->getLight() != light ) {
      directLighting = Colour(0);
    } else {
      directLighting = lambent_coef * light->getLight();
    }

    recursiveColour = recursiveColour * diffused;
    pixelColor = pixelColor + recursiveColour * directLighting;
  }

  return pixelColor;
}

Colour PathTracer::get_background_colour() {
  double r = m_background_image(m_pixel_x * m_width_ratio, (m_height-m_pixel_y-1) * m_height_ratio, 0);
  double g = m_background_image(m_pixel_x * m_width_ratio, (m_height-m_pixel_y-1) * m_height_ratio, 1);
  double b = m_background_image(m_pixel_x * m_width_ratio, (m_height-m_pixel_y-1) * m_height_ratio, 2);

  return Colour(r, g, b);
}







































