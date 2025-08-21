#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Object.h"
#include "Material.h"

class Triangle : public Object {  
public:  
   Vec3f v0, v1, v2; // Vertices of the triangle  
   Vec3f n; // Normal of the triangle  
   const Material* material; // Material of the triangle  

   Triangle(const Vec3f& a, const Vec3f& b, const Vec3f& c, const Material* mat)  
       : v0(a), v1(b), v2(c), material(mat) {  
       n = ((v1 - v0).cross(v2 - v0)).normalize();  
   }  

   bool hit(const Ray& ray, HitRec& hitRec) const;  

   const Material* getMaterial() const;  

   void computeSurfaceHitFields(const Ray& r, HitRec& rec) const;  
};
#endif