#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Object3D.h"
#include <vecmath.h>
#include <cmath>
#include <iostream>

class Triangle: public Object3D
{
  public:
    Triangle(const Vector3f &a, 
             const Vector3f &b, 
             const Vector3f &c, 
             const Vector3f &na,
             const Vector3f &nb,
             const Vector3f &nc,
             Material *m) :
        Object3D(m),
        _hasTex(false)
    {
        _v[0] = a;
        _v[1] = b;
        _v[2] = c;
        _normals[0] = na;
        _normals[1] = nb;
        _normals[2] = nc;
    }

    virtual bool intersect(const Ray &ray, float tmin, Hit &hit) const {
        const Vector3f &Rd = ray.getDirection();
        const Vector3f &Ro = ray.getOrigin();
        
        Matrix3f A(_v[0][0] - _v[1][0], _v[0][0] - _v[2][0], Rd[0],
                   _v[0][1] - _v[1][1], _v[0][1] - _v[2][1], Rd[1],
                   _v[0][2] - _v[1][2], _v[0][2] - _v[2][2], Rd[2]);

        Vector3f b(_v[0][0]-Ro[0], _v[0][1] - Ro[1], _v[0][2] - Ro[2]);

        Vector3f x = A.inverse() * b;

        float beta = x[0], gamma = x[1], t = x[2];

        if ((beta>=0.0)&&(gamma>=0.0)&&(beta+gamma<=1)) {
            if ((t < tmin) || (t>hit.getT())) { 
                return false;
            } else{
                Vector3f direction;

                direction = 
                    (1 - beta - gamma) * _normals[0] + beta * _normals[1] + gamma * _normals[2];

                // no intersection
                if (Vector3f::dot( direction , Rd ) == 0) {
                    return false;
                }
                direction = direction / direction.abs();
                hit.set(t, getMaterial(), direction);
                if (hasTex()) {
                    Vector2f texC =
                        (1 - beta - gamma) * _texCoords[0] +
                                      beta * _texCoords[1] +
                                     gamma * _texCoords[2];
                    hit.setTexCoord(texC);
                }
                return true;
            }
        }
        return false;
    }

    void setTex(const Vector2f &uva,
                const Vector2f &uvb,
                const Vector2f &uvc)
    {
        _texCoords[0] = uva;
        _texCoords[1] = uvb;
        _texCoords[2] = uvc;
        _hasTex = true;
    }

    bool hasTex() const {
        return _hasTex;
    }

    const Vector3f & getVertex(int index) const {
        assert(index < 3);
        return _v[index];
    }

    const Vector3f & getNormal(int index) const {
        assert(index < 3);
        return _normals[index];
    }

    const Vector2f & getTexCoord(int index) const {
        assert(index < 3);
        return _texCoords[index];
    }

  private:
    bool _hasTex;
    Vector3f _v[3];
    Vector3f _normals[3];
    Vector2f _texCoords[3];
};

#endif //TRIANGLE_H
