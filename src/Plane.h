#ifndef PLANE_H
#define PLANE_H

#include "Object3D.h"

#include <vecmath.h>
#include <cmath>

// TODO: Implement Plane representing an infinite plane
// Choose your representation, add more fields and fill in the functions
class Plane: public Object3D
{
  public:
    Plane(const Vector3f &normal, float d, Material *m) :
      Object3D(m)
    {
      _normal = normal.normalized();
      offset = d;
    }

    virtual bool intersect(const Ray &r, float tmin, Hit &h, float range_x,float range_y,float clip) const 
    {
      float denom = Vector3f::dot(r.getDirection(),_normal);
      if(denom==0){
	return false;
      }else{
	float t = (offset-Vector3f::dot(r.getOrigin(),_normal))/denom;
	if(t>=tmin&&t<h.getT()){
	  h.set(t,this->getMaterial(),_normal);
	  return true;
	}
      }
      return false;
    }

 private:
    Vector3f _normal;
    float offset;
};
#endif //PLANE_H
