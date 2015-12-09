#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Object3D.h"
#include "vecmath.h"

// TODO implement this class
// So that the intersect function first transforms the ray
// Add more fields as necessary
class Transform: public Object3D
{
public:
    Transform(const Matrix4f &m,
        Object3D *obj,
        bool explosion) :
        _object(obj),
        _m(m),
        _exp(explosion)
        {
            _om = m;
        }

        virtual bool intersect(const Ray &r, float tmin, Hit &h, float range_x,float range_y,float clip) const
        {
            Vector4f t_origin = _m.inverse()*Vector4f(r.getOrigin(),1);
            Vector4f t_dir = _m.inverse()*Vector4f(r.getDirection(),0);
            Vector3f neworigin = t_origin.xyz();
            Vector3f newDir = t_dir.xyz();
            //newDir.normalize();
            Ray t_ray = Ray(neworigin,newDir);
            if(_object->intersect(t_ray, tmin, h, range_x,range_y,clip)){
                Vector3f n_os = h.getNormal();
                Vector4f n_ws = _m.inverse().transposed()*Vector4f(n_os,0);
                h.setNormal(n_ws.xyz().normalized());
                return true;
            }else{
                return false;
            }
        }

        virtual void preRender(float t) {

            if (_exp){
				float scaleFactor = - 1.5*t*t + 2.8*t + 0.033;

                _m = _om * Matrix4f::uniformScaling(scaleFactor) ;
            }
            _object->preRender(t);
        }

    protected:
        Object3D *_object; //un-transformed object
        Matrix4f _m;
        Matrix4f _om;
        bool _exp;
    };

    #endif //TRANSFORM_H
