#pragma once

#include "TransformableObject.h"
#include "NameableObject.h"
#include "Material.h"
#include "Ray.h"
#include "Vector3.h"
#include "Matrix3x3.h"

#include "geometry/Shape3.h"
#include "geometry/Intersection.h"

class Actor : public virtual cg::TransformableObject, public virtual cg::NameableObject{
protected:
    cg::Reference<Shape3> _shape;
    cg::Reference<cg::Material> _material;
    cg::mat3f _normalMatrix;

public:
    Actor(cg::Reference<cg::Material>& material, const char* name = "Actor"):
    _material(material), cg::NameableObject(name){}
    
    bool intersect(cg::Ray3f&, Intersection& hit);
    cg::vec3f normal(Intersection& hit);
};

inline cg::vec3f 
Actor::normal(Intersection& hit){

    return _normalMatrix.transform(_shape->normal(hit.local_ray));
}

inline bool
Actor::intersect(cg::Ray3f& ray, Intersection& hit){
    
    cg::vec3f local_origin = worldToLocalMatrix().transform3x4(ray.origin);
    cg::vec3f local_direction = worldToLocalMatrix().transformVector(ray.direction);

    cg::Ray3f local_ray{local_origin, local_direction};

    float distance;

    if(_shape->intersect(local_ray, distance)){
        hit.actor = this;
        hit.distance = distance;
        hit.local_ray = local_ray;
        return true;
    }
    return false;
}