#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "Ray.h"
#include "Hit.h"
#include "Material.h"

#include <string>

class Object3D
{
  public:
    Object3D()
    {
        material = NULL;
    }

    virtual ~Object3D() {}

    Object3D(Material *material)
    {
        this->material = material;
    }

    const std::string & getType() const {
        return type;
    }

    Material * getMaterial() const {
        return material;
    }

    virtual bool intersect(const Ray &r, float tmin, Hit &h, float range_x, float range_y, float clip) const=0;

	virtual void preRender(float t) {};

  private:
    std::string type;
    Material *material;
};

#endif
