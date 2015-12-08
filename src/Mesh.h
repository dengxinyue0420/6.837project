#ifndef MESH_H
#define MESH_H

#include "Object3D.h"
#include "ObjTriangle.h"
#include "Octree.h"
#include "Triangle.h"
#include "Vector2f.h"
#include "Vector3f.h"

#include <vector>

class Mesh : public Object3D {
  public:
    Mesh(const std::string &filename, Material *m);

    virtual bool intersect(const Ray &r, float tmin, Hit &h, float range_x,float range_y, float clip) const;

    virtual bool intersectTrig(int idx,float range_x,float range_y, float clip) const;

	virtual void preRender(float t);

    const std::vector<Triangle> & getTriangles() const {
        return _triangles;
    }

  private:
    std::vector<Triangle> _triangles;
    mutable Octree octree;
    mutable const Ray *ray;
    mutable Hit *hit;
    mutable float tm;
};

#endif
