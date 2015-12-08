#ifndef GROUP_H
#define GROUP_H

#include "Object3D.h"
#include "Ray.h"
#include "Hit.h"

#include <iostream>
#include <vector>
/// TODO:
/// Implement Group
/// Add data structure to store a list of Object*
class Group : public Object3D
{
public:
    // Constructor
    Group()
    {
    }

    // Destructor
    ~Group()
    {
    }

    // Return true if intersection found
    virtual bool intersect(const Ray &r, float tmin, Hit &h,float range_x,float range_y, float clip) const
    {
        bool flag = false;
        for(int i=0;i<objs.size();i++){
            if(objs[i]->intersect(r,tmin,h,range_x,range_y,clip)){
                flag = true;
            }
        }
        return flag;
    }

    virtual void preRender(float t) {
        for (int i = 0; i < objs.size(); i++){
            objs[i] -> preRender(t);
        }
    }

    // Add object to group
    void addObject(Object3D *obj)
    {
        objs.push_back(obj);
    }

    // Return number of objects in group
    int getGroupSize()
    {
        return objs.size();
    }

private:
    std::vector<Object3D*> objs;
};

#endif
