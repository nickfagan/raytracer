#include "mesh.hpp"
#include <iostream>
#include "limits.h"

Mesh::Mesh(const std::vector<Point3D>& verts,
           const std::vector< std::vector<int> >& faces)
  : m_verts(verts),
    m_faces(faces)
{
  int i, j;

  m_min_x = m_min_y = m_min_z = INT_MAX;
  m_max_x = m_max_y = m_max_z = INT_MIN;

  // Get the bounding box for quick intersection testing
  for(i = 0; i < m_faces.size(); i++) {
    Point3D origin = m_verts[m_faces[i][0]];

    set_min_max(origin);

    for(j = 1; j < m_faces[i].size() - 1; j++) {
      Triangle triangle;

      triangle.push_back(origin);
      triangle.push_back(m_verts[m_faces[i][j]]);
      triangle.push_back(m_verts[m_faces[i][j+1]]);

      set_min_max(m_verts[m_faces[i][j]]);
      set_min_max(m_verts[m_faces[i][j+1]]);

      m_triangles.push_back(triangle);
    }
  }

  m_min_x -= 0.1;
  m_min_y -= 0.1;
  m_min_z -= 0.1;
  m_max_x += 0.1;
  m_max_y += 0.1;
  m_max_z += 0.1;
}

void Mesh::set_min_max(Point3D point) {
    if(point[0] < m_min_x) {
      m_min_x = point[0];
    }
    if(point[1] < m_min_y) {
      m_min_y = point[1];
    }
    if(point[2] < m_min_z) {
      m_min_z = point[2];
    }

    if(point[0] > m_max_x) {
      m_max_x = point[0];
    }
    if(point[1] > m_max_y) {
      m_max_y = point[1];
    }
    if(point[2] > m_max_z) {
      m_max_z = point[2];
    }
}

double Mesh::intersect(Point3D ray_point, Vector3D ray_dir, Point3D &intersection_point, Vector3D &intersection_normal) {
  int i;
  float t1 = -1;

  double bounding_box_result = intersect_cube(ray_point, ray_dir, intersection_point, intersection_normal, Point3D(m_min_x, m_min_y, m_min_z), Point3D(m_max_x, m_max_y, m_max_z));

  if(bounding_box_result == -1) {
    return -1;
  }

  for(i = 0; i < m_triangles.size(); i++) {
    Point3D p1 = m_triangles[i][0];
    Point3D p2 = m_triangles[i][1];
    Point3D p3 = m_triangles[i][2];

    Vector3D v0 = p2 - p1;
    Vector3D v1 = p3 - p1;
    Vector3D n = v0.cross(v1);

    double dot1 = n.dot(ray_dir);

    // Plane is parallel to ray
    if (dot1 == 0) continue;

    double t = -n.dot(ray_point - p1) / dot1;
    
    // Intersection point is behind ray point    
    if (t < 0.1)
        continue;

    Point3D intersection_point = ray_point + t * ray_dir;

    Vector3D point_to_plane = intersection_point - p1;
    double dot2 = v0.dot(v0);
    double dot3 = v0.dot(v1);
    double dot4 = v1.dot(v1);
    double dot5 = point_to_plane.dot(v0);
    double dot6 = point_to_plane.dot(v1);
    double denom = dot3 * dot3 - dot2 * dot4;

    double a, b;
    a = (dot3 * dot6 - dot4 * dot5) / denom;
    if (a < 0.0 || a > 1.0)        
        continue;
    b = (dot3 * dot5 - dot2 * dot6) / denom;
    if (b < 0.0 || (a + b) > 1.0) 
        continue;

    if(t1 == -1 || t1 > t) {
      t1 = t;
      intersection_point = intersection_point;
      intersection_normal = n;
    }
  }

  return t1;
}

std::ostream& operator<<(std::ostream& out, const Mesh& mesh)
{
  std::cerr << "mesh({";
  for (std::vector<Point3D>::const_iterator I = mesh.m_verts.begin(); I != mesh.m_verts.end(); ++I) {
    if (I != mesh.m_verts.begin()) std::cerr << ",\n      ";
    std::cerr << *I;
  }
  std::cerr << "},\n\n     {";
  
  for (std::vector<Mesh::Face>::const_iterator I = mesh.m_faces.begin(); I != mesh.m_faces.end(); ++I) {
    if (I != mesh.m_faces.begin()) std::cerr << ",\n      ";
    std::cerr << "[";
    for (Mesh::Face::const_iterator J = I->begin(); J != I->end(); ++J) {
      if (J != I->begin()) std::cerr << ", ";
      std::cerr << *J;
    }
    std::cerr << "]";
  }
  std::cerr << "});" << std::endl;
  return out;
}
